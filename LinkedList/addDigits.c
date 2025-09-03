
#include <stdio.h>
#include <malloc.h>

typedef unsigned long ULONG;

// NUMBER structure definition
typedef struct _NUMBER {
    ULONG Digit;
    struct _NUMBER *Next;
} NUMBER, *PNUMBER;

// Forward declared function prototypes. ***Code implementation below main function***
void PrintNumber(NUMBER *Number);
int InitializeNumbers (NUMBER *Number1, NUMBER *Number2);
PNUMBER AddNumbers(NUMBER *Number1, NUMBER *Number2);

//
// Program main entry
//
int 
main() 
{    
    NUMBER number1, number2;
    NUMBER *sumNumbers;
    InitializeNumbers(&number1, &number2);
    
    printf("First number: ");
    PrintNumber(&number1);
        
    printf("Second number: ");
    PrintNumber(&number2);
    //
    // Now add the lists
    //
    sumNumbers = AddNumbers(&number1, &number2);
    if(sumNumbers != NULL)
    {
        printf("\nSum: ");
        PrintNumber(sumNumbers);        
    }else{
        printf("\nFailed to add numbers!\n");
    }
    
    return 0;
}

PNUMBER
ReverseNumber(
    PNUMBER Number
)
{
    PNUMBER Prev, Current, Temp;

    if (Number == NULL){
        return NULL;
    } else if (Number->Next == NULL){
        return Number;
    }

    Prev = NULL;
    Current = Number;
    while(Current){
        Temp = Current;
        Current = Current->Next;
        Temp->Next = Prev;
        Prev = Temp;
    }

    return Temp;
}

// Routine that takes two NUMBER lists, adds them together, and returns a *new* NUMBER list representing the sum.
// FIX ME! THIS LOGIC IS NOT CORRECT

PNUMBER 
AddNumbers(
    NUMBER *Number1,
    NUMBER *Number2
    )
{
    
    PNUMBER ReversedNum1 = ReverseNumber(Number1);
    PNUMBER ReversedNum2 = ReverseNumber(Number2);
    PNUMBER ResultNum = NULL;
    PNUMBER NextNum; 
    ULONG carryOver = 0;
    
    while(ReversedNum1 != NULL && ReversedNum2 != NULL) {
        ULONG temp = ReversedNum1->Digit + ReversedNum2->Digit + carryOver;
        NextNum = (PNUMBER) malloc(sizeof(NUMBER));
        if(ResultNum == NULL) {
            ResultNum = NextNum;
        }
        NextNum->Digit = temp / 10;
        carryOver = temp % 10;
        NextNum = NextNum->Next;
        ReversedNum1 = ReversedNum1->Next;
        ReversedNum2 = ReversedNum2->Next;
    }
    
    while(ReversedNum1 != NULL) {
        ULONG temp = carryOver + ReversedNum1->Digit;
        NextNum = (PNUMBER) malloc(sizeof(NUMBER));
        NextNum->Digit = temp / 10;
        carryOver = temp % 10;
        NextNum = NextNum->Next;
        ReversedNum1 = ReversedNum1->Next;
    }
    
    while(ReversedNum2 != NULL) {
        ULONG temp = carryOver + ReversedNum2->Digit;
        NextNum = (PNUMBER) malloc(sizeof(NUMBER));
        NextNum->Digit = temp / 10;
        carryOver = temp % 10;
        NextNum = NextNum->Next;
        ReversedNum2 = ReversedNum2->Next;
    }
    
    return ReverseNumber(ResultNum);
}

int 
InitializeNumbers ( 
    NUMBER *Number1, 
    NUMBER *Number2
    )
{
    NUMBER *currentDigit, *nextDigit;    
    
    //
    // First number is 2339
    // [SANRIO] why hardcode the value?
    ULONG firstNumber[4] = {2, 3, 3, 9};
    currentDigit = Number1;
    currentDigit->Digit = firstNumber[0];
    for (int i = 1; i < 4; i++){ // change hardcoded 4 to sizeof(firstnumber)/sizeof(ULONG)        
        nextDigit = (PNUMBER) malloc(sizeof(NUMBER));        
        if (nextDigit == NULL) return -1;  // define error codes
        nextDigit->Digit = firstNumber[i];
        nextDigit->Next = NULL;
        currentDigit->Next = nextDigit;
        currentDigit = nextDigit;
    }
    
    //
    // Second number is 264
    //
    ULONG secondNumber[3] = {2, 6, 4};    
    currentDigit = Number2;    
    currentDigit->Digit = secondNumber[0];
    for (int i = 1; i < 3; i++){        
        nextDigit = (PNUMBER) malloc(sizeof(NUMBER));        
        if (nextDigit == NULL) return -1;        
        nextDigit->Digit = secondNumber[i];
        nextDigit->Next = NULL;
        currentDigit->Next = nextDigit;
        currentDigit = nextDigit;
    }
    return 0;
}

void 
PrintNumber(
    NUMBER *Number
    )
{
    NUMBER *p = Number;
    while(p){
        printf("%lu", p->Digit);
        p=p->Next;
    }
    printf("\n");
}
