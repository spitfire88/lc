#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

// Generate a list of random numbers between 1-1000
int* generateRandomArray(int n) {
    int* arr = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000 + 1;
    }
    return arr;
}

// print the list
void printList(struct ListNode* head) {
    struct ListNode* p = head;
    while (p) {
        printf("%d ", p->val);
        p = p->next;
    }
    printf("\n");
}

// Create a new list
struct ListNode* createList(int* arr, int size) {
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* p = head;
    for (int i = 0; i < size; i++) {
        struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
        node->val = arr[i];
        node->next = NULL;
        p->next = node;
        p = node;
    }
    return head->next;
}

// sort the list
struct ListNode* sortBinaryList(struct ListNode* head) {
    struct ListNode* p = head;
    while (p) {
        struct ListNode* q = p->next;
        while (q) {
            if (p->val > q->val) {
                int temp = p->val;
                p->val = q->val;
                q->val = temp;
            }
            q = q->next;
        }
        p = p->next;
    }
    return head;
}

int main(int argc, char* argv[]) {
    int* arr = generateRandomArray(10);
    struct ListNode* l1 = createList(arr, 10);
    printList(l1);
    sortBinaryList(l1);
    printList(l1);
    arr = generateRandomArray(10);
    struct ListNode* l2 = createList(arr, 10);
    printList(l2);
    sortBinaryList(l2);
    printList(l2);

    //struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2);
    return 0;
}
