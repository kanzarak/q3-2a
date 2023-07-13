#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // todo
    struct stat filestats;
    DIR* dp;
    struct dirent *dirp;

    char* accesses[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

    if (argc != 2) {
        fprintf(stderr, "usage: filestats directory_name\n");
        exit(-1);
    }

    if ((dp = opendir(argv[1])) == NULL) {
        fprintf(stderr, "can’t open %s\n", argv[1]);
        exit(-1);
    }

    //Read content
    char fullpath[256];
    chdir(argv[1]); // cd to specified directory
    while ((dirp = readdir(dp)) != NULL) {
        //Print all stats about file
        realpath(dirp->d_name, fullpath);

        int ret = lstat((const char*)fullpath, &filestats);//stat needs absolute path
        if(ret < 0) {
            fprintf(stderr, "File stat error\n");
            continue;
        }

        printf("File name: %s\n", dirp->d_name);
        printf("inode number: %lu\n", filestats.st_ino);
        printf("number of links: %ld\n", filestats.st_nlink);
        printf("User ID of owner: %d\n", filestats.st_uid);
        printf("Group ID of owner: %d\n", filestats.st_gid);
        printf("Size in bytes: %d bytes\n", (int)filestats.st_size);

        printf("Last access: %s", ctime(&filestats.st_atime));
        printf("Last modification: %s", ctime(&filestats.st_mtime));
        printf("Last status change: %s", ctime(&filestats.st_ctime));
        printf("Number of disk blocks allocated: %ld\n", filestats.st_blocks);

        printf("Access mode in octal: %o\n", filestats.st_mode);// octal
        printf("Access mode flags: ");
        for(int i = 6; i >= 0; i -= 3){
            printf("%s", accesses[(filestats.st_mode >> i) & 7]);
        }

        printf("\n\n");
    }
}