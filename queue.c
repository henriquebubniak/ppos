#include <stdio.h>
#include "queue.h"

int queue_size(queue_t *queue) {
    int size;
    struct queue_t *cur = queue;
    if (cur == NULL) {
        return 0;
    }
    size = 1;
    while (cur->next != queue) {
        size += 1;
        cur = cur->next;
    }
    return size;
}
void queue_print (char *name, queue_t *queue, void print_elem (void*) ) {
    struct queue_t *cur = queue;
    printf("[");
    if (cur == NULL) {
        printf("]\n");
        return;
    }
    do {
        print_elem(cur);
        printf(cur->next == queue ? "" : " ");
        cur = cur->next;
    } while (cur != queue);
    printf("]\n");
}
int queue_append(queue_t **queue, queue_t *elem) {
    if (queue == NULL) {
        fprintf(stderr, "queue pointer null\n");
        return -1;
    }
    if (elem == NULL) {
        fprintf(stderr, "element pointer null\n");
        return -1;
    }
    if (elem->next != NULL || elem->prev != NULL) {
        fprintf(stderr, "element is already in queue\n");
        return -1;
    }
    if (*queue == NULL) {
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
        return 0;
    }
    elem->next = *queue;
    elem->prev = (*queue)->prev;
    (*queue)->prev = elem;
    elem->prev->next = elem;
    return 0;
}
int queue_remove (queue_t **queue, queue_t *elem) {
    if (queue == NULL) {
        fprintf(stderr, "queue pointer null\n");
        return -1;
    }
    if (*queue == NULL) {
        fprintf(stderr, "queue is empty\n");
        return -1;

    }
    if (elem == NULL) {
        fprintf(stderr, "element pointer null\n");
        return -1;
    }
    queue_t* cur = *queue;
    while (cur != elem && cur->next != *queue) {
        cur = cur->next;
    }
    if (cur == elem) {
        if (cur->next == cur || cur->prev == cur) {
            *queue = NULL;
            cur->next = NULL;
            cur->prev = NULL;
            return 0;

        }
        queue_t *cur_next, *cur_prev;
        if (cur == *queue) {
            *queue = cur->next;
        }
        cur_next = cur->next;
        cur_prev = cur->prev;
        cur_next->prev = cur_prev;
        cur_prev->next = cur_next;
        cur->next = NULL;
        cur->prev = NULL;
        return 0;
    }
    fprintf(stderr, "element not in queue\n");
    return -1;
}

