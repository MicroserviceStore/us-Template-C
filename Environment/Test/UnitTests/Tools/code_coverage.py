import sys
import re

filename = sys.argv[1]

print('')

files_to_check = {
	'Environment/Test/UnitTests/us_operation_source.c',
	'Include/AIGenerated/operation_func.inc'
}

non_blocker_files = {

}

is_valid = 1
with open(filename) as fp:
	line = fp.readline()
	while line:
		if "File '" in line:
			file_name = line[6:]
			file_name = file_name[:len(file_name)-2]
			if file_name in files_to_check:
				line = fp.readline()
				if line:
					coverage_text = line[len('Lines executed:'):]
					coverage_text = coverage_text[:len(coverage_text)-1]
					if '100.00%' not in coverage_text:
						#print(file_name.ljust(70, ' ') + ' : ' + '\033[93m' + coverage_text + '\033[0m')
						print('X'.ljust(5, ' ') + file_name.ljust(70, ' ') + ' : ' + coverage_text)
						if file_name not in non_blocker_files:
							is_valid = 0
					else:
						print(' '.ljust(5, ' ') + file_name.ljust(70, ' ') + ' : ' + coverage_text)
		if line:
			line = fp.readline()

print('')

if not is_valid:
	sys.exit(1)

#sys.exit(0)
