#include "socket.h"
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

  fd_set read_sets;
  fd_set copy_read_sets;
  FD_ZERO(&copy_read_sets);
  FD_SET(fd, &copy_read_sets);
  int maxfd = fd;


  while(true) {
    // select 需要监听的都留空
    read_sets = copy_read_sets;

    struct sockaddr_in clientIp;
    int clientIpSize = sizeof(clientIp);
    int con;
    int count;
    if((count = select(maxfd+1, &read_sets, NULL, NULL, NULL)) == -1) {
      cerr << "select error" << endl;
      exit(1);
    }
    cout<< "select sucess count: " << count << endl;

    for (int i = 0; i <= maxfd ; i++) {
      if (FD_ISSET(i, &read_sets)) {
        cout << "socket i: "<< i << endl;
        if (i == fd) {
          if ((con = accept(fd, (struct sockaddr *) & clientIp, (socklen_t *) &clientIpSize)) == -1) {
            cerr << "accept client error" << endl;
          }
          maxfd = con > maxfd ? con : maxfd;
          FD_SET(con, &copy_read_sets);
        } else {
          callback(i);
        }
      }
    }
  }
}
Socket::~Socket(){
  cout << "close" << endl;
  close(fd);
}
