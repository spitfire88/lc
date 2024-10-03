#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

// linked list
typedef struct node {
    int data;
    struct node *next;
} node_t;

node_t *init(void) {
    node_t *x = malloc(sizeof(node_t));
    x->data = INT_MIN;
    x->next = NULL;
    return x;
}

int addnode(node_t *x, int data) {
    node_t *t = x;
    if(x == NULL)
        return -1;
    
    while(t->next != NULL) 
        t = t->next;

    t->next = malloc(sizeof(node_t));
    t->next->data = data;
    t->next->next = NULL;
    return 0;
}

node_t *removenode(node_t *x, int pos) {
    node_t *t = x;
    node_t *s = NULL;

    if(x == NULL) return NULL;

    if(pos == 0) {
        while(t != NULL) {
            x = t;
            t = t->next;
            free(x);
            x = NULL;
        }
        return NULL;
    }

    while(t->next != NULL && pos) {
        pos--;
        s = t;
        t = t->next;
    }

    if (pos == 0) {
        s->next = t->next;
        free(t);
        return x;
    } else if (t->next == NULL) {
        return x;
    }
}

void printnode(node_t *x) {
    int i = 0;

    if(x == NULL)
        return;
    
    x = x->next;
    while(x != NULL && ++i) {
        printf("%d: %d\t", i, x->data);
        x = x->next;
    }
    printf("\n");
    return;
}

int main() {
    node_t *y = init();
    printnode(y);
    assert(addnode(y, 1) == 0);
    printnode(y);
    assert(addnode(y, 2) == 0);
    printnode(y);
    assert(addnode(y, 3) == 0);
    printnode(y);
    y = removenode(y, 10);
    printnode(y);
    y = removenode(y, 0);
    printnode(y);
    #if 0
    assert(removenode(y, 1) == 0);
    printnode(y);
    assert(removenode(y, 2) == 0);
    printnode(y);
    assert(removenode(y, 1) == 0);
    printnode(y);
    #endif

    return 0;
}