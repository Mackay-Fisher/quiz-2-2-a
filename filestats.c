#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

void flagConvertor(unsigned int octal_mode) {

}

int main(int argc, char *argv[])
{
    if (argc != 2)//Check if the number of arguments is correct
    {
        printf("usage: filestats directory_name\n");
        exit(1);
    }
    DIR *dir;//Get a dircetors pointer to be assigned to the directory we are opening
    dir = opendir(argv[1]);//This will open the directory based on the argument given to us in main where argv[1] is the directory name
    if (dir == NULL)//Check if it is possible to opne the diirectory
    {
        printf("can't open %s\n", argv[1]);//If not print an error message
        exit(1);
    }
    struct dirent * filepointers;//Create a pointer to the dirent structure
    filepointers = readdir(dir);//Read the directory
    

    /** Octal Conversion
    0 = ---
    1 = --x
    2 = -w-
    3 = -wx
    4 = r--
    5 = r-x
    6 = rw-
    7 = rwx
    */


    while (filepointers != NULL)//While the directory is not empty
    {
        char *filename = filepointers->d_name;//Get the file name
        char fullfile[2000];
        strcpy(fullfile, argv[1]);//Copy the directory name to the fullfile
        strcat(fullfile, filename);//Add a / to the end of the fullfile
        struct stat filestats;//Create a structure to hold the file stats
        stat(fullfile, &filestats);//Get the file stats and store them in the structure
        printf("File name: %s\n", filepointers->d_name);//Print the file name
        printf("inode number: %ld\n", filestats.st_ino);//Print the inode number
        printf("number of links %ld\n", filestats.st_nlink);//Print the number of links
        printf("User ID of owner: %d\n", filestats.st_uid);//Print the user ID of the owner
        printf("Group ID of owner: %d\n", filestats.st_gid);//Print the group ID of the owner
        printf("Size in bytes: %ld\n", filestats.st_size);//Print the file size in bytes
        printf("Last access: %s", ctime(&filestats.st_atime));//Print the last access time
        printf("Last modification: %s", ctime(&filestats.st_mtime));//Print the last modification time
        printf("Number of disk blocks allocated: %ld\n", filestats.st_blocks);//Print the number of disk blocks allocated
        printf("Access mode in octal: %o\n", filestats.st_mode);//Print the access mode in octal
        printf("Acess mode flags: %d\n\n", filestats.st_mode);//Print the access mode flags
        filepointers = readdir(dir);//Read the next file
    }
    
}