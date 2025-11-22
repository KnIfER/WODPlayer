import  os
import  sys
import  time


# import sys
# sys.path.insert(0, '.')

from  opt import *
print('play opt::', opt.app)
	

def compare_strings(len_a, len_h, mid, a, b):
	len_b = len(b)
	print('len_b', len_b, len_a-len_h,len_a+len_h )
	if len_b<len_a-len_h: #  or len_b>len_a+len_h
		opt.err = -2
		return
	if b.find(mid)<0:
		opt.err = -1
		return
	
	max_length_1 = 0 
	max_length = 0 
	grade = 0 
	current_length = 0 
	offset_start_a = 0 
	offset_start_b = 0 
	
	for i in range(len_a):
		for j in range(len_b):
			if a[i] == b[j]:
				current_length = 1
				x, y = i + 1, j + 1
				while x < len_a and y < len_b and a[x] == b[y]:
					current_length += 1
					x += 1
					y += 1
				# 更新最长片段长度
				if current_length > 3:
					if current_length > max_length:
						if max_length: # yeah
							max_length_1 = max_length
						max_length = current_length
						offset_start_a = i
						offset_start_b = j
				# 跳过已处理的字符，避免重复计算
				i = x - 1
				break
	
	
	# return (max_length, max_length/len_b, a[offset_start:offset_start+max_length])
	# return (max_length, max_length/len_b, len_a*0.65)
	# return (max_length, max_length/len_b, max_length/min(len_a, len_b))
	if max_length>len_a*0.3 or max_length/min(len_a, len_b)>0.3:
		print('max_length ===>', max_length, a[offset_start_a:offset_start_a+max_length])
		# second pass
		current_length_1 = 0 
		offset_start_a_1 = 0 
		for i in range(offset_start_a+max_length, len_a):
			for j in range(offset_start_b+max_length, len_b):
				if a[i] == b[j]:
					current_length_1 = 1
					x, y = i + 1, j + 1
					while x < len_a and y < len_b and a[x] == b[y]:
						current_length_1 += 1
						x += 1
						y += 1
					# 更新最长片段长度
					if current_length_1 > 3:
						if current_length_1 > max_length_1:
							max_length_1 = current_length_1
							offset_start_a_1 = i 
					i = x - 1
					break
		
		# return (-max_length, max_length/len_b, a[offset_start_a:offset_start_a+max_length], a[offset_start_a_1:offset_start_a_1+max_length_1], max_length_1)
		max_length += max_length_1
		grade = max_length/min(len_a, len_b)
		if max_length>len_a*0.7 or grade>0.7:
			return (max_length, max_length/len_b, grade)
	# opt.err = -3
	# return (-3, max_length, max_length_1, len_a*0.7, grade)

# 优化：如果  mid:mid+mid_ln 中含有标点符号，依次前后偏移 mid，直到没有标点符号
# def mid_pattern(len_a, a):
# 	mid = int(len_a/2)-1
# 	mid_ln = 3
# 	if len_a==4:
# 		mid_ln = 2
# 	return a[mid:mid+mid_ln]
	
punctuation = ' \t!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~，。、；：？！…（）【】《》‘’“”'

def has_punctuation(s):
	return any(char in punctuation for char in s)
	
def mid_pattern(len_a, a):
	mid = int(len_a / 2) - 1
	mid_ln = 3 if len_a != 4 else 2
	
	current_mid = mid
	substring = a[current_mid : current_mid + mid_ln]
	
	if not has_punctuation(substring):
		return substring
	
	# 初始子串含标点，开始偏移寻找（先向后再向前，逐步扩大偏移量）
	offset = 1
	while True:
		# 向后偏移
		new_mid = mid + offset
		if new_mid + mid_ln <= len_a:
			candidate = a[new_mid : new_mid + mid_ln]
			if not has_punctuation(candidate):
				return candidate
		
		# 向前偏移
		new_mid = mid - offset
		if new_mid >= 0:
			candidate = a[new_mid : new_mid + mid_ln]
			if not has_punctuation(candidate):
				return candidate
		
		offset += 1
		
		if offset > len_a:
			return None
	return None
	
	
def name_nosuffix(name):
	# os.path.splitext(name)[0]
	idx = name.find(".", max(0, len(name)-5))
	if(idx>0):
		return name[0:idx]
	return None


def find_srt(file_path, auxiliary = None, items = None):
	if auxiliary:
		folder_path = auxiliary
	else:
		folder_path = os.path.dirname(file_path)
	print(f"正在扫描文件夹: {folder_path}\n")
	
	srt_files = []
	ref_name = name_nosuffix(os.path.basename(file_path))
	if not ref_name:
		return ref_name
	
	# ref_name = ref_name.lower()
	len_a = len(ref_name)
	len_h = int(len_a*0.95)
	len_h = max(len_h, 10)
	mid = mid_pattern(len_a, ref_name)
	
	print('mid', mid)
	
	if not mid:
		return None
	
	
	if not items:
		items = os.listdir(folder_path)
	for item in items:
		if item.endswith('.srt') or item.endswith('.SRT'):
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
		

def onPlay(path):
	drive = path[0].lower()
	doit = True
	auxiliary = None
	
	if(drive!='r') :
		doit = False
		folder = os.path.dirname(path)
		if(folder.find("学习")>0):
			doit = True
		elif(folder.find("教程")>0):
			doit = True
		elif(folder.find("Toutorial")>0):
			doit = True
	
	if not doit:
		# print("nothing")
		return "nothing"
	elif drive=='t':
		auxiliary = r"I:\Softwares\Subtitles"
	
	print(f"Python: 收到播放请求，处理视频 ")
	
	found = None
	if auxiliary:
		found = find_srt(path, auxiliary)
	if not found:
		found = find_srt(path, None)
	
	if(found):
		print("attachSubtitle")
		if opt.app:
			import cppmodule
			cppmodule.attachSubtitle(found)
	
	print ("时间::", int(time.time()-opt.start)*1000)
	return "sucess"


if __name__ == "__main__":
	if not opt.app:
		onPlay(r"R:\DY\Aircraft Control Surfaces Explained  Ailerons, flaps, elevator, rudder and more.mp4")