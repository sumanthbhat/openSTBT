import sys
import os

FRAME_SIZE = 691200
print "Extracting frames from given source"
YUV_422_video_file_src_720_480 = str(sys.argv[1])

try:
	fp = open(YUV_422_video_file_src_720_480)

except:
	print "Error opening file"
	sys.exit(0)

file_size = os.path.getsize(YUV_422_video_file_src_720_480)

Number_of_frames = file_size/FRAME_SIZE

File_name_base = "Frame"
for frame_count in range(0,Number_of_frames):
	data = fp.read(FRAME_SIZE)
	file_name = File_name_base + str(frame_count) + ".yuv"
	eF = open(file_name,"w+")
	eF.write(data)
	eF.close()

fp.close()
