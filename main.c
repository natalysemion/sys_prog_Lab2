#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_ALPHABET_SIZE 26

struct Transition {
    int from_state;
    char input_symbol;
    int to_state;
};

struct Automaton {
    int alphabet_size;
    char alphabet[MAX_ALPHABET_SIZE];
    int num_states;
    int states[MAX_STATES];
    int initial_state;
    int num_final_states;
    int final_states[MAX_STATES];
    struct Transition transitions[MAX_STATES * MAX_ALPHABET_SIZE];
    int num_transitions;
};

bool is_final_state(const struct Automaton automaton, int state) {
    for (int i = 0; i < automaton.num_final_states; i++) {
        if (automaton.final_states[i] == state) {
            return true;
        }
    }
    return false;
}

int get_transition_state(const struct Automaton automaton, int current_state, char input_symbol) {
    for (int i = 0; i < automaton.num_transitions; i++) {
        if (automaton.transitions[i].from_state == current_state &&
            automaton.transitions[i].input_symbol == input_symbol) {
            return automaton.transitions[i].to_state;
        }
    }
    return -1;
}

bool dfs(const struct Automaton automaton, int current_state, int to_state, char input_symbol, bool visited[]) {
    if (current_state == to_state) {
        return true; // Path found
    }

    visited[current_state] = true;

    for (int i = 0; i < automaton.num_transitions; i++) {
        if (!visited[automaton.transitions[i].to_state] &&
            automaton.transitions[i].from_state == current_state &&
            automaton.transitions[i].input_symbol == input_symbol) {
            if (dfs(automaton, automaton.transitions[i].to_state, to_state, input_symbol, visited)) {
                return true; // Path found
            }
        }
    }

    return false; // No path found
}

int can_transition(const struct Automaton automaton, int from_state, int to_state, char input_symbol) {
    bool *visited = (bool*)malloc(automaton.num_states * sizeof(bool));
    if (visited == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < automaton.num_states; i++) {
        visited[i] = false;
    }

    return dfs(automaton, from_state, to_state, input_symbol, visited);
}

bool is_word_accepted(const struct Automaton automaton, const char* word) {
    int current_state = automaton.initial_state;
    for (int i = 0; word[i] != '\0'; i++) {
        current_state = get_transition_state(automaton, current_state, word[i]);
        printf("%d ", current_state);
        if (current_state == -1) {
            return false; // Word is not accepted
        }
    }
    return is_final_state(automaton, current_state);
}
bool is_word_accepted2(const struct Automaton automaton, const char* word, int* state) {
    int current_state = *state;
    for (int i = 0; word[i] != '\0'; i++) {
        current_state = get_transition_state(automaton, current_state, word[i]);
        if (current_state == -1) {
            return false; // Word is not accepted
        }
    }

    *state = current_state;
    return true;
}

void displayAutomaton(const struct Automaton automaton) {
    printf("Alphabet size: %d\n", automaton.alphabet_size);
    printf("Alphabet:\n");
    for(int i = 0; i < automaton.alphabet_size; i++){
         printf("%c ", automaton.alphabet[i]);
    }
    printf("\n");
    printf("Number of states: %d\n", automaton.num_states);
    printf("States:\n");
    for(int i = 0; i < automaton.num_states; i++){
        printf("%d ", automaton.states[i]);
    }
    printf("\n");
    printf("Initial state: %d\n", automaton.initial_state);
    printf("Number of final states: %d\n", automaton.num_final_states);

    printf("Final states: ");
    for (int i = 0; i < automaton.num_final_states; i++) {
        printf("%d ", automaton.final_states[i]);
    }
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < automaton.num_transitions; i++) {
        printf("%d --(%c)--> %d\n", automaton.transitions[i].from_state, automaton.transitions[i].input_symbol, automaton.transitions[i].to_state);
    }
}

int main() {
    char filename[] = "input1.txt";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    struct Automaton automaton;
    automaton.num_transitions = 0;

    fscanf(file, "%d %d %d %d", &automaton.alphabet_size, &automaton.num_states, &automaton.initial_state, &automaton.num_final_states);

    for (int i = 0; i < automaton.num_final_states; i++) {
        fscanf(file, "%d", &automaton.final_states[i]);
    }

    while (fscanf(file, "%d %c %d", &automaton.transitions[automaton.num_transitions].from_state,
                  &automaton.transitions[automaton.num_transitions].input_symbol,
                  &automaton.transitions[automaton.num_transitions].to_state) == 3) {
        automaton.num_transitions++;
    }

    fclose(file);

    for(int i = 0; i < automaton.num_states; i++){
        automaton.states[i] = i;
    }

    for(int i = 0; i < automaton.alphabet_size; i++){
        automaton.alphabet[i]= 'a' + i;
    }

    displayAutomaton(automaton);

    char w0[100];
    char w1[] = "aaba";
    char w2[] = "bbaa";

    printf("Enter w0: ");
    scanf("%s", w0);

    int current_state = automaton.initial_state;
    bool b = false;
    bool w1_accepted = is_word_accepted2(automaton, w1, &current_state);
    printf("\nw1: %d", w1_accepted);
    printf("\ncurrent_state after w1: %d\n", current_state);
    if (w1_accepted){
        bool w0_accepted = is_word_accepted2(automaton, w0, &current_state);
        printf("\nw0: %d", w0_accepted);
        printf("\ncurrent_state after w0: %d\n", current_state);

        if(w0_accepted){
            bool w2_accepted = is_word_accepted2(automaton, w2, &current_state);
            printf("\nw2: %d", w2_accepted);
            printf("\ncurrent_state after w2: %d\n", current_state);

            if(w2_accepted){
                if(is_final_state(automaton, current_state)){
                    printf("\nThe word w = w1w0w2 is accepted by the automaton.\n");
                    b = true;
                }
            }
        }
    }
    if(!b){
        printf("\nThe word w = w1w0w2 is not accepted by the automaton.\n");
    }


    return 0;
}
