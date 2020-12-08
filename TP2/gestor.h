#include"includes.h"

#define TAM_PAL 1000

#define MAX_USERS 20

typedef struct 
{
    char bwords[40];
    int max_msgs;
    int max_bwords;
} SETOPTS;

typedef struct
{
    SETOPTS* settings;
    MSG* totalMSGS;
} CHECK_MSG;
