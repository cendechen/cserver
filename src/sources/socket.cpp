#include "socket.h"
using namespace std;

Socket::Socket(string host, int port) {
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in serAddr;
  memset(&serAddr, '/0', sizeof(serAddr));

  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(port);
  inet_aton(host.c_str(), &serAddr.sin_addr.s_addr);

  if(bind(fd, (struct sockaddr *) &serAddr, sizeof(struct sockaddr_in)) == -1) {
    cerr << "bind socket on " << host << ":" << port << endl;
    exit(1);
  }
  if (listen(fd, 1000) == -1) {
    cerr << "listen socket error" << endl;
    exit(1);
  }
}

void Socket::registerCallback (f fn) {
  callback = fn;
}
// 开启监听和异常处理
void Socket::start() {
  while(true) {
    struct sockaddr_in clientIp;
    int clientIpSize = sizeof(clientIp);
    int con;
    if ((con = accept(fd, (struct sockaddr *) & clientIp, &clientIpSize)) == -1) {
      cerr << "accept client error" << endl;
    }
    if (con) {
      callback(con);
    }
  }
}
Socket::~Socket(){
  close(fd);
}
