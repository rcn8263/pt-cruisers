/// 
/// File Name: pt-cruiser.c
///	Description: Creates each racer and a thread for each racer to run the race
///
/// @author Ryan Nowak rcn8263
/// 

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "racer.h"
#include "display.h"

/// Checks if given char string is a number
///
/// @param s string to check
/// @return true if it is a number, false otherwise
static int is_number(char *s) {
    int i = 0;

    //checking for negative numbers
    if (s[0] == '-') {
        i = 1;
    }
    for (; s[i] != 0; i++) {
        //if (s[i] > '9' || s[i] < '0')
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

/// creates each racer and a thread for each racer to run the race
int main(int argc, void *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
        return EXIT_FAILURE;
    }
    
    //sets the seed for the random number generator used by the racers
    srand((unsigned) time(NULL));
    
    //sets the maximum delay for the racers
    int max_delay = DEFAULT_DELAY;
    
    //checks if the first argument is an integer and initializes the racers
    int index = 1;
    if (is_number(argv[index])) {
        int delay = strtol(argv[index], NULL, 10);
        index++;
        if (delay > 0) {
            max_delay = delay;
        }
    }
    init_racers(max_delay);
    
    //checks that all of the racer names have a length of 6 characters or less
    for (int i = index; i < argc; i++) {
        if (strlen(argv[i]) > MAX_NAME_LEN) {
            fprintf(stderr, "Error: racer names must not exceed length 6.\n");
            return EXIT_FAILURE;
        }
    }
    
    //checks if there are 2 or more names given by the arguments.
    if (index + 2 > argc) {
        fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
        return EXIT_FAILURE;
    }
    
    //Create the Racer instances, one for each name on the command line.
    int num_racers = argc - index;
    Racer *racers[num_racers];
    for (int i = 0; i < num_racers; i++) {
        racers[i] = make_racer(argv[i+index], i+1);
    }
    
    //clears the terminal window before the race is started
    clear();
    
    //creates a thread for each racer
    pthread_t threads[num_racers];
    for (int i = 0; i < num_racers; i++) {
        if(pthread_create( &threads[i], NULL, run, racers[i] )) {
            fprintf(stderr, "ERROR IN pthread_create");
            return EXIT_FAILURE;
        }
    }
    
    //waits for each thread to finish
    for (int i = 0; i < num_racers; i++) {
        pthread_join(threads[i], NULL);  
    }
    
    
    //frees all dynamically allocated data
    for (int i = 0; i < num_racers; i++) {
        destroy_racer(racers[i]);
    }
    
    //moves the curser to the next line after the race
    set_cur_pos(num_racers + 1, 0);
    
    return EXIT_SUCCESS;
}