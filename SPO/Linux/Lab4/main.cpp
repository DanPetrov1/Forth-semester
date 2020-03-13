#include <iostream>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <pthread.h>
#include <sstream>
#include <list>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

using namespace std;

unsigned int last_thread = 0;

void * threadStartRoutine(void* arg)
{
    pthread_mutex_t mutex = *((pthread_mutex_t*)(arg));
    while(true)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if(last_thread == (unsigned int)pthread_self())
            sleep(1);
        pthread_mutex_lock(&mutex);

        cout << "Hello! I'm subthread " << (unsigned int)pthread_self() << endl;

        last_thread = (unsigned int)pthread_self();

        pthread_mutex_unlock(&mutex);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
    }
}

int main(int argc, char** argv)
{
    char symbol;
    int sigReciever;
    int status;
    void **threadReceiver;
    list<pthread_t> threadList;
    pthread_mutex_t mutex;
    pthread_t newThread;

    sigset_t waitSet;
    sigemptyset(&waitSet);
    sigaddset(&waitSet, SIGCONT);
    sigprocmask(SIG_BLOCK, &waitSet, NULL);

    void *(*start_routine) (void *) = threadStartRoutine;
    int mutexError = pthread_mutex_init(&mutex, NULL);
    if(mutexError != 0)
    {
        cout << "Error while creating mutex (code): " << mutexError << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    cout << "Hello! This is the parent thread." << endl;
    cout << "If you want to create new thread, please press '+'" << endl;
    cout << "If you want to delete last thread, please press '-'" << endl;
    cout << "If you want to quit, please press 'q'" << endl;
    while(true)
    {
        rewind(stdin);
        cin.get(symbol);
        switch(symbol)
        {
            case '+': {
                status = pthread_create(&newThread, NULL, start_routine, &mutex);
                if (status == 0) {
                    threadList.push_back(newThread);
                } else {
                    cout << "Erorr while creating thread!" << endl << endl;
                    if (!threadList.empty()) {
                        for (auto &threadId : threadList) {
                            pthread_cancel(threadId);
                            pthread_join(threadId, threadReceiver);
                        }
                        threadList.clear();
                    }
                    pthread_mutex_destroy(&mutex);
                    exit(ERROR_CREATE_THREAD);
                }
                break;
            }
            case '-': {
                if (threadList.empty()) {
                    cout << "There are no threads to delete!" << endl;
                } else {
                    pthread_cancel(threadList.back());
                    pthread_join(threadList.back(), threadReceiver);
                    threadList.pop_back();
                }
                break;
            }
            case 'q': {
                while (!threadList.empty()) {
                    pthread_cancel(threadList.back());
                    pthread_join(threadList.back(), threadReceiver);
                    threadList.pop_back();
                }
                threadList.clear();
                pthread_mutex_destroy(&mutex);
                return 0;
            }
            default:
                break;
        }
    }
}
