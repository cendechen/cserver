#ifndef EPOLL_HEAD
#define EPOLL_HEAD
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
int setnonblocking(int);
void addfd(int, int, bool);
#endif
