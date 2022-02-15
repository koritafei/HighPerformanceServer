#include "../thread_pool/ctrip_thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  ctrip_init_thread_pool(5);
  struct ctrip_task *task = nullptr;

  for (int i = 0; i < 100; i++) {
    task = static_cast<struct ctrip_task *>(malloc(sizeof(struct ctrip_task)));
    task->value = i + 1;
    task->next  = nullptr;
    printf("new task, task value [%d]\n", task->value);
    usleep(static_cast<unsigned int>(i));
    ctrip_thread_pool_add_task(task);
  }

  sleep(100);
  ctrip_destory_thread_pool();

  return 0;
}
