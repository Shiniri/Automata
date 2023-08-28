
#include "automata.h"


/*
*   Constructs a new DFSA transition, which still has to be added to a state via
*   dfsa_state_add_transition.
*/
DFSA_TRANS * dfsa_trans_constructor(DFSA_STATE * new_destination_state, char new_trigger_character)
{
    DFSA_TRANS * new_transition = malloc(sizeof(DFSA_TRANS));
    if (!new_transition)
    {
        return NULL;
    }

    new_transition->destination_state = new_destination_state;
    new_transition->trigger_character = new_trigger_character;

    return new_transition;
}


/*
*   Constructs a new empty DFSA state. Also needs to be manually added to DFSA with
*   dfsa_add_state.
*/
DFSA_STATE * dfsa_state_constructor(int new_accept_state, unsigned int new_identifier)
{
    DFSA_STATE * new_state = malloc(sizeof(DFSA_STATE));
    if (!new_state)
    {
        return NULL;
    }

    new_state->accept_state = new_accept_state;
    new_state->identifer = new_identifier;
    new_state->transition_count = 0;

    return new_state;
}


/*
*   Adds a preexisting DFSA transition to a preexisting DFSA state.
*/
int dfsa_state_add_trans(DFSA_STATE * state, DFSA_TRANS * transition)
{
    state->transition_count++;
    state->transitions = realloc(state->transitions, sizeof(DFSA_TRANS) * state->transition_count);
    if (!state->transitions)
    {
        return 0;
    }

    state->transitions[state->transition_count - 1] = transition;

    return 1;
}


/*
*   Constructs a DFSA with only one state as initial state.
*/
DFSA * dfsa_constructor()
{
    // Memory for DFSA itself
    DFSA * new_dfsa = malloc(sizeof(DFSA));
    if (!new_dfsa)
    {
        return NULL;
    }

    // Parameters
    new_dfsa->initial_state = dfsa_state_constructor(0, 1);
    new_dfsa->state_count = 1;
    
    // Add initial state to state array
    new_dfsa->states = malloc(sizeof(DFSA_STATE));
    if (!new_dfsa->states)
    {
        return NULL;
    }
    new_dfsa->states[0] = new_dfsa->initial_state;

    return new_dfsa;
}


/*
*   Adds a preexisting DFSA state to a preexisting DFSA.
*/
int dfsa_add_state(DFSA * automaton, DFSA_STATE * state)
{
    automaton->state_count++;
    automaton->states = realloc(automaton->states, sizeof(DFSA_STATE) * automaton->state_count);
    if (!automaton->states)
    {
        return 0;
    }

    automaton->states[automaton->state_count - 1] = state;

    return 1;
}


/*
*   Self explanatory I believe...
*/
void dfsa_destructor(DFSA * automaton)
{
    // Free memory associated with all states pointed to by state array
    for (int state_index = 0; state_index < automaton->state_count; state_index++)
    {
        // Free memory associated with all transitions pointed to by transition array
        for (int trans_index = 0; trans_index < automaton->states[state_index]->transition_count; trans_index++)
        {
            free(automaton->states[state_index]->transitions[trans_index]);
        }
        free(automaton->states[state_index]);
    }

    // Free automaton itself
    free(automaton);
}


/*
*   Visualizes the automaton which is passed to the function.
*/
void dfsa_print(DFSA * automaton)
{
    printf("--------------- AUTOMATON DATA ---------------\n");
    for (int state_index = 0; state_index < automaton->state_count; state_index++)
    {
        printf(
            "\t State-ID: %u Accept: %i\n", 
            automaton->states[state_index]->identifer,
            automaton->states[state_index]->accept_state
        );
        for (int transition_index = 0; transition_index < automaton->states[state_index]->transition_count; transition_index++)
        {
            printf
            (
                "\t\t %u -- %c --> %u\n", 
                automaton->states[state_index]->identifer, 
                automaton->states[state_index]->transitions[transition_index]->trigger_character, 
                automaton->states[state_index]->transitions[transition_index]->destination_state->identifer
            );
        }
    }
    printf("--------------- AUTOMATON DATA ---------------\n");
}