#include"gestor.h"

int gest(SETOPTS settings);
int checkCMD();
int getvars(SETOPTS* set);

//*MAIN DUH

int main(int argc, char *argv[], char *envp[])
{
    SETOPTS set;

    printf("Ola, sou o Gestor.\n");
    getvars(&set);
    gest(set);
    exit(0);
}

//*FUNCTION TO GET THE ENV VARS TO A STRUCT

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
        set->max_msgs = atoi(getenv("MAXMSG"));
    
    return 0;

}

int gest(SETOPTS settings)
{
    int pc[2], cp[2], nB, nHits, cont = 0, res, verifP, perro, estado, stop = 0;
    char pal[TAM_PAL], hits[5], *buf;

    perro = pipe(pc);
    if(perro == -1)
    {
        printf("[ERRO]Nao conseguiu abrir o pipe [pc]");
        exit(0);
    }

    perro = pipe(cp);/* code */
    if(perro == -1)
    {
        printf("[ERRO]Nao conseguiu abrir o pipe [pp]");
        exit(0);
    }
    
    verifP = fork();

    if (verifP == 0)
    {
        dup2(pc[0], 0);
        close(pc[0]);       //*MAKES CHILD READ FROM PARENT
        close(pc[1]);

        dup2(cp[1], 1);
        close(cp[0]);       //*MAKE CHILD WRITE TO PARENT
        close(cp[1]);

        if (execl("verif", "verif", settings.bwords , NULL) == -1){
            printf("Erro a exec verif!");
            exit(1);
        }
    }
    else
    {
        close(pc[0]);   //*PARA NAO LER DO CHILD POR ESTE PIPE
        close(cp[1]);   //*PARA NAO ESCREVER NO CHILD POR ESTE PIPE
        do
        {
            res = checkCMD();

            switch (res)
            {
                case 0 :
                    close(pc[1]);
                    close(cp[0]);
                    kill(verifP, SIGUSR2);
                    exit(0);
                case 1 :
                    if(cont == settings.max_msgs)
                    {
                        printf("Max Msgs reached!\n");
                        break;
                    }
                    cont++;
                    printf("Por favor introduza as palavras: ");
                    fflush(stdout);

                    do
                    {
                        
                        fgets(pal, TAM_PAL, stdin);
                        write(pc[1], pal, strlen(pal));

                        
                    }while((strcmp(pal, "##MSGEND##\n")) != 0 || stop == 1);
                    nB = read(cp[0], hits, sizeof(hits));
                    hits[nB - 1] = '\0';
                    nHits = atoi(hits);

                    printf("\nPalavras Proibidas = %d\n", nHits);
                    fflush(stdout);
                    if(nHits >= settings.max_bwords)
                    {
                        printf("Msg ignored, max banned words reached!\n");
                    }
                    break;
                case -1 :
                    printf("Work in progress!\n");
                    break;
                case -2 :
                    printf("Missing options!\n");
                    break;
                default:
                    printf("ERROR Write valid command or option!\n");
                    break;
            }
        } while (1);
    }
    
    
}

int checkCMD()
{   
    char cmd[15];
    char* command;
    char* option;
    

    do{
        printf("Comando: ");
        fgets(cmd, 15, stdin);

        command = strtok(cmd, " ");     //*WILL DIVIDE THE STRING OF COMMAND IN WORDS TO CHECK THE COMMAND AND ARGUMENTS
        option = strtok(NULL, " ");     //*ITS DIVIDED BY THE SPACE BUT CAN BE DIVIDED BY ANYTHING, CHECK STRTOK()

        if(option == NULL)
            cmd[strlen(cmd)] = '\0';    //*MAKES ACCEPTABLE COMMANDS WITHOUT ARGUMENTS LIKE SHUTDOWN


        if((strcmp(command, "verifica\n")) == 0)
        {
            printf("A chamar o verificador\n");
            return 1;
        }

        if((strcmp(command, "shutdown\n")) == 0)
        {
            printf("Vou terminar!\n");
            return 0;;
        }
        //TODO: ------------------------META 2----------------------
        if((strcmp(command, "filter")) == 0)
        {
            if((strcmp(option, "on\n")) == 0)
            {
                return -1;
            }
            
            if((strcmp(option, "off\n")) == 0)
            {
                return -1;
            }
        }
        if((strcmp(command, "filter\n")) == 0)
        {
            return -2;
        }

        if((strcmp(command, "users\n")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "topics\n")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "msg\n")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "topic")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "topic\n")) == 0)
        {
            return -2;
        }

        if((strcmp(command, "del")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "del\n")) == 0)
        {
            return -2;
        }

        if((strcmp(command, "kick")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "kick\n")) == 0)
        {
            return -2;
        }

        if((strcmp(command, "prune")) == 0)
        {
            return -1;
        }

        if((strcmp(command, "prune\n")) == 0)
        {
            return -2;
        }

        else
        {
            return -3;
        }
        

    }while(1);
}