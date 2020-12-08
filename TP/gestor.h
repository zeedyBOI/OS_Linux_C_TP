#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>

#define TAM_PAL 1000

typedef struct 
{
    char bwords[40];
    int max_msgs;
    int max_bwords;
} SETOPTS;
