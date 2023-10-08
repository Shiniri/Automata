
#ifndef AUTOMATA
#define AUTOMATA

#include <stdlib.h>
#include <stdio.h>


/*------------*/
/*  Structs   */
/*------------*/

typedef struct DFSA_TRANSITION DFSA_TRANS;
typedef struct DFSA_STATE DFSA_STATE;
typedef struct DFSA DFSA;

typedef struct DFSA_TRANSITION
{
    char trigger_character;
    DFSA_STATE * destination_state;
} DFSA_TRANS;


typedef struct DFSA_STATE
{
    int accept_state;
    unsigned int identifer;
    unsigned int transition_count;
    DFSA_TRANS ** transitions;
} DFSA_STATE;


typedef struct DFSA
{
    unsigned int state_count;
    DFSA_STATE ** states;
    DFSA_STATE * initial_state;
} DFSA;


// to-do: do proper error handling
enum ERROR_CODES
{
    DFSA_MEMORY_FAILURE = NULL;
};


/**-------------*/
/*  Functions   */
/*--------------*/


// Construct & Destruct & Visualize //

DFSA_TRANS * dfsa_trans_constructor(DFSA_STATE * new_destination_state, char new_trigger_character);

DFSA_STATE * dfsa_state_constructor(int new_accept_state);
int dfsa_state_add_trans(DFSA_STATE * state, DFSA_TRANS * transition);

DFSA * dfsa_constructor();
DFSA * dfsa_copy_constructor(DFSA * other_automaton);
int dfsa_add_state(DFSA * automaton, DFSA_STATE * state);

void dfsa_destructor(DFSA * automaton);

void dfsa_print(DFSA * automaton);


// Operations //

DFSA * dfsa_union(DFSA * automaton_1, DFSA * automaton_2);
DFSA * dfsa_concatenation(DFSA * automaton_1, DFSA * automaton_2);


#endif