#include"gestor.h"

int getvars(SETOPTS* set);
//int gest(SETOPTS* set);
void* checkUsr(void* dados);
void* checkMSG(void* dados);

int main(int argc, char* argv[])
{
    int fd;
    char  str[30], *command, *option;
    pthread_t cUSR, cMSG;

    SETOPTS set;
    getvars(&set);

    USERNAMES totalUsers[20];

    MSG tMSGS[set.max_msgs];
    CHECK_MSG data;
    
    data.settings = &set;
    
    char zero[3] = "-1";
    for(int i = 0; i < 20; i++)
    {
        strcpy(totalUsers[i].nome, zero);
    }

    data.totalMSGS = malloc(sizeof(tMSGS) * data.settings->max_msgs);
    data.totalMSGS = tMSGS;
    mkfifo(USERS_FIFO, 0600);
    mkfifo(MSG_FIFO, 0600);
pthread_mutex_init(&mut, NULL);
    pthread_create(&cUSR, NULL, checkUsr, (void*) &totalUsers);
    pthread_create(&cMSG, NULL, checkMSG, (void*) &data);
    do
    {
        printf("comando: ");
        fflush(stdout);
        fgets(str, 30, stdin);
        command = strtok(str, " ");
        option = strtok(NULL, " ");
        if(option == NULL)
            str[strlen(str)] = '\0';

        if(strcmp(command, "users\n") == 0)
        {
            for(int i = 0; i < 20; i++)
            {
                if(strcmp(totalUsers[i].nome, "-1") != 0)
                {
                    printf("%s", totalUsers[i].nome);
                    fflush(stdout);
                }
            }
        }
        if(strcmp(command, "msgs\n") == 0)
        {
            for(int i = 0; i < set.max_msgs; i++)
            {
                if(strcmp(data.totalMSGS[i].text, "\0") != 0)
                {
                    printf("\n%s\n", data.totalMSGS[i].text);
                    fflush(stdout);
                }
            }
        }
        if(strcmp(command, "topics\n") == 0)
        {
            for(int i = 0; i < set.max_msgs; i++)
            {
                if(strcmp(data.totalMSGS[i].topic, "\0") != 0)
                {
                    printf("%s\n", data.totalMSGS[i].topic);
                    fflush(stdout);
                }
            }
        }
    } while(strcmp(command, "shutdown\n") != 0);
    //for(int i = 0; i < 20; i++)

    unlink(USERS_FIFO);
    unlink(MSG_FIFO);
    return 0;
}


int getvars(SETOPTS* set)
{
    char* buf;
 
    buf = getenv("WORDSNOT");
    if (buf!=NULL)
        strcpy(set->bwords, buf);


    buf = getenv("MAXNOT");
    if (buf != NULL)
        set->max_bwords = atoi(buf);
    
    buf != getenv("MAXMSG");
    if (buf != NULL)
        set->max_msgs = atoi(buf);
    
    return 0;

}

void* checkUsr(void* dados)
{
    USERNAMES data;
    USERNAMES* tUsr;
    tUsr = (USERNAMES*) dados;
    int cf, gf, n, cont, existe, livre, i;
    char username[50];

    gf = open(USERS_FIFO, O_RDWR);

    do
    {
        n = read(gf, &data, sizeof(USERNAMES));

        if(n == sizeof(USERNAMES))
        {
            if(data.io == 0)
            {
                //pthread_mutex_lock(&mut);
                cont = 0;
                
                existe = -1;
                livre = -1;

                strcpy(username, data.nome);
                
                for(i = 0; i < 20; i++)
                {
                    existe = -1;
                    if(strcmp(username, tUsr[i].nome) == 0)
                    {
                        existe = 1;
                        sprintf(username, "%s%d", data.nome, ++cont);
                    }
                    if(livre == -1 && strcmp(tUsr[i].nome, "-1") == 0)
                        livre = i;
                }
                if(existe == -1 && livre != -1)
                {
                    strcpy(data.nome, username);
                    strcpy(tUsr[livre].nome, username);
                    printf("\n%s logged in\nComando: ", username);
                    fflush(stdout);
                };
                cf = open(data.pid, O_WRONLY);
                n = write(cf, &data, sizeof(USERNAMES));
                close(cf);
                //pthread_mutex_unlock(&mut);
            }
            if(data.io == -1)
            {
                for(i = 0; i < 20; i++)
                {
                    if(strcmp(data.nome, tUsr[i].nome) == 0)
                    {
                        strcpy(tUsr[i].nome, "-1");
                        strcpy(tUsr[i].pid, "-1");
                    }
                }
            }
        }
    } while (1);
    
    close(gf);
    pthread_exit(NULL);
}

void* checkMSG(void* dados)
{
    MSG msg;
    CHECK_MSG* cMSG;
    cMSG = (CHECK_MSG*) dados;
    int cf, gf, n, livre = -1, i;

    gf = open(MSG_FIFO, O_RDWR);

    do
    {
        n = read(gf, &msg, sizeof(MSG));
        if(n == sizeof(MSG))
        {
            //pthread_mutex_lock(&mut);
            for(i = 0; i < cMSG->settings->max_msgs; i++)
            {
                if(livre == -1 && strcmp(cMSG->totalMSGS[i].topic, "\0"))
                    livre = i;
            }
            if(livre != -1)
            {
                strcpy(cMSG->totalMSGS[livre].topic, msg.topic);
                strcpy(cMSG->totalMSGS[livre].title, msg.title);
                strcpy(cMSG->totalMSGS[livre].text, msg.text);
                strcpy(cMSG->totalMSGS[livre].autor, msg.autor);
                cMSG->totalMSGS[livre].lifetime = msg.lifetime;
                printf("\nMSG received\nComando: ");
                fflush(stdout);
            }
            
        //pthread_mutex_unlock(&mut);
        }
    } while (1);
    close(gf);
}