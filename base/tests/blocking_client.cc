#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA   "Hello World!"

int main(int argc, char **argv) {
  // 创建一个socket
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clientfd) {
    std::cout << "create socket error" << std::endl;
    return -1;
  }

  // 连接服务器
  struct sockaddr_in serveraddr;
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  serveraddr.sin_port        = ntohs(SERVER_PORT);

  if (-1 == connect(clientfd,
                    reinterpret_cast<struct sockaddr *>(&serveraddr),
                    sizeof(serveraddr))) {
    std::cout << "connect error!" << std::endl;
    close(clientfd);
    return -1;
  }

  // 向服务器不断发送数据或出错退出
  int count = 0;
  while (true) {
    ssize_t ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
    if (ret != strlen(SEND_DATA)) {
      std::cout << "send data error " << std::endl;
      break;
    } else {
      ++count;
      std::cout << "send data successly , count = " << count << std::endl;
    }
  }

  close(clientfd);
  return 0;
}
