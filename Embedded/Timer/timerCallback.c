#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//
// SetSystemTimer (ULONG AbsoluteDueTime, VOID *Callback) // can only handle one callback at a time
// GetCurrentTime ()
//

//
// SetAppTimer (ULONG RelativeDueTime, VOID *Callback)
// SetAppTimerCallback ()
//

//
// T0: SetAppTimer (10, A);
// T3: SetAppTimer (5, B);
// T5: SetAppTimer (3, C);
// T8: B expires, C expires
// T10: A expires
//


struct myTimer {
    int delay; 
    int abs_due_time;
    void (*callback)(void);
    struct myTimer *next;
};

struct Timer *head;  // initialized

// L1->3->5->10
// 0: 10
// 3: abs 8 -> 8, 10
// 5: abs 8 -> 8, 8, 10
// 8: pop 8, 8, -> 10
// 10: pop 10

/*void initialize(){
    head = malloc();
    
    head->next = NULL;
}*/



int SetAppTimer(ULONG RelativeDueTime, VOID *Callback) {
    struct Timer *temp = malloc(sizeof(struct myTimer));
    // no error
    temp->delay = RelativeDueTime;
    temp->abs_due_time = GetCurrenttime() + RelativeDueTime;
    temp->callback = Callback;
    temp->next = NULL;
   
    if(head == NULL) {
        SetSystemTimer(temp->abs_due_time, SetAppTimerCallback);
        head = temp;
    } else {
        if(temp->abs_due_time < head->abs_due_time) { // since SetSystemTimer programs low level timer to handle only one callback at a time
            SetSystemTimer(temp->abs_due_time, SetAppTimerCallback);
        }
    }
    insertAt(temp, head); // inserted at <= next->abs_due_time
    return 0;
}

void SetAppTimerCallback() {
    struct Timer *iter = head;
    
    while(iter != NULL && (iter->abs_due_time < GetCurrentTime())) {
        iter->callback();
        struct Timer *temp = iter;
        iter = iter->next;
        remove(temp, head);
        free(temp);
        temp = NULL;
    }
    
    if(head != NULL) {
        SetSystemTimer(head->abs_due_time, SetAppTimerCallback);
    }
    
    return;
}