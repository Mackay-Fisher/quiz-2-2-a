#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
//I wrote my own int power fucntion because i got stuborn and decided to that I wanted to write this fucntion in a very backwards way.
int mypow(int base, int exp) {
    int result = 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}

void octalToFlags(unsigned int octal_mode, char * flags) {
    //In a six digit octal number the first three digits are related to the [SETUID][SETGID][STICKY] while the last three are the permessions assocated with the [USER][GROUP][OTHERS]
    //We can find each of these value by chekcing if the values and thier directory.
    char * projections[]={//This is check against the basis connections of the function
        "---",//0
        "--x",//1
        "-w-",//2
        "-wx",//3
        "r--",//4
        "r-x",//5
        "rw-",//6
        "rwx"//7
    };
    int arr[6] = {0};
    int i = 0;
    int val = 0;
    while (octal_mode)
    {
        arr[i] =octal_mode%8;
        octal_mode = octal_mode/8;
        i++;
    }
    i = 0;
    for (int j = 0; j < 6; j++)
    {
        val += arr[j]*mypow(10, j);
    }
    octal_mode = val;
    int setuid = octal_mode/100000;//gets the left most digit
    octal_mode = octal_mode%100000;//then removes the left most digit
    int setgid = octal_mode/10000;//gets the next left most digit
    octal_mode = octal_mode%10000;//removes the next left most digit
    int sticky = octal_mode/1000;//gets the next left most digit
    octal_mode = octal_mode%1000;//removes the next left most digit
    int user = octal_mode/100;//gets the next left most digit this one being importnat to the actual permessions
    octal_mode = octal_mode%100;//removes the next left most digit
    flags[0] = projections[user][0];//sets the first flag to the first value of the user
    flags[1] = projections[user][1];//sets the second flag to the second value of the user
    flags[2] = projections[user][2];//sets the third flag to the third value of the user
    int group = octal_mode/10;//gets the next left most digit this one being importnat to the actual permessions
    flags[3] = projections[group][0];//sets the first flag to the first value of the group
    flags[4] = projections[group][1];//sets the second flag to the second value of the group
    flags[5] = projections[group][2];//sets the third flag to the third value of the group
    octal_mode = octal_mode%10;//removes the next left most digit
    int other = octal_mode;//gets the next left most digit this one being importnat to the actual permessions
    flags[6] = projections[other][0];//sets the first flag to the first value of the other
    flags[7] = projections[other][1];//sets the second flag to the second value of the other
    flags[8] = projections[other][2];//sets the third flag to the third value of the other
    //Not sure if we should implemnt sticky and setgid becaus ethe output etxt does not chnage 40775 from rwxrwxr-x to rwxrwsr-x
    //So i left nothing to do with the sticky and setgid and only focus on the type form the start
    
    //I see that the output text has nothing for directory or not directory so i will not implement that and instaed only focus on the user group and other
    /*flags[0] = setuid == 1 && filedir ? 'd' : '-';
    flags[6] = (flags[6] == 'x' && setgid) ? 's' : 'S';
    flags[9] = (flags[9] == 'x' && sticky) ? 't' : 'T';*/
    //These would be the flags I would check if I instead extedned the buffer to 10 and added the type check of - or d at the start then I would check thoese flags for the s and t
    //But because the example only shows the 9 associated with the last three digits that all I implmenetd as well :)
    //Now that we have the values we can convert them to the flags
    //Now we can convert the values to the flags 
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
        char flags[10];//Create a buffer to hold the flags
        octalToFlags(filestats.st_mode, flags);//Get the flags
        printf("Access mode flags: %s\n\n", flags);//Print the access mode flags
        filepointers = readdir(dir);//Read the next file
    }
    
}