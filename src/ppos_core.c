#include "ppos.h"
#include "ppos_data.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACKSIZE 64*1024	/* tamanho de pilha das threads */

task_t dispatcher;
task_t main_task;
task_t *current_task = NULL;
queue_t *ready_tasks_queue = NULL;
int available_tid = 1;


void print_elem_id(void* arg) {
    if (arg == NULL) {
        return;
    }
    printf("%d, ", ((task_t*)arg)->tid);
}
void print_elem_prio(void* arg) {
    if (arg == NULL) {
        return;
    }
    printf("%d:p%d, ", ((task_t*)arg)->tid, ((task_t*)arg)->dynamic_priority);
}

void ppos_init() {
    #ifdef DEBUG
        printf ("ppos iniciado\n") ;
    #endif
    setvbuf(stdout, 0, _IONBF, 0);
    getcontext(&main_task.context);
    main_task.tid = 0;
    main_task.status = 1;
    current_task = &main_task;
    make_task(&dispatcher, dispatch, "");
}

int make_task(task_t *task, void (*start_func)(void *), void *arg) {
    if (task == NULL || start_func == NULL) {
        return -1;
    }
    char* stack;
    ucontext_t context;
    getcontext(&context);
    stack = malloc (STACKSIZE);
    if (stack) {
        context.uc_stack.ss_sp = stack;
        context.uc_stack.ss_size = STACKSIZE;
        context.uc_stack.ss_flags = 0;
        context.uc_link = 0;
    } else {
        return -1;
    }
    makecontext (&context, (void*)(*start_func), 1, (char*)arg);
    task->context = context;
    task->tid = available_tid;
    available_tid += 1;
    task->status = 0;
    task->dynamic_priority = 0;
    task->static_priority = 0;
    return task->tid;
}

int task_init(task_t *task, void (*start_func)(void *), void *arg) {
    if (task == NULL || start_func == NULL) {
        return -1;
    }
    if (make_task(task, start_func, arg) < 0) {
        return -1;
    };
    queue_append(&ready_tasks_queue, (queue_t*)task);
    #ifdef DEBUG
        printf ("task_init: iniciada tarefa %d\n", task->tid) ;
    #endif
    return task->tid;
}

int task_switch(task_t *task) {
    #ifdef DEBUG
        printf("task_switch: task %d -> task %d\n", current_task->tid, task->tid);
    #endif
    if (task == NULL) {
        return -1;
    }
    task_t *old_task;
    old_task = current_task;
    current_task = task;
    task->status = 1;
    swapcontext(&old_task->context, &task->context);
    old_task->status = 0;
    return 1;
}

void task_exit(int exit_code) {
    #ifdef DEBUG
        printf ("task_exit: encerrada tarefa %d\n", task_id()) ;
    #endif
    task_switch(&dispatcher);
}

int task_id() {
    return current_task->tid;
}

void task_yield() {
    queue_append(&ready_tasks_queue, (queue_t*)current_task);
    task_switch(&dispatcher);
}

void dispatch() {
    while(1) {
        task_t* next_task = schedule();
        if (next_task == NULL) {
            return;
        }
        queue_remove(&ready_tasks_queue, (queue_t*)next_task);
        task_switch(next_task);

        #ifdef DEBUG
            printf("dispatch next_task: %d\n", next_task->tid);
            queue_print("Ready", ready_tasks_queue, print_elem);
        #endif
    }
}

task_t* schedule() {
    return (task_t*)ready_tasks_queue;
void task_setprio(task_t *task, int prio) {
    if (task == NULL) {
        current_task->static_priority = prio;
        current_task->dynamic_priority = prio;
    }
    task->static_priority = prio;
    task->dynamic_priority = prio;
}

int task_getprio(task_t *task) {
    if (task == NULL) {
        return current_task->static_priority;
    }
    return task->static_priority;
}
