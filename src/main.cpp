#include <iostream>
#include "socket/socket.h"
using namespace std;

int handle (int fd) {
  char buffer[1024];
  int cnt;
  cnt = read(fd, buffer, sizeof(buffer));
  cout << "read data cnt:" << cnt << " content:" << buffer << endl;
  const char *str = "hello world";
  send(fd, str, sizeof(str), 0);
  return 0;
}
int main() {
  Socket serve("127.0.0.1", 9990);
  serve.registerCallback(handle);
  serve.start();
  return 0;
}
