import  os
import  sys
import  time

from  opt import *
from on_play import *


def guessPlay(path):
	idx = path.rfind("?")
	tag = ""
	if idx>0:
		tag = path[idx+1:]
		path = path[0:idx]
	path = path.replace("|", "｜") # yeah
	path = path.replace(":", "：") # yeah
	print('guessPlay', tag)
	if tag:
		if "Godot FPS Tutorial" in tag:
			return guessPlayAt(path, r"T:\陪伴学习\Godot\Godot FPS Tutorial")
		return None
	return guessPlayAt(path, r"r:\ship")


def guessPlayAt(path, folder_path):
	print(f"正在扫描文件夹: {folder_path}\n")
	
	srt_files = []
	ref_name = path
	if not ref_name:
		return ref_name
	
	# ref_name = ref_name.lower()
	len_a = len(ref_name)
	len_h = int(len_a*0.95)
	len_h = max(len_h, 10)
	mid = mid_pattern(len_a, ref_name)
	
	print('mid', mid, len_a)
	
	if not mid:
		return None
	
	items = os.listdir(folder_path)
	
	for item in items:
		if item.endswith('.mp4') or item.endswith('.MP4'):
			item_path = os.path.join(folder_path, item)
			if os.path.isfile(item_path):
				srt_name = name_nosuffix(item)
				rate = compare_strings(len_a, len_h, mid, ref_name, srt_name)
				print('rate', rate if rate else opt.err, srt_name)
				if rate and rate[0]>0:
					srt_files.append([item_path, rate[0], rate[1]])
	
	if not srt_files:
		return None
	
	srt_files.sort(key=lambda x: -x[1])
	max_similarity = srt_files[0][1]
	
	top_candidates = []
	for f in srt_files:
		if f[1] == max_similarity or 1:
			top_candidates.append(f)
		else:
			break
	
	if len(top_candidates) > 1:
		top_candidates.sort(key=lambda x: (-x[2], -os.path.getmtime(x[0])))
		
	# print('top_candidates', top_candidates)
	# print('top_candidates', top_candidates[0][0])
	
	return top_candidates[0][0]
	
	
	



if __name__ == "__main__":
	print('guess::', guessPlay(r"Noclip For FPS Character Controllers - Godot 4 Tutorial"))
		  
		  
		  
		  
		  
		  
		  