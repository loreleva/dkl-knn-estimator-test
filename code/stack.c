#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stack.h"

double pop(stack *s){
	element *last = s->last;
	double value = NAN;

	if (last){
		value = last->value;
		if (last->next){
			last->next->prev = NULL;
			s->last = last->next;
		}
		else{
			s->last = NULL;
			s->top = NULL;
		}
		s->size--;
		free(last);
	}

	return value;
}

void push(stack *s, double value){
	element *new = (element *) calloc(1, sizeof(element));
	new->value = value;

	element *top = s->top;
	if (top){
		top->next = new;
		new->prev = top;
	}
	else {
		s->last = new;
		new->prev = NULL;
	}
	new->next = NULL;
	s->top = new;
	s->size++;
}

void print_stack(stack *s){
	element *el = s->last;
	if (el){
		printf("%f", el->value);
		while (el->next){
			el = el->next;
			printf(" -> %f", el->value);
		}
		printf("\n");
	}
	else{
		printf("Stack is empty\n");
	}
}

double * get_stack_values(stack *s){
	int size = s->size;
	double * values = (double *) calloc(size, sizeof(double));
	int i=0;
	element *el = s->last;
	for (int i=0; i<size; i++){
		values[i] = el->value;
		el = el->next;
	}
	return values;
}

void free_stack(stack *s){
	element *el = s->last;
	while (el){
		element *temp = el->next;
		free(el);
		el = temp;
	}
	free(s);
}