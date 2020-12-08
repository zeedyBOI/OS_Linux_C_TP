#include"cliente.h"

int checkusr(USERNAMES* username);
void writeMSG(USERNAMES* user);

int main(int argc, char* argv[])
{
    int n, nw, gf, cf, res, opt, u, option;
    USERNAMES user;
    char buf[40];
    fd_set fd_s;

    if (argc != 3)
    {
        printf("Sign In with a username!  EX: ./cliente -u edd\n");
        fflush(stdout);
        exit(-1);
    }

    if (access(USERS_FIFO, F_OK) != 0)
    {
        printf("[ERRO] Servidor nao existe!\n");
        fflush(stdout);
        exit(-2);
    }

    sprintf(user.pid, CLI_FIFO, getpid());
    mkfifo(user.pid, 0600);

    initscr();

    while((opt = getopt(argc, argv, "u:")) != -1)
    {
        switch (opt)
        {
            case 'u' :
                strcpy(user.nome, optarg);
                u = checkusr(&user);

                if (u == 0)
                {
                    printw("\tWelcome %s\n", user.nome);
                    refresh();
                }
                else
                {
                    printw("\tWelcome %s, your username was already being used so we changed it!\n", user.nome);
                    refresh();
                }
                break;
            default :
                printw("ERRO Argumentos Invalidos\n");
                endwin();
                exit(0);
        }
    }

    cf = open(user.pid, O_RDWR);
    if(cf < 0)

    {
        printw("[ERRO] Nao consegue abrir ficheiro\n");
        endwin();
        exit(3);
    }
    do
    {
        printw("\t###Menu###\n");
        printw("1 - Write Message\n");
        printw("2 - Show Topics\n");
        printw("0 - Exit\n");
        printw("Option: ");
        refresh();
        FD_ZERO(&fd_s);
        FD_SET(0, &fd_s);
        FD_SET(cf, &fd_s);
        res = select(cf + 1, &fd_s, NULL, NULL, NULL);

        if(res > 0 && FD_ISSET(0, &fd_s))
        {
            scanw("%d", &option);
            switch (option)
            {
            case 0:
                user.io = -1;
                gf = open(USERS_FIFO, O_WRONLY);
                write(gf, &user, sizeof(USERNAMES));
                close(gf);
                close(cf);
                unlink(user.pid);
                endwin();
                exit(0);
            case 1:
            erase();
                writeMSG(&user);
                break;
            default:
                printw("Option does not exist\n");
                refresh();
                break;
            }

        }

        if(res > 0 && FD_ISSET(cf, &fd_s))
        {
            gf = open(USERS_FIFO, O_WRONLY);
            if(gf < 0)
            {
                printw("[ERRO] Nao consegue abrir ficheiro");
                endwin();
                exit(3);
            }
            n = read(gf, &user, sizeof(USERNAMES));
            close(gf);
            if(n == sizeof(USERNAMES))
            {
                printw("\n%s\n%s\n", user.nome, user.pid);
                refresh();
            }
        }
    }while(1);

    close(cf);
    unlink(user.pid);
    exit(0);
}


//!A TRATAR DO LADO DO GESTOR PARA LER NUM ARRAY DE USERNAMES
//!E DEVOLVER SE O USERNAME

int checkusr(USERNAMES* username)
{
    USERNAMES new;
    int ds, dr, s_f, c_f;
    /* printf("%s%s", username->nome, username->pid);
    strcpy(cliente, username->pid); */

    s_f = open(USERS_FIFO, O_WRONLY);
    if (s_f < 0)
    {
        printw("[ERRO] Nao consegue abrir o FIFO do servidor\n");
        endwin();
        exit(-3);
    }

    c_f = open(username->pid, O_RDWR | O_CREAT | O_TRUNC);
    if (c_f < 0)
    {
        printw("[ERRO] Nao consegue abrir o FIFO do cliente\n");
        endwin();
        exit(-3);
    }

    username->io = 0;
    ds = write(s_f, username, sizeof(USERNAMES));
    printw("Checking Username...\n");
    refresh();
    dr = read(c_f, &new, sizeof(USERNAMES));
    printw("Done!\n");
    refresh();
    if (dr == sizeof(USERNAMES))
    {
        if(strcmp(username->nome, new.nome) == 0)
        {
            close(s_f);
            close(c_f);
            return 0;
        }
        else
        {
            strcpy(username->nome, new.nome);
            close(s_f);
            close(c_f);
            return 1;
        }
    }
}

void writeMSG(USERNAMES* user)
{
    int s_f, c_f, ds, dr, y, x;
    MSG msg;
    clear();
    getmaxyx(stdscr, y, x);
    WINDOW *win1, *win2;
    s_f = open(MSG_FIFO, O_WRONLY | O_CREAT | O_TRUNC);
    if (s_f < 0)
    {
        printw("[ERRO] Nao consegue abrir o FIFO do servidor\n");
        endwin();
        exit(-3);
    }

    c_f = open(user->pid, O_RDWR);
    if (c_f < 0)
    {
        printw("[ERRO] Nao consegue abrir o FIFO do cliente\n");
        endwin();
        exit(-3);
    }
    win1 = newwin(y/2 - 2, x - 2, 1, 1);
    win2 = newwin(y/2 - 2, x - 2, y/2 + 1, 1);
    wrefresh(win1);
    wrefresh(win2);
    mvwprintw(win1, 1, 1, "\nTopic: ");
    wrefresh(win1);
    wgetnstr(win1, msg.topic, 15);
    wprintw(win1, "\nTitle: ");
    wrefresh(win1);
    wgetnstr(win1, msg.title, 30);
    wprintw(win2, "\nMessage:\n");
    wrefresh(win2);
    wgetnstr(win2, msg.text, 1000);
    wprintw(win1, "\nLifetime: ");
    wrefresh(win1);
    wscanw(win1, "%d", &msg.lifetime);
    printw("\n%d: ", msg.lifetime);
    strcpy(msg.pid, user->pid);
    strcpy(msg.autor, user->nome);
    ds = write(s_f, &msg, sizeof(MSG));
    wprintw(win2, "escrevi %d\n", ds);
    wrefresh(win2);
    delwin(win1);
    delwin(win2);
    endwin();
}