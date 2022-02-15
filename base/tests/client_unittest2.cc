#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA   "Hello World!"

int main(int argc, char **argv) {
  // 1. 创建一个socket
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clientfd) {
    std::cout << "create socket error." << std::endl;
    return -1;
  }

  // 调用bind
  struct sockaddr_in bindaddr;
  bindaddr.sin_family      = AF_INET;
  bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  bindaddr.sin_port        = htons(0);

  if (-1 == bind(clientfd,
                 reinterpret_cast<struct sockaddr *>(&bindaddr),
                 sizeof(struct sockaddr))) {
    std::cout << "bind socket error" << std::endl;
    return -1;
  }

  // 2. 连接服务器
  struct sockaddr_in serveraddr;
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  serveraddr.sin_port        = htons(SERVER_PORT);

  if (-1 == (connect(clientfd,
                     reinterpret_cast<struct sockaddr *>(&serveraddr),
                     sizeof(serveraddr)))) {
    std::cout << "connect socket error." << std::endl;
    return -1;
  }

  // 3. 向服务器发送数据
  ssize_t ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
  if (ret != strlen(SEND_DATA)) {
    std::cout << "send data error" << std::endl;
    return -1;
  }

  std::cout << "send data sucessly, data : " << SEND_DATA << std::endl;

  // 4. 从服务端接收数据
  char recvBuf[32] = {0};
  ret              = recv(clientfd, recvBuf, 32, 0);
  if (ret > 0) {
    std::cout << "recv data sucessly, data : " << recvBuf << std::endl;
  } else {
    std::cout << "recv data error, data : " << recvBuf << std::endl;
  }

  // 5. 关闭socket
  // close(clientfd);
  while (true) {
    sleep(2);
  }

  return 0;
}