#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>

// 自定义代表无效的FD值
#define INVALID_FD -1

int main(int argc, char **argv) {
  // 创建一个侦听socket
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == listenfd) {
    std::cout << "create socket error!" << std::endl;
    return -1;
  }

  // 初始化服务器地址
  struct sockaddr_in bindaddr;
  bindaddr.sin_family      = AF_INET;
  bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  bindaddr.sin_port        = htons(3000);

  if (-1 == bind(listenfd,
                 reinterpret_cast<struct sockaddr *>(&bindaddr),
                 sizeof(struct sockaddr))) {
    std::cout << "bind socket error" << std::endl;
    close(listenfd);
    return -1;
  }

  // 启动侦听
  if (-1 == listen(listenfd, SOMAXCONN)) {
    std::cout << "listen socket error" << std::endl;
    close(listenfd);
    return -1;
  }

  // 存储客户端socket数据
  std::vector<int> clientfds;
  int              maxfd = listenfd;

  while (true) {
    fd_set readset;
    FD_ZERO(&readset);

    // 将侦听socket加入到待检测的可读事件中
    FD_SET(listenfd, &readset);

    // 将客户端fd加入到待检测的可读事件中去
    unsigned long clientfdslength = clientfds.size();
    for (unsigned long i = 0; i < clientfdslength; i++) {
      if (INVALID_FD != clientfds[i]) {
        FD_SET(clientfds[i], &readset);
      }
    }

    timeval tv;
    tv.tv_sec  = 1;
    tv.tv_usec = 0;

    // 只检测可读事件
    int ret = select(maxfd + 1, &readset, nullptr, nullptr, &tv);

    if (-1 == ret) {
      // 出错，退出程序
      if (EINTR != errno) {
        break;
      }
    } else if (0 == ret) {
      // select 函数超时，下次继续
      continue;
    } else {
      // 检测到某个socket有事件
      if (FD_ISSET(listenfd, &readset)) {
        // 侦听到socket可读事件，则表示有新的链接到来
        struct sockaddr_in clientaddr;
        socklen_t          clientaddrlen = sizeof(clientaddr);

        // 接受客户端连接
        int clientfd = accept(listenfd,
                              reinterpret_cast<struct sockaddr *>(&clientaddr),
                              &clientaddrlen);
        if (-1 == clientfd) {
          // 连接出错，直接返回
          break;
        }

        // 只接受连接不接受任何数据
        std::cout << "accept a client connection, fd: " << clientfd
                  << std::endl;
        clientfds.push_back(clientfd);

        // 记录下最新的最大maxfd，方便后续更新
        maxfd = maxfd > clientfd ? maxfd : clientfd;
      } else {
        // 假设对端发来的数据不超过64
        char          recvbuf[64];
        unsigned long clientfdslen = clientfds.size();

        for (unsigned long i = 0; i < clientfdslen; i++) {
          if (-1 != clientfds[i] && FD_ISSET(clientfds[i], &readset)) {
            memset(recvbuf, 0, sizeof(recvbuf));
            // 非侦听socket，继续接受数据
            ssize_t recvlen = recv(clientfds[i], recvbuf, 64, 0);
            if (recvlen <= 0 && errno != EINTR) {
              // 收取数据出错
              std::cout << "recv data error, clientfd " << clientfds[i]
                        << std::endl;
              close(clientfds[i]);
              // 不直接删除该元素，将当前位置置为INVALID
              clientfds[i] = INVALID_FD;
              continue;
            }

            std::cout << "clientfd : " << clientfds[i] << " recv data is "
                      << recvbuf << std::endl;
          }
        }
      }
    }
  }

  // 关闭所有客户端
  unsigned long clientfdslength = clientfds.size();
  for (unsigned long i = 0; i < clientfdslength; i++) {
    if (INVALID_FD != clientfds[i]) {
      close(clientfds[i]);
    }
  }

  close(listenfd);

  return 0;
}