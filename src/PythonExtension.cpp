
#include <iostream>
#include <Python.h>
#include <pch.h>
//#include "VideoPlayer/VLCPlayer.h"


// 声明C++函数，将被Python调用
void attachSubtitle(const std::string& path);

// Python调用C++函数时的包装器
static PyObject* cppmodule_attachSubtitle(PyObject* self, PyObject* args) {
	const char* path;

	// 解析Python传递的参数
	if (!PyArg_ParseTuple(args, "s", &path)) {
		return nullptr; // 参数解析失败
	}

	// 调用实际的C++函数
	attachSubtitle(std::string(path));

	// 返回None给Python
	Py_RETURN_NONE;
}

// 方法映射表：Python可调用的C++函数列表
static PyMethodDef CppMethods[] = {
	{
		"attachSubtitle",          // Python中调用的函数名
		cppmodule_attachSubtitle,  // 对应的C++函数包装器
		METH_VARARGS,              // 表示函数接受参数元组
		"调用C++的attachSubtitle方法" // 函数说明文档
	},
	{nullptr, nullptr, 0, nullptr} // 结束标记
};

// 模块定义结构
static struct PyModuleDef cppmodule = {
	PyModuleDef_HEAD_INIT,
	"cppmodule",   // 模块名
	"C++模块，供Python调用", // 模块说明
	-1,            // 模块状态大小
	CppMethods     // 模块方法列表
};

// 模块初始化函数（必须命名为PyInit_<模块名>）
PyMODINIT_FUNC PyInit_cppmodule(void) {
	return PyModule_Create(&cppmodule);
}


extern void LoadSubtitle(const TCHAR* filePath);
extern void resetOSD();

// 实际的C++实现函数
void attachSubtitle(const std::string& path) {
	std::cout << "C++: 接收到字幕文件 - " << path << std::endl;
	std::cout << "C++: 正在将字幕附加到视频..." << std::endl;
	QkString data = QkString(path.data());
	lxx(caption::ss, STR(data));

	LoadSubtitle(STR(data));
	resetOSD();
}


extern QkString bin_path;

int py_main(QkString path) {
	try {
		std::cout << "C++: 调用Python的onPlay方法，视频路径: ";
		lxx(C++: 调用Python的onPlay方法，视频路径)
		// 1. 指定Python安装目录
		std::string pythonDir = "C:/python37";


		//setPythonPath("I:\\Explore\\quickjscpp\\example\\Release", "I:\\Explore\\quickjscpp\\example\\Release\\python");
		//setPythonPath("I:\\Explore\\quickjscpp\\example\\Release\\python");
		//setPythonPath(pythonDir);

		// 2. 在初始化前向Python注册我们的模块
		PyImport_AppendInittab("cppmodule", &PyInit_cppmodule);


		// 4. 初始化Python解释器
		Py_Initialize();

		if (!Py_IsInitialized()) {
			throw std::runtime_error("Python初始化失败");
		}

		// 5. 导入并运行同目录下的test.py
		//QkString scritps_path = bin_path;
		//scritps_path.Append(L"\\scripts");
		//PySys_SetPath(scritps_path);
		//PySys_SetPath(L"D:\\Code\\WODPlayer\\bin;D:\\Code\\WODPlayer\\bin\\scripts");
		PyObject* pModule = PyImport_ImportModule("main");
		lxx("onPlay::pModule dd", pModule)
		if (!pModule) {
			PyErr_Print();
			throw std::runtime_error("无法导入test.py模块");
		}

		// 6. 获取test.py中的onPlay函数
		PyObject* pFunc = PyObject_GetAttrString(pModule, "onPlay");
		lxx("onPlay::dd", pFunc)
		if (!pFunc || !PyCallable_Check(pFunc)) {
			if (PyErr_Occurred()) PyErr_Print();
			throw std::runtime_error("无法找到可调用的onPlay函数");
		}

		// 7. 准备调用参数（视频路径）
		std::string videoPath = "sample.mp4";
		path.GetData(videoPath);
		PyObject* pArgs = PyTuple_New(1);
		PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(videoPath.c_str()));

		// 8. 调用Python的onPlay函数
		std::cout << "C++: 调用Python的onPlay方法，视频路径: " << videoPath << std::endl;
		PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

		// 9. 处理返回结果
		if (pResult) {
			const  char* resultStr = PyUnicode_AsUTF8(pResult);
			if (resultStr) {
				std::cout << "C++: Python返回结果: " << resultStr << std::endl;
				lxx(ss, resultStr)
			}
			Py_DECREF(pResult);
		}
		else {
			PyErr_Print();
			throw std::runtime_error("调用onPlay函数失败");
		}

		// 10. 清理资源
		Py_DECREF(pArgs);
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
		Py_Finalize();
	}
	catch (const std::exception& e) {
		lxxx(错误:  ss, e.what())
		std::cerr << "错误: " << e.what() << std::endl;
		Py_Finalize();
		return 1;
	}

	return 0;
}
