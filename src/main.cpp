#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#include "processPool/process_pool.h"
#include "socket/epoll.h"
#include "processPool/http.h"

using namespace std;

int main(int argc, char ** argv) {
  if (argc <= 2) {
    cout << "usage: "<< basename(argv[0]) << "ip port" << endl;
    return 1;
  }
  const char* ip = argv[1];
  int port = atoi(argv[2]);

  int listenfd = socket(PF_INET, SOCK_STREAM, 0);
  assert(listenfd > 0);
  int ret = 0;
  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  ret = bind(listenfd, (struct sockaddr *)& address, sizeof(address));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  processPool<cgi_conn> * pool = processPool<cgi_conn>::create(listenfd);
  if (pool) {
    pool->run();
    delete pool;
  }
  close(listenfd);
  return 0;
}
