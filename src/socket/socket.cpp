#include "socket.h"
#include "epoll.h"
#define MAX_EVENT_NUMBER 1024
using namespace std;

Socket::Socket(string host, int port) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serAddr;
  memset(&serAddr, '\0', sizeof(serAddr));

  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(port);
  inet_aton(host.c_str(), (struct in_addr *)&serAddr.sin_addr.s_addr);

  if(bind(fd, (struct sockaddr *) &serAddr, sizeof(serAddr)) == -1) {
    cerr << "bind socket on " << host << ":" << port << endl;
    exit(1);
  }
  if (listen(fd, 5) == -1) {
    cerr << "listen socket error" << endl;
    cerr << errno <<" " << strerror(errno) << endl;
    exit(1);
  }
}

void Socket::registerCallback (f fn) {
  callback = fn;
}

// 开启监听和异常处理
void Socket::start() {

  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  addfd(epollfd, fd, true);

  while(true) {
    // select 需要监听的都留空
    int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if (ret < 0) {
      cerr << "epoll_wait error" << endl;
      exit(-1);
    }

    ltCb(events, ret, epollfd);
  }
  close(epollfd);
}

void Socket::ltCb(epoll_event *events, int number, int epollfd) {
  for(int i = 0; i< number; i++) {
    int sockfd = events[i].data.fd;
    if (sockfd == fd) {
      struct sockaddr_in clientIp;
      socklen_t clientIpSize = sizeof(clientIp);
      int con;
      con = accept(fd, (struct sockaddr *)&clientIp, &clientIpSize);
      addfd(epollfd, con, false);
    } else if(events[i].events & EPOLLIN) {
      callback(sockfd);
    } else {
      cout << "some thing has happen" << endl;
    }
  }
}
void Socket::etCb(epoll_event *events, int number, int epollfd) {
  for(int i = 0; i< number; i++) {
    int sockfd = events[i].data.fd;
    if (sockfd == fd) {
      struct sockaddr_in clientIp;
      socklen_t clientIpSize = sizeof(clientIp);
      int con;
      con = accept(fd, (struct sockaddr *)&clientIp, &clientIpSize);
      addfd(epollfd, con, true);
    } else if(events[i].events & EPOLLIN) {
      callback(sockfd);
    } else {
      cout << "some thing has happen" << endl;
    }
  }
}
Socket::~Socket(){
  cout << "close" << endl;
  close(fd);
}
