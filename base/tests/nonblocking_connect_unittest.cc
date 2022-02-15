#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT    3000
#define SEND_DATA      "Hello World!"

int main(int argc, char **argv) {
  // 1. 创建一个socket
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == clientfd) {
    std::cout << "create socket error." << std::endl;
    return -1;
  }

  // 创建成功后设置clientfd为非阻塞，如果创建时设置会影响connect的行为
  int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
  int newSocketFlag = oldSocketFlag | O_NONBLOCK;

  if (-1 == fcntl(clientfd, F_SETFL, newSocketFlag)) {
    close(clientfd);
    std::cout << "set socket to nonblock error." << std::endl;
    return -1;
  }

  // 2. 链接服务器
  struct sockaddr_in server_addr;
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
  server_addr.sin_port        = htons(SERVER_PORT);

  for (;;) {
    int ret = connect(clientfd,
                      reinterpret_cast<struct sockaddr *>(&server_addr),
                      sizeof(struct sockaddr));
    if (0 == ret) {
      std::cout << "connect to server sucessly." << std::endl;
      close(clientfd);
      return 0;
    } else if (-1 == ret) {
      if (EINTR == errno) {
        // connect 被信号中断，重试connect
        std::cout << "connecting interruptted by signal, try again."
                  << std::endl;
        continue;
      } else if (EINPROGRESS == errno) {
        // 连接建立中
        break;
      } else {
        close(clientfd);
        return -1;  // 出错
      }
    }
  }

  fd_set writeset;
  FD_ZERO(&writeset);
  FD_SET(clientfd, &writeset);

  // 可以利用tv_time和tv_utime做精细控制
  struct timeval tv;
  tv.tv_sec  = 3;
  tv.tv_usec = 0;

  if (1 != select(clientfd + 1, NULL, &writeset, NULL, &tv)) {
    std::cout << "[select] connect to server error." << std::endl;
    close(clientfd);
    return -1;
  }

  int       err;
  socklen_t len = static_cast<socklen_t>(sizeof err);
  if (0 > ::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len)) {
    close(clientfd);
    return -1;
  }

  if (0 == err) {
    std::cout << "connect to server successfully." << std::endl;
  } else {
    std::cout << "connect to server error." << std::endl;
  }

  close(clientfd);
  return 0;
}