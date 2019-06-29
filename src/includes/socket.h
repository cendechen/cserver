#ifndef HEAD_SOCKET
#define HEAD_SOCKET
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
// 定义回调函数类型

typedef int (*f)(int);

class Socket {
  public:
    Socket(string host, int port);
    ~Socket();
    void registerCallback(f);
    void start(); // 开始接受请求
  private:
    int fd;
    f callback;
};
#endif
