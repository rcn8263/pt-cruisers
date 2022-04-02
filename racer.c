/// 
/// File Name: racer.c
///	Description: Creation of a racer and ability for them to race.
///
/// @author Ryan Nowak rcn8263
/// 

#define _DEFAULT_SOURCE

#include <pthread.h>
#include "racer.h"

long max_delay = DEFAULT_DELAY;

/// Do setup work for all racers at the start of the program.
void init_racers( long milliseconds ) {
    max_delay = milliseconds;
}

/// Create a new racer.
Racer *make_racer( char *name, int position ) {
    Racer *racer = malloc(sizeof(struct Racer_s));
    
    racer->dist = 0;
    racer->row = position;
    
    racer->graphic = malloc(sizeof(char) * MAX_CAR_LEN);
    strcat(racer->graphic, "~O=");
    strcat(racer->graphic, name);
    for (int i = 0; i < MAX_NAME_LEN - strlen(name); i++) {
        strcat(racer->graphic, "-");
    }
    strcat(racer->graphic, "-o>");
    
    return racer;
}

/// Destroy all dynamically allocated storage for a racer.
void destroy_racer( Racer *racer ) {
    free(racer->graphic);
    free(racer);
}

/// Run one racer in the race.
void *run( void *racer ) {
    Racer *p_racer = racer;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    //race continues until racer gets a flat tire or they finish
    while (1) {
        int rdelay = rand() % max_delay;
        usleep(rdelay * 1000);
         
        if (rdelay <= 3) {
            //racer gets a flat tire
            set_cur_pos(p_racer->row, p_racer->dist + 1);
            put('X');
            break;
        }
        
        
        pthread_mutex_lock(&lock);
        
        //deletes the racer's car from the display
        set_cur_pos(p_racer->row, p_racer->dist);
        for (int i = 0; i < MAX_CAR_LEN; i++) {
            put(' ');
        }
        
        //draws the finish line
        set_cur_pos(p_racer->row, FINISH_LINE);
        put('|');
        
        //draws the racer's new car position to the display
        p_racer->dist++;
        set_cur_pos(p_racer->row, p_racer->dist);
        for (int i = 0; i < MAX_CAR_LEN; i++) {
            put(p_racer->graphic[i]);
        }
        
        pthread_mutex_unlock(&lock);
        
        //racer crosses the finish line
        if (p_racer->dist >= FINISH_LINE) {
            break;
        }
        
    }
}