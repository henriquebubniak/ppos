#include "ppos.h"
#include "ppos_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACKSIZE 64*1024	/* tamanho de pilha das threads */

task_t main_task;
task_t *current_task;
task_t *ready_tasks_queue;
int available_tid = 0;


void ppos_init() {
    setvbuf(stdout, 0, _IONBF, 0);
    current_task = &main_task;
}

int task_init(task_t *task, void (*start_func)(void *), void *arg) {
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
    return task->tid;
}

int task_switch(task_t *task) {
    if (task == NULL) {
        return -1;
    }
    task_t *old_task;
    ucontext_t old_context;
    old_task = current_task;
    current_task = task;
    task->status = 1;
    swapcontext(&old_context, &task->context);
    old_task->context = old_context;
    old_task->status = 0;
    return 1;
}

void task_exit(int exit_code) {
    task_switch(&main_task);
}
