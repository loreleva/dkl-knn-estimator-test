#ifndef stack_h
#define stack_h

typedef struct element element;
struct element{
	double value;
	element *prev;
	element *next;
};


struct stack{
	element *top;
	element *last;
	int size;
};
typedef struct stack stack;

double pop(stack *s);

void push(stack *s, double value);

double * get_stack_values(stack *s);

void print_stack(stack *s);

void free_stack(stack *s);

#endif