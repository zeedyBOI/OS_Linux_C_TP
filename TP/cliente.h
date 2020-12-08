#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>

#define MAX_TOP 15
#define MAX_TITLE 30
#define MAX_TEXT 1000

typedef struct
{
    char topic[MAX_TOP];
    char title[MAX_TITLE];
    char text[MAX_TEXT];
    int lifetime;
} MSG;
