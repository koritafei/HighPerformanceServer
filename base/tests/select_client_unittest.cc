#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 3000

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
  serveraddr.sin_port        = htons(SERVER_PORT);

  // 连接服务器
  if (-1 == connect(clientfd,
                    reinterpret_cast<struct sockaddr *>(&serveraddr),
                    sizeof(serveraddr))) {
    std::cout << "connect error " << std::endl;
    close(clientfd);
    return -1;
  }

  int ret;
  int count = 0;

  while (true) {
    fd_set readset;
    FD_ZERO(&readset);

    // 将侦听socket加入到待检测的可读事件中去
    FD_SET(clientfd, &readset);
    timeval tv;
    tv.tv_sec  = 5;
    tv.tv_usec = 0;
    // 暂且只检测可读事件
    ret = select(clientfd + 1, &readset, nullptr, nullptr, &tv);
    std::cout << "tv.tv_sec : " << tv.tv_sec << ", tv.tv_usec : " << tv.tv_usec
              << std::endl;

    if (-1 == ret) {
      // 被中断信号的情况，其余情况均为出错
      if (EINTR == errno) {
        break;
      }
    } else if (0 == ret) {
      // select 函数超时
      std::cout << "no event in specific time interval, count：" << count
                << std::endl;
      ++count;
      continue;
    } else {
      if (FD_ISSET(clientfd, &readset)) {
        // 检测到可读事件
        char buf[32];
        memset(&buf, 0, sizeof(buf));

        ssize_t n = recv(clientfd, &buf, 32, 0);
        if (n < 0) {
          // 除了信号被中断情况，其余情况均为出错
          if (EINTR != errno) {
            break;
          }
        } else if (0 == n) {
          // 对端关闭了连接
          break;
        } else {
          std::cout << "recv data: " << buf << std::endl;
        }
      } else {
        std::cout << "other socket event" << std::endl;
      }
    }
  }

  // 关闭socket
  close(clientfd);
  return 0;
}
