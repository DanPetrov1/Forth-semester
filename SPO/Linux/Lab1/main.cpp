
#include "ncurses.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>

int main()
{
    initscr();
    int child_pid;
    int status;
    if((child_pid = fork()) == -1)
    {
        printw("process creation error");
        exit(1);
    }
    else
    {
        if(child_pid == 0)
        {
            usleep(1000000);
            printw("this is child\n");
            time_t seconds = time(NULL);
            tm* timeinfo = localtime(&seconds);
            printw("Child ID %d\n", getpid());
            printw("Parent ID %d\n", getppid());
            printw("Current Datetime: %s\n", asctime(timeinfo));
        }
        else
        {
            printw("this is parent\n");
            time_t seconds = time(NULL);
            tm* timeinfo = localtime(&seconds);
            printw("Current Datetime: %s\n", asctime(timeinfo));
        }

        if(wait(&status) == child_pid)
        {
            printw("Ok %d\n", status);
            exit(child_pid);
        }
    }
    refresh();
    getch();
    endwin();
    return 0;
}