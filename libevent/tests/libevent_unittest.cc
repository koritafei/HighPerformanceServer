#include <event.h>
#include <event2/event_struct.h>
#include <time.h>

#include <iostream>

struct event   ev;
struct timeval tv;

void time_cb(int fd, short event, void *argc) {
  std::cout << "timer wakeup!" << std::endl;
  event_add(&ev, &tv);
}

int main(int argc, char **argv) {
  struct event_base *base = event_init();
  tv.tv_sec               = 10;
  tv.tv_usec              = 0;
  evtimer_set(&ev, time_cb, NULL);
  event_base_set(base, &ev);
  event_add(&ev, &tv);

  event_base_dispatch(base);
}