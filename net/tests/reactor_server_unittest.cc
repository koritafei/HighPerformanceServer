#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>

#define WORK_THREAD_NUM 5

#define min(a, b) (a <= b ? (a) : (b))
int             g_epollfd                   = 0;
bool            g_bStop                     = false;
int             g_listenfd                  = 0;
pthread_t       g_acceptthreadid            = 0;
pthread_t       g_threadid[WORK_THREAD_NUM] = {0};
pthread_cond_t  g_acceptcond;
pthread_mutex_t g_acceptmutex;

pthread_cond_t  g_cond;
pthread_mutex_t g_mutex;

pthread_mutex_t g_clientmutex;
std::list<int>  g_listClients;

void prog_exit(int signo) {
  ::signal(SIGINT, SIG_IGN);
  ::signal(SIGTERM, SIG_IGN);

  std::cout << "program recv signal " << signo << " to exit." << std::endl;

  g_bStop = true;
  epoll_ctl(g_epollfd, EPOLL_CTL_DEL, g_listenfd, 0);

  ::shutdown(g_listenfd, SHUT_RDWR);
  ::close(g_epollfd);
  ::close(g_listenfd);

  ::pthread_cond_destroy(&g_acceptcond);
  ::pthread_mutex_destroy(&g_acceptmutex);

  ::pthread_cond_destroy(&g_cond);
  ::pthread_mutex_destroy(&g_mutex);

  ::pthread_mutex_destroy(&g_clientmutex);
}

bool create_server_listen(const char *ip, short port) {
  g_listenfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (-1 == g_listenfd) {
    return false;
  }

  int on = 1;
  ::setsockopt(g_listenfd,
               SOL_SOCKET,
               SO_REUSEADDR,
               reinterpret_cast<char *>(&on),
               sizeof(on));
  ::setsockopt(g_listenfd,
               SOL_SOCKET,
               SO_REUSEPORT,
               reinterpret_cast<char *>(&on),
               sizeof(on));

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(ip);
  serveraddr.sin_port        = htons(port);

  if (-1 == ::bind(g_listenfd,
                   reinterpret_cast<struct sockaddr *>(&serveraddr),
                   sizeof(struct sockaddr))) {
    return false;
  }

  if (-1 == listen(g_listenfd, 50)) {
    return false;
  }

  g_epollfd = ::epoll_create1(1);

  if (-1 == g_epollfd) {
    return false;
  }

  struct epoll_event epev;
  memset(&epev, 0, sizeof(struct epoll_event));
  epev.events  = EPOLLIN | EPOLLOUT;
  epev.data.fd = g_listenfd;

  if (-1 == epoll_ctl(g_epollfd, EPOLL_CTL_ADD, g_listenfd, &epev)) {
    return false;
  }

  return true;
}

void release_client(int clientfd) {
  if (-1 == epoll_ctl(g_epollfd, EPOLL_CTL_DEL, clientfd, NULL)) {
    std::cout << "release client socket failed as call epoll_ctl failed"
              << std::endl;
  }
  ::close(clientfd);
}

void *accept_thread_func(void *arg) {
  while (!g_bStop) {
    ::pthread_mutex_lock(&g_acceptmutex);
    ::pthread_cond_wait(&g_acceptcond, &g_acceptmutex);

    struct sockaddr_in clientaddr;
    socklen_t          addrlen;
    int                newfd = ::accept(g_listenfd,
                         reinterpret_cast<struct sockaddr *>(&clientaddr),
                         &addrlen);
    ::pthread_mutex_unlock(&g_acceptmutex);

    if (-1 == newfd) {
      continue;
    }

    std::cout << "new client connected : " << ::inet_ntoa(clientaddr.sin_addr)
              << " : " << ::ntohs(clientaddr.sin_port) << std::endl;

    // 将新的socket设置成nonblocking
    int oldflag = ::fcntl(newfd, F_GETFL, 0);
    int newflag = oldflag | O_NONBLOCK;
    if (-1 == ::fcntl(newfd, F_SETFL, newflag)) {
      std::cout << "fcntl error, oldflag =" << oldflag
                << ", newflag = " << newflag << std::endl;
      continue;
    }

    struct epoll_event epev;
    memset(&epev, 0, sizeof(struct epoll_event));
    epev.events  = EPOLLIN | EPOLLRDHUP | EPOLLET;
    epev.data.fd = newfd;

    if (-1 == epoll_ctl(g_epollfd, EPOLL_CTL_ADD, newfd, &epev)) {
      std::cout << "epoll_ctl error, fd =" << newfd << std::endl;
    }
  }

  return nullptr;
}

void *worker_thread_func(void *arg) {
  while (!g_bStop) {
    int clientfd;
    ::pthread_mutex_lock(&g_clientmutex);

    while (g_listClients.empty()) {
      ::pthread_cond_wait(&g_cond, &g_clientmutex);
    }

    clientfd = g_listClients.front();
    g_listClients.pop_front();
    ::pthread_mutex_unlock(&g_clientmutex);

    // gdb
    // 调试不能实时刷新标准输出，使用这个函数刷新标准输出，使得信息实时在屏幕显示
    std::cout << std::endl;

    std::string clientmsg;
    char        buf[256];
    bool        bError = false;

    while (true) {
      memset(buf, 0, sizeof(buf));
      ssize_t nRecv = ::recv(clientfd, buf, 356, 0);

      if (-1 == nRecv) {
        if (EWOULDBLOCK == errno) {
          break;
        } else {
          std::cout << "recv error, client disconnected, fd = " << clientfd
                    << std::endl;
          release_client(clientfd);
          bError = true;
          break;
        }
      } else if (0 == nRecv) {
        // 对端关闭
        std::cout << "peer closed, client disconnected, fd = " << clientfd
                  << std::endl;
        release_client(clientfd);
        bError = true;
        break;
      }
      clientmsg += buf;
    }
    // 出错了，无需继续执行
    if (bError) {
      continue;
    }

    std::cout << "client msg : " << clientmsg << std::endl;

    // 将消息添加时间戳之后返回
    time_t             now    = time(NULL);
    struct tm         *nowstr = localtime(&now);
    std::ostringstream ostimestr;
    ostimestr << "[" << nowstr->tm_year + 1900 << "-" << std::setw(2)
              << std::setfill('0') << nowstr->tm_mon + 1 << std::setw(2)
              << std::setfill('0') << nowstr->tm_mday << " " << std::setw(2)
              << std::setfill('0') << nowstr->tm_hour << ":" << std::setw(2)
              << std::setfill('0') << nowstr->tm_min << ":" << std::setw(2)
              << std::setfill('0') << nowstr->tm_sec << "]server reply: ";
    clientmsg.insert(0, ostimestr.str());

    while (true) {
      ssize_t nSend =
          ::send(clientfd, clientmsg.c_str(), clientmsg.length(), 0);
      if (-1 == nSend) {
        if (EWOULDBLOCK == errno) {
          ::sleep(10);
          continue;
        } else {
          std::cout << "send error, fd = " << clientfd << std::endl;
          release_client(clientfd);
          break;
        }
      }

      std::cout << "send : " << clientmsg;
      clientmsg.erase(0, nSend);

      if (clientmsg.empty()) {
        break;
      }
    }
  }

  return nullptr;
}

void daemon_run() {
  int pid;
  ::signal(SIGCHLD, SIG_IGN);

  // 在父进程中，fork返回新创建的子进程id
  // 在子进程中，fork返回0
  // 如果出现错误，fork返回负值
  pid = fork();
  if (pid < 0) {
    std::cout << "fork error" << std::endl;
    exit(-1);
  } else if (pid > 0) {
    // 父进程退出，子进程独立运行
    exit(0);
  }
  // parent与child运行在同一个session中，parent是会话领头进程
  // parent
  // 作为领头的会话进程，当调用exit结束时，子进程会变成孤儿进程，被init进程接管
  // 执行setsid()之后，child将重新获得一个新的会话(session)id
  // 此时parent退出不会影响child
  setsid();
  int fd;
  fd = open("/dev/null", O_RDWR, 0);

  if (-1 != fd) {
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
  }

  if (fd > 2) {
    close(fd);
  }
}

int main(int argc, char **argv) {
  short port = 0;
  int   ch;
  bool  bdaemon = false;

  while (-1 == (ch = getopt(argc, argv, "p:d"))) {
    switch (ch) {
      case 'd':
        bdaemon = true;
        break;
      case 'p':
        port = static_cast<short>(atol(optarg));
        break;
    }
  }

  if (bdaemon) {
    daemon_run();
  }

  if (0 == port) {
    port = 12345;
  }

  if (!create_server_listen("0.0.0.0", port)) {
    std::cout << "Unable to create listen server: ip=0.0.0.0, port=" << port
              << "." << std::endl;
    return -1;
  }

  // 设置信号处理
  ::signal(SIGCHLD, SIG_DFL);
  ::signal(SIGPIPE, SIG_IGN);
  ::signal(SIGINT, prog_exit);
  ::signal(SIGTERM, prog_exit);

  ::pthread_cond_init(&g_acceptcond, NULL);
  ::pthread_mutex_init(&g_acceptmutex, NULL);
  ::pthread_cond_init(&g_cond, NULL);
  ::pthread_mutex_init(&g_mutex, NULL);
  ::pthread_mutex_init(&g_clientmutex, NULL);

  ::pthread_create(&g_acceptthreadid, NULL, accept_thread_func, NULL);

  // 启动工作线程
  for (int i = 0; i < WORK_THREAD_NUM; i++) {
    ::pthread_create(&g_threadid[i], NULL, worker_thread_func, NULL);
  }

  while (!g_bStop) {
    struct epoll_event ev[1024];
    int                n = ::epoll_wait(g_epollfd, ev, 1024, 10);
    if (0 == n) {
      continue;
    } else if (n < 0) {
      std::cout << "epoll_wait error" << std::endl;
      continue;
    }

    int m = min(0, 10);
    for (int i = 0; i < m; i++) {
      // 通知接收进程接收新连接
      if (g_listenfd == ev[i].data.fd) {
        pthread_cond_signal(&g_acceptcond);
      } else {
        // 通知普通工作线程接收数据
        ::pthread_mutex_lock(&g_clientmutex);
        g_listClients.push_back(ev[i].data.fd);
        ::pthread_mutex_unlock(&g_clientmutex);
        ::pthread_cond_signal(&g_cond);
      }
    }
  }

  return 0;
}