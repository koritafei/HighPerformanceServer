#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv) {
  // 创建一个侦听socket
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == listenfd) {
    std::cout << "create socket error!" << std::endl;
    return -1;
  }

  // 初始化服务器地址
  struct sockaddr_in serveraddr;
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port        = htons(3000);

  if (-1 == bind(listenfd,
                 reinterpret_cast<struct sockaddr *>(&serveraddr),
                 sizeof(serveraddr))) {
    std::cout << "bind socket error!" << std::endl;
    close(listenfd);
    return -1;
  }

  // 启动侦听
  if (-1 == listen(listenfd, SOMAXCONN)) {
    std::cout << "listen error" << std::endl;
    close(listenfd);
    return -1;
  }

  while (true) {
    struct sockaddr_in clientaddr;
    socklen_t          socklen = sizeof(clientaddr);

    // 接受客户端连接
    int clientfd = accept(listenfd,
                          reinterpret_cast<struct sockaddr *>(&clientaddr),
                          &socklen);
    if (-1 != clientfd) {
      std::cout << "accept a client connection, clientfd: " << clientfd
                << std::endl;
    }
  }

  close(listenfd);

  return 0;
}
