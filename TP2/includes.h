#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>
#include<pthread.h>
#include<curses.h>

#define USERS_FIFO "USERS"
#define MSG_FIFO "MSGS"
#define CLI_FIFO "CLI%d"
#define MAX_TOP 15
#define MAX_TITLE 30
#define MAX_TEXT 1000

typedef struct
{
    char nome[30];
    char pid[30];
    int io;
} USERNAMES;

typedef struct
{
    char topic[MAX_TOP];
    char title[MAX_TITLE];
    char text[MAX_TEXT];
    int lifetime;
    char autor[30];
    char pid[30];
} MSG;

pthread_mutex_t mut;