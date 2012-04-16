#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfs_api.h"

/* The maximum file name length. We assume that filenames can contain
 * upper-case letters and periods ('.') characters. Feel free to
 * change this if your implementation differs.
 */
#define MAX_FNAME_LENGTH 12   /* Assume at most 12 characters (8.3) */

/* The maximum number of files to attempt to open or create.  NOTE: we
 * do not _require_ that you support this many files. This is just to
 * test the behavior of your code.
 */
#define MAX_FD 100 

/* The maximum number of bytes we'll try to write to a file. If you
 * support much shorter or larger files for some reason, feel free to
 * reduce this value.
 */
#define MAX_BYTES 30000 /* Maximum file size I'll try to create */
#define MIN_BYTES 10000         /* Minimum file size */

/* Just a random test string.
*/
static char test_str[] = "A mathematician is a machine for turning coffee into theorems.\n";

/* rand_name() - return a randomly-generated, but legal, file name.
 *
 * This function creates a filename of the form xxxxxxxx.xxx, where
 * each 'x' is a random upper-case letter (A-Z). Feel free to modify
 * this function if your implementation requires shorter filenames, or
 * supports longer or different file name conventions.
 * 
 * The return value is a pointer to the new string, which may be
 * released by a call to free() when you are done using the string.
 */

int main()
{
	mksfs(1);
	int chunksize = 10, i;
	char testbuf[20], buffer[5000];
	int fd = sfs_fopen("My doc");
	for (i = 0; i < chunksize; i++) {
        buffer[i] = (char) i;
		//printf("%d: Printing character: %c\n",i, buffer[i]);
    }
	printf("buffer: %c\n", buffer[1]);
    sfs_fwrite(fd, buffer, chunksize);
	
	sfs_fseek(fd, 0);
	sfs_fread(fd, testbuf, 10);
	
	printf("Retrieved info: %c\n", testbuf[2]);
	sfs_ls();
	return 0;
}












