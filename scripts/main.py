# 导入C++注册的模块
import os
import importlib
import sys
from pathlib import Path


# import scripts
# import scripts.opt as option
# from scripts.opt import *

# opt = scripts.opt.opt

import sys
sys.path.insert(0, '.')

# sys.path.append("D:\Code\WODPlayer\bin\scripts")
# # import scripts

# opt = importlib.import_module(r"opt")


# opt = scripts.opt
# from  scripts import opt

# opt.init()


# 获取当前文件夹路径，添加到模块搜索路径
current_dir = Path(Path(os.path.abspath(__file__)).parent, "scripts").as_posix()
sys.path.append(current_dir)

opt = None

if not opt:
	opt = importlib.import_module("opt").opt

	opt.init=1
	opt.app=not sys.executable.endswith("python.exe")

	# import scripts.on_play as player

	# 日志文件路径
	LOG_FILE_PATH = r"R:\cache\wodpy.log"

	class LogRedirector:
		def __init__(self, original_stream):
			self.original_stream = original_stream
			os.makedirs(os.path.dirname(LOG_FILE_PATH), exist_ok=True)
			self.log_file = open(LOG_FILE_PATH, 'a', encoding='utf-8')

		def write(self, message):
			self.log_file.write(message)
			# self.log_file.flush()
			# self.original_stream.write(message)

		def flush(self):
			self.log_file.flush()
			# self.original_stream.flush()

		def close(self):
			if not self.log_file.closed:
				self.log_file.close()
			pass

	if opt.app:
		original_stdout = sys.stdout
		original_stderr = sys.stderr
		sys.stdout = LogRedirector(original_stdout)
		sys.stderr = LogRedirector(original_stderr)

	print('init opt?', opt.app)


def onPlay(path):
	# 获取当前文件夹下所有.py文件
	for file in os.listdir(current_dir):
		if file.endswith('.py') and file not in ['__init__.py', os.path.basename(__file__), "opt.py"]:
			print("run file ===========>", file)
			module_name = Path(file).stem
			try:
				module = importlib.import_module(module_name)
				if hasattr(module, 'onPlay') and callable(module.onPlay):
					print(f"运行 {module_name}.onPlay()...")
					# 调用onPlay函数
					module.onPlay(path)
					print(f"{module_name}.onPlay() 执行完成\n")
				else:
					print(f"模块 {module_name} 中未定义可调用的 onPlay() 函数，跳过\n")
					
			except Exception as e:
				print(f"导入或运行模块 {module_name} 时出错: {str(e)}\n")
			

def guessPlay(path):
	print('guessPlay', path)
	for file in os.listdir(current_dir):
		if file.endswith('.py') and file not in ['__init__.py', os.path.basename(__file__), "opt.py"]:
			print("\n\nrun file ===========>", file)
			module_name = Path(file).stem
			try:
				module = importlib.import_module(module_name)
				if hasattr(module, 'guessPlay') and callable(module.guessPlay):
					print(f"运行 {module_name}.guessPlay()...")
					# 调用onPlay函数
					ret =  module.guessPlay(path)
					print(f"{module_name}.guessPlay() 执行完成={ret} n")
					if ret:
						return ret
				else:
					print(f"模块 {module_name} 中未定义可调用的 guessPlay() 函数，跳过\n")
					
			except Exception as e:
				print(f"导入或运行模块 {module_name} 时出错: {str(e)}\n")
	return ""
	

if __name__ == "__main__":
	print("开始导入当前文件夹下的所有模块并执行 onPlay() 函数...\n")
	onPlay(r"R:\DY\Aircraft Control Surfaces Explained  Ailerons, flaps, elevator, rudder and more.mp4")
	print("所有模块处理完毕")
