#ifndef __CTRIP_THREAD_POOL_H__
#define __CTRIP_THREAD_POOL_H__

// 生产者消费者的任务队列

#include <pthread.h>

#ifndef NULL
#define NULL 0
#endif

#define PUBLIC

PUBLIC struct ctrip_task {
  struct ctrip_task *next;
  int                value;
};

struct ctrip_thread_info {
  // 线程退出标志
  unsigned long      thread_running;
  unsigned long      thread_num;
  unsigned long      task_num;
  struct ctrip_task *tasks;
  pthread_t         *thread_id;
  pthread_mutex_t    mutex;
  pthread_cond_t     cond;
};

/**
 * @brief 初始化线程池线程数目
 * @param  thread_num      默认为8
 * @return PUBLIC
 * */
PUBLIC void ctrip_init_thread_pool(unsigned long thread_num = 8);

/**
 * @brief 销毁线程
 * @return PUBLIC
 * */
PUBLIC void ctrip_destory_thread_pool();

/**
 * @brief 向任务池中增加一个任务
 * @param  task
 * @return PUBLIC
 * */
PUBLIC void ctrip_thread_pool_add_task(struct ctrip_task *task);

/**
 * @brief 从任务池中取一个任务
 * @return struct ctrip_task*
 * */
struct ctrip_task *ctrip_thread_pool_retrieve_task();

/**
 * @brief 执行一个任务
 * @return PUBLIC
 * */
PUBLIC void ctrip_pthread_pool_do_task(struct ctrip_task *);

/**
 * @brief 线程函数
 * @param  thread_param
 * @return void
 * */
void *ctrip_thread_routine(void *thread_param);

#endif /* __CTRIP_THREAD_POOL_H__ */
