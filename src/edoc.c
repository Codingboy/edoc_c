#include "version.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	for (int i=0; i<argc; i++)
	{
		printf("%s", argv[i]);
	}
/*
	parser.add_argument("-e", "--encode", action="store_true", help="Specify mode: encode")
	parser.add_argument("-d", "--decode", action="store_true", help="Specify mode: decode")
	parser.add_argument("-p", "--password", action="store", metavar="password", help="Specify password.")
	parser.add_argument("-f", "--file", help="Specify file/folder.")
	parser.add_argument("-t", "--test", action="store_true", help="Runs unittests.")
	args = vars(parser.parse_args())
	file = args["file"]
	while file[-1] == os.sep:
		file = file[:-1]
	password = args["password"]
	encodeMode = args["encode"]
	testMode = args["test"]
	root = None
	progress = 0
	targetprogress = getSize(file)
	start = 0
	pr = None
	now = time.time()
	floatingBPS = 0
	lastProgress = 0
	if testMode:
		unittest.main(argv=[sys.argv[0]])
		input("Press Enter to leave")
		exit()
	else:
		if password is None:
			password = input("Enter password: ")
			if useCurses:
				window = curses.initscr()
				window.clear()
				window.refresh()
		if password is not None:
			if profiling:
				pr = cProfile.Profile()
				pr.enable()
			start = time.time()
			if encodeMode:
				archiver = Archiver(file, True)
				compressor = Compressor()
				encoder = Encoder(password)
				writebuffer = WriteBuffer(file+".edoc")
				while True:
					printProgress()
					breakcondition = False
					data = archiver.read()
					datalen = len(data)
					progress += datalen
					if datalen == 0:
						breakcondition = True
					data = compressor.compress(data)
*/	
	return 0;
}
