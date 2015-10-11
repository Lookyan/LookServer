#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <pthread.h>

typedef struct worker {
	pthread_t thread;
	int terminate;
	struct workqueue *workqueue;
	struct worker *prev;
	struct worker *next;
} worker_t;

typedef struct job {
	void (*job_function)(struct job *job);
	void *user_data;
	struct job *prev;
	struct job *next;
} job_t;

typedef struct workqueue {
	struct worker *workers;
	struct job *waiting_jobs;
	pthread_mutex_t jobs_mutex;
	pthread_cond_t jobs_cond;
} workqueue_t;

class WorkQueue
{
public:
    ~WorkQueue();   
    static void workqueue_add_job(workqueue_t *workqueue, job_t *job);    
    static void workqueue_shutdown(workqueue_t *workqueue);      
    static int workqueue_init(workqueue_t *workqueue, int numWorkers);     
    static void *worker_function(void *ptr);
private:
    WorkQueue();
};

#endif // WORKQUEUE_H
