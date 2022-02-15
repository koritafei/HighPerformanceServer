#include "ctrip_thread_pool.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct ctrip_thread_info g_threadinfo;
int                      thread_running = 0;

void ctrip_init_thread_pool(unsigned long thread_num) {
  if (thread_num <= 0) {
    thread_num = 8;
  }

  pthread_mutex_init(&g_threadinfo.mutex, NULL);
  pthread_cond_init(&g_threadinfo.cond, NULL);

  g_threadinfo.thread_num     = thread_num;
  g_threadinfo.thread_running = 1;
  g_threadinfo.task_num       = 0;
  g_threadinfo.tasks          = NULL;

  thread_running = 1;

  g_threadinfo.thread_id =
      static_cast<pthread_t *>(malloc(sizeof(pthread_t) * thread_num));

  for (unsigned long i = 0; i < thread_num; i++) {
    pthread_create(&g_threadinfo.thread_id[i],
                   NULL,
                   ctrip_thread_routine,
                   NULL);
  }
}

void ctrip_destory_thread_pool() {
  g_threadinfo.thread_running = 0;
  thread_running              = 0;

  pthread_cond_broadcast(&g_threadinfo.cond);

  for (unsigned long i = 0; i < g_threadinfo.thread_num; i++) {
    pthread_join(g_threadinfo.thread_id[i], NULL);
  }

  pthread_mutex_destroy(&g_threadinfo.mutex);
  pthread_cond_destroy(&g_threadinfo.cond);
}

void ctrip_thread_pool_add_task(struct ctrip_task *task) {
  if (NULL == task) {
    return;
  }

  printf("add a task, task value is %d\n", task->value);

  pthread_mutex_lock(&g_threadinfo.mutex);
  struct ctrip_task *head = g_threadinfo.tasks;

  if (NULL == head) {
    g_threadinfo.tasks = task;
  } else {
    while (head->next) {
      head = head->next;
    }

    head->next = task;
  }

  ++g_threadinfo.task_num;

  // 当变化后，signal 通知wait函数
  pthread_cond_signal(&g_threadinfo.cond);
  pthread_mutex_unlock(&g_threadinfo.mutex);
}

struct ctrip_task *ctrip_thread_pool_retrieve_task() {
  struct ctrip_task *head = g_threadinfo.tasks;

  while (nullptr != head) {
    g_threadinfo.tasks = head->next;
    --g_threadinfo.task_num;
    printf("retrieve a task, task value is [%d]\n", head->value);
    return head;
  }

  printf("No Tasks!\n");
  return nullptr;
}

void *ctrip_thread_routine(void *thread_param) {
  printf("thread NO.[%d] start.\n", static_cast<int>(pthread_self()));

  while (thread_running) {
    struct ctrip_task *current = nullptr;
    pthread_mutex_lock(&g_threadinfo.mutex);

    while (g_threadinfo.task_num <= 0) {
      // 当获得了锁，如果条件不适合，wait会释放锁
      pthread_cond_wait(&g_threadinfo.cond, &g_threadinfo.mutex);

      if (!g_threadinfo.thread_running) {
        break;
      }
    }

    current = ctrip_thread_pool_retrieve_task();

    pthread_mutex_unlock(&g_threadinfo.mutex);
    ctrip_pthread_pool_do_task(current);
  }

  printf("thread NO.[%d] exited\n", static_cast<int>(pthread_self()));
  return nullptr;
}

void ctrip_pthread_pool_do_task(struct ctrip_task *task) {
  if (nullptr == task) {
    return;
  }

  printf("do task, task value is %d\n", task->value);

  // delete memory
  free(task);
  task = nullptr;
}