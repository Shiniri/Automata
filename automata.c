
#include "automata.h"


/*--------------------------------------*/
/*  Construct & Destruct & Visualize    */
/*--------------------------------------*/


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
*   dfsa_add_state. Note that its identifier is always 0 to start, because it gets
*   updated once it gets added to an automaton.
*/
DFSA_STATE * dfsa_state_constructor(int new_accept_state)
{
    DFSA_STATE * new_state = malloc(sizeof(DFSA_STATE));
    if (!new_state)
    {
        return NULL;
    }

    new_state->accept_state = new_accept_state;
    new_state->identifer = 0;
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
    new_dfsa->initial_state = dfsa_state_constructor(0);
    new_dfsa->initial_state->identifer = 1;
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
*   Deep copy of automata. (Nontrivial)
*/
DFSA * dfsa_copy_constructor(DFSA * other_automaton)
{
    DFSA * new_automaton = dfsa_constructor();
    if (new_automaton == NULL)
    {
        return NULL;
    }

    // Copy states without transitions
    for (int state_index = 1; state_index < other_automaton->state_count; state_index++)
    {
        DFSA_STATE * new_state = (DFSA_STATE *)malloc(sizeof(DFSA_STATE));
        if (new_state == NULL)
        {
            dfsa_destructor(new_automaton);
            return NULL;
        }

        new_state->accept_state = other_automaton->states[state_index]->accept_state;
        new_state->identifer = other_automaton->states[state_index]->identifer;
        new_state->transition_count = 0;

        dfsa_add_state(new_automaton, new_state);
    }

    // Copy transitions
    for (int state_index = 0; state_index < other_automaton->state_count; state_index++)
    {
        for (int transition_index = 0; transition_index < other_automaton->states[state_index]->transition_count; transition_index++)
        {
            char trigger_character = other_automaton->states[state_index]->transitions[transition_index]->trigger_character;
            unsigned int destination_identifier = other_automaton->states[state_index]->transitions[transition_index]->destination_state->identifer;

            // Find the corresponding destination state in the new automaton
            DFSA_STATE * destination_state = NULL;
            for (int state_index = 0; state_index < new_automaton->state_count; state_index++)
            {
                if (new_automaton->states[state_index]->identifer == destination_identifier)
                {
                    destination_state = new_automaton->states[state_index];
                    break;
                }
            }

            // Create a new transition and link it to the corresponding states
            DFSA_TRANS * new_transition = dfsa_trans_constructor(destination_state, trigger_character);
            if (new_transition == NULL || !dfsa_state_add_trans(new_automaton->states[state_index], new_transition))
            {
                dfsa_destructor(new_automaton);
                return NULL;
            }
        }
    }

    return new_automaton;
}



/*
*   Adds a preexisting DFSA state to a preexisting DFSA.
*   Its identifier is updated, to always correspond with the
*   state count of the automaton.
*/
int dfsa_add_state(DFSA * automaton, DFSA_STATE * state)
{
    automaton->state_count++;
    automaton->states = realloc(automaton->states, sizeof(DFSA_STATE) * automaton->state_count);
    if (!automaton->states)
    {
        return 0;
    }

    state->identifer = automaton->state_count;
    automaton->states[automaton->state_count - 1] = state;

    return 1;
}


/*
*   Destroys an automaton including all memory associated with it.
*   Watch out for other ways you might reference this memory!
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


/*--------------*/
/*  Operations  */
/*--------------*/

/*
*   DFSA-Union: New initial state, to initial states of two automata and all 
*   accept states of said automata to new accept state.
*
*   Note that this does not create a deep copy of the two input automata,
*   but rather makes them part of thew new one! Meaning you can break this
*   automaton by destroying either automaton 1 or 2.
*/
DFSA * dfsa_union(DFSA * automaton_1, DFSA * automaton_2)
{
    // Create a new automaton with an initial state
    // also create deep copes of automaton_1 and automaton_2
    DFSA * new_automaton = dfsa_constructor();
    DFSA * automaton_1_copy = dfsa_copy_constructor(automaton_1);
    DFSA * automaton_2_copy = dfsa_copy_constructor(automaton_2);

    // Add the states of automaton 1 copy to the new automaton
    for (int state_index = 0; state_index < automaton_1_copy->state_count; state_index++)
    {
        if (!dfsa_add_state(new_automaton, automaton_1_copy->states[state_index]))
        {
            return NULL;
        }
    }

    // Same for automaton 2 copy
    for (int state_index = 0; state_index < automaton_2_copy->state_count; state_index++)
    {
        if (!dfsa_add_state(new_automaton, automaton_2_copy->states[state_index]))
        {
            return NULL;
        }
    }

    // Connect the new initial state to the (ex-)initial states of the newly added automata
    DFSA_TRANS * to_automaton_1 = dfsa_trans_constructor(new_automaton->states[1], '\0');
    DFSA_TRANS * to_automaton_2 = dfsa_trans_constructor(new_automaton->states[automaton_1_copy->state_count + 1], '\0');
    if (!dfsa_state_add_trans(new_automaton->initial_state, to_automaton_1) | !dfsa_state_add_trans(new_automaton->initial_state, to_automaton_2))
    {
        return NULL;
    }

    return new_automaton;
}


/*
*   DFSA-Concatenation: All accept states of first automaton connect to initial state of second
*/
DFSA * dfsa_concatenation(DFSA * automaton_1, DFSA * automaton_2)
{
    // New automaton contains automaton 1
    DFSA * new_automaton = dfsa_copy_constructor(automaton_2);

    // Add to that states of automaton 2
    for (int state_index = 0; state_index < automaton_2->state_count; state_index++)
    {
        if (!dfsa_add_state(new_automaton, automaton_2->states[state_index]))
        {
            return NULL;
        }
    }

    // Connect accept states of automaton 1 to initial state of automaton 2
    for (int state_index = 0; state_index < (new_automaton->state_count - automaton_2->state_count); state_index++)
    {
        if (new_automaton->states[state_index]->accept_state)
        {
            DFSA_TRANS * new_transition = dfsa_trans_constructor(automaton_2->initial_state, '\0');
            if ((new_transition == NULL) | (!dfsa_state_add_trans(new_automaton->states[state_index], new_transition)))
            {
                return NULL;
            }
        }
    }

    return new_automaton;
}


/*
*   DFSA Kleene Closure: connect all accept states of automaton back to its initial state
*   via epsilon-transition
*/
DFSA * dfsa_kleene_closure(DFSA * automaton)
{
    DFSA * new_automaton = dfsa_copy_constructor(automaton);
    for (int state_index = 0; state_index < new_automaton->state_count; state_index++)
    {
        if (new_automaton->states[state_index]->accept_state == 1)
        {
            DFSA_TRANS * new_transition = dfsa_trans_constructor(new_automaton->initial_state, '\0');
            if ((new_transition == NULL) | !dfsa_state_add_trans(new_automaton->states[state_index], new_transition))
            {
                dfsa_destructor(new_automaton);
                return NULL;
            }
        }
    }

    return new_automaton;
}


/*----------*/
/*  Regex   */
/*----------*/


/*
*   A minimal implementation of an algorithm constructing DFSAs from a given
*   regex expression. Does not handle most cases usually handled by regex libraries,
*   such as encapsulations / character classes.
*   Handles each character which is not reserved as basic operand.
*/
DFSA * myt_regex_to_dfsa(const char * regex) 
{
    if (!regex || *regex == '\0') 
    {
        // Empty regex, return an automaton that accepts only the empty string
        DFSA * automaton = dfsa_constructor();
        DFSA_STATE * single_state = dfsa_state_constructor(1);
        if (automaton == NULL | single_state == NULL)
        {
            printf("Memory error during allocation.\n");
            return NULL;
        }

        if(!dfsa_add_state(automaton, single_state))
        {
            printf("Failed to add state.\n");
            return NULL;
        }

        DFSA_TRANS * single_transition = dfsa_trans_constructor(single_state, '\0');
        if (single_transition == NULL)
        {
            printf("Memory error during allocation.\n");
            return NULL;
        }

        if(!dfsa_state_add_trans(automaton->initial_state, single_transition))
        {
            printf("Failed to add state.\n");
            return NULL;
        }

        return automaton;
    }

    char ch = *regex;

    // If the character is an escape character, advance and treat the next character as a literal
    if (ch == '\\') 
    {
        regex++;
        ch = *regex;
        if (!ch) { return NULL; }
    }

    // Handle union operation
    if (ch == '|') 
    {
        regex++;
        DFSA * left = myt_regex_to_dfsa(regex);
        if (left == NULL)
        {
            printf("Failed to create automaton.\n");
            return NULL;
        }

        while (*regex != '|' && *regex != '\0') { regex++; }
        regex++;  // skip the '|'

        DFSA * right = myt_regex_to_dfsa(regex);
        DFSA * united = dfsa_union(left, right);
        if (right == NULL | united == NULL)
        {
            printf("Failed to create automaton.\n");
            return NULL;
        }

        dfsa_destructor(left);
        dfsa_destructor(right);

        return united;
    }

    // Handle concatenation (implicit)
    if (ch != '(' && ch != '*' && ch != '|') 
    {
        DFSA * left = myt_regex_to_dfsa(regex);
        DFSA * right = myt_regex_to_dfsa(regex + 1);
        if (left == NULL | right == NULL)
        {
            printf("Failed to create automaton.\n");
            return NULL;
        }

        DFSA * concatenated = dfsa_concatenation(left, right);
        if (concatenated == NULL)
        {
            printf("Failed to create automaton.\n");
            return NULL;
        }

        dfsa_destructor(left);
        dfsa_destructor(right);

        return concatenated;
    }

    // Handle Kleene closure
    if (ch == '*') 
    {
        regex++;
        DFSA * automaton = myt_regex_to_dfsa(regex);
        DFSA * closed = dfsa_kleene_closure(automaton);
        dfsa_destructor(automaton);
        return closed;
    }

    // Handle parentheses (grouping) -- for simplicity, assume balanced parentheses
    if (ch == '(') 
    {
        int depth = 1;
        const char * end = regex + 1;
        while (*end && depth > 0) 
        {
            if (*end == '(') depth++;
            else if (*end == ')') depth--;
            end++;
        }

        char sub_regex[end - regex];
        strncpy(sub_regex, regex + 1, end - regex - 2);  // +1 and -2 to exclude the parentheses
        sub_regex[end - regex - 2] = '\0';

        DFSA * automaton = myt_regex_to_dfsa(sub_regex);
        if (automaton == NULL)
        {
            printf("Failed to create automaton.\n");
            return NULL;
        }

        return automaton;
    }

    printf("Error: Unexpected character in regex.\n");
    return NULL;
}