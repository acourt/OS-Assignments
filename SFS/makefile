sfs_test: sfs_test.o sfs_api.o disk_emu.o
	gcc -Wall -o sfs_test sfs_test.o sfs_api.o disk_emu.o 

sfs_test.o: sfs_test.c sfs_api.h
	gcc -Wall -c sfs_test.c

sfs_api.o: SFS.c sfs_api.h disk_emu.h
	gcc -Wall -c SFS.c -o sfs_api.o

disk_emu.o: disk_emu.c disk_emu.h
	gcc -Wall -c disk_emu.c