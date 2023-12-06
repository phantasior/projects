/*
	How archive looks like:
	--STAMP--
	[contains:
		size of coding block,
		number of files in archive,
		check = 13 (to check if it is my type of archive)	
	], Stamp encodes with hamming code where size of coding block equals to sizeof(Stamp)
	--FILE_HEADER--
	[contains:
		size of file,
		length of file name
	]
	--NAME_OF_FILE--
	--CONTENT_OF_FILE--
	--FILE_HEADER--
	and so on

	example of usage:
		create archive: ./archiver --file=archive.haf -c FILE1 FILE2 FILE3 -b 11
		extract archive: ./archiver -f archive.haf -x
*/

#include "lib/arc.h"
#include "lib/parser.h"
#include "lib/hamming.h"

int main(int argc, char** argv) {
	Option option = Parse(argc, argv);

	if (option.create) {
		Create(option.archive_name, option.files_list, option.block_size);
	} else if (option.list) {
		List(option.archive_name);
	} else if (option.extract) {
		Extract(option.archive_name, option.files_list);
	} else if (option.append) {
		Append(option.archive_name, option.files_list);
	} else if (option.del) {
		Del(option.archive_name, option.files_list);
	} else if (option.concat) {
		Concat(option.archive_name, option.files_list);
	}
}
