
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


/**-------------*/
/*  Functions   */
/*--------------*/

DFSA_TRANS * dfsa_trans_constructor(DFSA_STATE * new_destination_state, char new_trigger_character);

DFSA_STATE * dfsa_state_constructor(int new_accept_state, unsigned int new_identifier);
int dfsa_state_add_trans(DFSA_STATE * state, DFSA_TRANS * transition);

DFSA * dfsa_constructor();
int dfsa_add_state(DFSA * automaton, DFSA_STATE * state);
void dfsa_destructor(DFSA * automaton);
void dfsa_print(DFSA * automaton);


#endif