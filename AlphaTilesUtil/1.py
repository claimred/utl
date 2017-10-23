import os
import sys

List = open("confs.txt")

for line in List.readlines():
	num = int(line[line.find("[") + 1:line.find("]")])
	num = num - 1

#	print line[:line.find("[") + 1]
#	print num
#	print line[line.find("]"):]

	newstr = "%s%d%s" % (line[:line.find("[") + 1], num, line[line.find("]"):])
	sys.stdout.write(newstr)

