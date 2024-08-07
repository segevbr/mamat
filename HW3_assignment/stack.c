#include "stack.h"
#include "common.h"
#include <stdlib.h>

/* --- Defining the ADT needed - stack implementation using linked list --- */

typedef struct node {
    /* Must use struct node* because Node/node* typedef is only defined outside
       of the defenition */
    struct node* next; 

    /* Pointer to the element in a stack node */
    elem_t value;
} Node;

typedef struct stack {
    Node* top;              /* Store the top node */
    int max_size;           /* Max size of the stack given by user */
    int current_size;
    clone_t clone;          /* Clone function of the user */
    destroy_t destroy;    /* Mememory dislocation function of the user */
    print_t print;          /* Printing function of the user */
} stack;


/* -------- Implementation of the functions to handle the stack --------*/

stack* stack_create(int max_stack_size, clone_t clone,
                    destroy_t destroy, print_t print){

    /* Initialize a pointer to the newly created stack */
    stack* new_stack;
    new_stack = (stack *)malloc(sizeof(stack));
    
    /* Check if the stack was initialized unsuccessfully (memory allocation) */    
    if (!new_stack || max_stack_size <= 0){
        stack_destroy(new_stack);  
        return NULL;        /* Return error*/
    }

    new_stack->top = NULL;
    new_stack->max_size = max_stack_size;
    new_stack->current_size = 0;
    new_stack->clone = clone;
    new_stack->destroy = destroy;
    new_stack->print = print;

    return new_stack;       /* Return a pointer to the stack ADT */
}

STACK_STATUS stack_destroy(stack* stack_pointer) {
    
    if (stack_pointer == NULL){   /* If the stack isn't empty*/
        free(stack_pointer);               /* free the stack memory*/
    } else {

        /* Assign a temporary node so we can free up the memory it contain and 
        assign the next node as top */
        Node* tmp_top_to_remove = stack_pointer->top;

        while (!stack_is_empty(stack_pointer)){
            
            /* Set tmp to the top of stack*/
            tmp_top_to_remove = stack_pointer->top; 
            
            /* Set top to next node*/
            stack_pointer->top = stack_pointer->top->next; 

            /* Dislocate the element inside the note using the user destroy
               function and decrease the current number of elements by 1 */
            stack_pointer->destroy(tmp_top_to_remove->value);
            stack_pointer->current_size--;
            
            /* Dislocate the memory of what used to be the top node in stack */
            free(tmp_top_to_remove);
        }

        /* After we've done freeing each node in the stack we will free the
           entire stack struct */
        free(stack_pointer->top);
        free(stack_pointer); 
        }

    /* Check if stack memory was disloacated properly */
    if (stack_pointer != NULL){        /* If stack pointer is not NULL */
        return STACK_FAILURE;
    }
        
    return STACK_SUCCESS;
}

STACK_STATUS stack_push(stack* stack_pointer, elem_t* new_element){

    /* Check if the stack was created */    
    if (stack_pointer == NULL){
        return STACK_FAILURE;

    } else if (stack_pointer->current_size == stack_pointer->max_size){
        /* Return failure if current number fo elements in stack is 
           bigger than size*/
        return STACK_FAILURE; 

    } else {

        /* Insert a copy of the element as value */
        Node* new_node;
        new_node = (Node *)malloc(sizeof(Node));

        /* Checking memory allocation of malloc */
        if (!new_node){
            free(new_node);  
            return STACK_FAILURE;
        }
        
        new_node->value = stack_pointer->clone(new_element);

        if (new_node->value == NULL){
            free(new_node);
            return STACK_FAILURE;
        }

        /* Temporary node to check if a new node was pushed successfully*/
        //Node* tmp_node = stack_pointer-> top;
        new_node->next = stack_pointer->top; 
        stack_pointer->top = new_node;
        stack_pointer->current_size++;  

        
        /* If the top isn't the new node or set to null return failure */
        if (stack_pointer->top != new_node){
            stack_pointer->destroy(new_node->value);
            free(new_node);
            return STACK_FAILURE;
        }
    }

    /* New element was successfully pushed to the stack */
    return STACK_SUCCESS;
}

void stack_pop(stack* stack_pointer){
    if (stack_pointer == NULL){
        return;
    }

    if (stack_is_empty(stack_pointer)){    /* Check if the stack is empty */
        return;                         /* If so exit the function */
    } else {

        Node* tmp_node_to_pop = stack_pointer->top;
        stack_pointer->top = stack_pointer->top->next;
        stack_pointer->current_size--;

        /* First we free the element memory using the destroy and later the node
           itself */
        stack_pointer->destroy(tmp_node_to_pop->value);
        free(tmp_node_to_pop);
    }
}

elem_t stack_peek(stack* stack_pointer){
    
    if (stack_pointer == NULL){ /* Check if the stack is empty */
        return NULL;                 /* If so return NULL for failure */
    } else if (stack_pointer->top == NULL){
        return NULL;
    }
    /* Return a pointer to last element inserted to the stack */
    return  stack_pointer->top->value;  
}

int stack_size(stack* stack_pointer){
    
    /* Return the amount of numbers currently inside the stack */
    return stack_pointer->current_size;
}

bool stack_is_empty(stack* stack_pointer){

    if (stack_pointer->top == NULL || stack_pointer == NULL){ /* If there is no top node */
        return true;                 /* will return true */
    }

    return false;
}

int stack_capacity(stack* stack_pointer){
    
    if (stack_pointer == NULL){ /* Check if the stack is empty */
        return 0;                       /* If so return NULL for failure */
    } else {
        return (stack_pointer->max_size - stack_pointer->current_size);
    }
}

void stack_print(stack* stack_pointer){
    
    if (stack_pointer == NULL){
        return;
    }

    Node* tmp_node_to_check = stack_pointer->top;

    while (tmp_node_to_check){ /* While current node to print isn't NULL */

        /* Print value of the stack element using the user print function */
        stack_pointer->print(tmp_node_to_check->value); 
        tmp_node_to_check = tmp_node_to_check->next; 
    }
}



