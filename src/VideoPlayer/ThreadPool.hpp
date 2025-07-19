#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <chrono>
#include <map>
#include <memory>

class LambdaTaskManager;
__declspec(selectany) LambdaTaskManager* instance = 0;

class LambdaTaskManager {
public:
    // 获取单例实例
    static LambdaTaskManager& getInstance() {
        if(!instance|| instance->shutdown_)
            instance = new LambdaTaskManager();
        return *instance;
    }

    // 禁止拷贝和赋值
    LambdaTaskManager(const LambdaTaskManager&) = delete;
    LambdaTaskManager& operator=(const LambdaTaskManager&) = delete;

    // 提交lambda任务，返回任务ID
    size_t postLambda(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mutex_);

        // 生成任务ID
        size_t taskId = ++taskIdCounter_;

        // 将任务加入队列
        taskQueue_.emplace(taskId, std::move(task));

        // 重置空闲计时器
        resetIdleTimer();

        // 如果线程没有运行，则启动
        if (!workerThread_.joinable()) {
            lxx(新县城)
            workerThread_ = std::thread(&LambdaTaskManager::workerFunction, this);
        }

        // 通知工作线程
        cv_.notify_one();

        return taskId;
    }

    // 析构函数，确保线程正确结束
    ~LambdaTaskManager() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            lxx(线程关闭!!)
            shutdown_ = true;
            cv_.notify_all();
        }

        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }

private:
    LambdaTaskManager()
        : shutdown_(false),
        taskIdCounter_(0),
        idleTimerReset_(false) {}

    // 工作线程函数
    void workerFunction() {
        auto idleTimeout = std::chrono::seconds(8);
        //auto idleTimeout = std::chrono::minutes(15);
        auto lastActivityTime = std::chrono::steady_clock::now();

        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);

            // 检查是否有任务
            if (!taskQueue_.empty()) {
                // 取出任务
                auto taskPair = std::move(taskQueue_.front());
                taskQueue_.pop();
                lock.unlock();

                // 执行任务
                try {
                    taskPair.second();
                }
                catch (...) {
                    // 捕获所有异常，防止线程因异常退出
                }

                // 更新最后活动时间
                lastActivityTime = std::chrono::steady_clock::now();
                idleTimerReset_ = false;
            }
            else {
                // 检查是否应该关闭
                if (shutdown_) {
                    return;
                }

                // 等待任务或超时
                if (cv_.wait_for(lock, idleTimeout, [this] {
                    return !taskQueue_.empty() || shutdown_ || idleTimerReset_;
                    })) {
                    // 如果有新任务到达或需要关闭
                    if (shutdown_) {
                        return;
                    }

                    if (idleTimerReset_) {
                        // 重置计时器
                        lastActivityTime = std::chrono::steady_clock::now();
                        idleTimerReset_ = false;
                        continue;
                    }
                }
                else {
                    // 超时，检查是否真的空闲了这么久
                    auto now = std::chrono::steady_clock::now();
                    if (now - lastActivityTime >= idleTimeout) {
                        // 真正的空闲超时，退出线程
                        shutdown_ = true;
                        return;
                    }
                    // 否则继续等待剩余时间
                }
            }
        }
    }

    // 重置空闲计时器
    void resetIdleTimer() {
        idleTimerReset_ = true;
        cv_.notify_all();
    }

private:
    std::thread workerThread_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::pair<size_t, std::function<void()>>> taskQueue_;
    std::atomic<bool> shutdown_;
    std::atomic<size_t> taskIdCounter_;
    std::atomic<bool> idleTimerReset_;
};
