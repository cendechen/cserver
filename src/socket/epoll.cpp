#include "epoll.h"


int setnonblocking( int fd) {
  int oldOption = fcntl(fd, F_GETFL);
  int newOption = oldOption | O_NONBLOCK;
  fcntl(fd, F_SETFL, newOption);
  return oldOption;
}

void addfd(int epollfd, int fd, bool enable_et) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN;

  if (enable_et) {
    event.events |= EPOLLET;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}
