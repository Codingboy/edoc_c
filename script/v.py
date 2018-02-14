#! /usr/bin/env python

import sys
import re

mode = 0;
#0 = print version
#1 = build
#2 = minor
#3 = major
filename = ""
for arg in sys.argv:
	if arg == "--build":
		mode = 1
	elif arg == "--minor":
		mode = 2
	elif arg == "--major":
		mode = 3
	elif arg != sys.argv[0]:
		filename = arg
file = open(filename, "r")
data = list()
for line in file:
	if "#define VERSION \"" in line:
		version = re.split("\"", line)[1]
		versionarray = re.split("\.", version)
		major = versionarray[0]
		minor = versionarray[1]
		build = versionarray[2]
		if mode == 0:
			print version
			sys.exit()
		elif mode == 1:
			build = int(build) + 1
		elif mode == 2:
			minor = int(minor) + 1
			build = 0
		elif mode == 3:
			build = 0
			minor = 0
			major = int(major) + 1
		newversion = str(major) + "." + str(minor) + "." + str(build)
		newline = re.split("\"", line)[0] + "\"" + newversion + "\"" + re.split("\"", line)[2]
		line = newline
	data.append(line)
file.close()
file = open(filename, "w")
for line in data:
	file.write(line)
file.close()
