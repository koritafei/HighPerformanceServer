#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  // 1. 创建一个侦听的socket
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == listenfd) {
    std::cout << "create listen socket error!" << std::endl;
    return -1;
  }

  // 2. 初始化服务器地址
  struct sockaddr_in bindaddr;
  bindaddr.sin_family      = AF_INET;
  bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  bindaddr.sin_port        = htons(3000);

  if (-1 == (bind(listenfd,
                  reinterpret_cast<struct sockaddr *>(&bindaddr),
                  sizeof(struct sockaddr)))) {
    std::cout << "bind socket error." << std::endl;
    return -1;
  }

  // 3. 启动侦听
  if (-1 == listen(listenfd, SOMAXCONN)) {
    std::cout << "listen socket error." << std::endl;
    return -1;
  }

  // 记录所有客户端连接的容器
  std::vector<int> clientfds;
  while (true) {
    struct sockaddr_in clientaddr;
    socklen_t          clientaddrlen = sizeof(struct sockaddr_in);

    // 4.接收客户端连接
    int clientfd = accept(listenfd,
                          reinterpret_cast<struct sockaddr *>(&clientaddr),
                          &clientaddrlen);

    if (-1 != clientfd) {
      char recvBuf[32] = {0};
      // 5. 从客户端接收数据
      ssize_t ret = recv(clientfd, recvBuf, 32, 0);
      if (0 < ret) {
        std::cout << "recv data from client, data : " << recvBuf << std::endl;
        // 6. 将收到数据回射给客户端
        ret = send(clientfd, recvBuf, strlen(recvBuf), 0);

        if (static_cast<size_t>(ret) != strlen(recvBuf)) {
          std::cout << "send data error. " << std::endl;
        } else {
          std::cout << "send data to client sucessly. data: " << recvBuf
                    << std::endl;
        }
      } else {
        std::cout << "recv data error." << std::endl;
      }

      clientfds.push_back(clientfd);
    }
  }

  // 7. 关闭侦听socket
  close(listenfd);

  return 0;
}