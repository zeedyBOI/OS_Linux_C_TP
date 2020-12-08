#include"cliente.h"

int main(int argc, char* argv[])
{
    int opt;

    if (argc != 3)
    {
        printf("Sign In with a username!  EX: ./cliente -u edd\n");
        exit(0);
    }

    while((opt = getopt(argc, argv, "u:")) != -1)
    {
        switch (opt)
        {
            case 'u' :
                printf("Ola %s, sou o Cliente\n", optarg);
                break;
            default :
                printf("ERRO Argumentos Invalidos\n");
                exit(0);
        }
    }
    MSG message;

    printf("Topico: ");
    fgets(message.topic, MAX_TOP, stdin);
    printf("Titulo: ");
    fgets(message.title, MAX_TITLE, stdin);
    printf("Corpo: ");
    fgets(message.text, MAX_TEXT, stdin);
    printf("Lifetime: ");
    scanf("%d", &message.lifetime);

    exit(0);
}