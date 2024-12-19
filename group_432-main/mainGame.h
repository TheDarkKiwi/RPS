/* Authors: Jack Edwards and Conor McNeill */
/* Rock, Paper, Scissors game */

#ifndef MAINGAME_H
#define MAINGAME_H

#include "system.h"

/* player assignment */
typedef enum { 
    player1,
    player2,
    unassigned
} player_t;

/* outcome conditions */
typedef enum {
    draw = 'N',
    win,
    lose,
} outcome_t;

/* make state functions global */
outcome_t outcome_player1;
outcome_t outcome_player2;
player_t state;
uint8_t player_selected_array_num;

/* track player score globally */
uint8_t player1_score = 0;
uint8_t player2_score = 0;
uint8_t output_send_check = 'P';
uint8_t output_recieve = 'V';


/* global variables used to transmit outcomes */
char outcome_player1_temp = 0;
char outcome_player2_temp = 0;


/* player choices as bitmaps */
uint8_t config1_bitmap_pos[5][7] =
{
    /* rock */
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 0}
};
uint8_t config2_bitmap_pos[5][7] =
{
    /* paper */
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0}
};
uint8_t config3_bitmap_pos[5][7] =
{
    /* Scissors */
    {0, 1, 0, 0, 0, 1, 0},
    {1, 0, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0}
};

/* configure choices into an array to choose from */
uint8_t (*config_arrays[3])[5][7] = 
{
    &config1_bitmap_pos,
    &config2_bitmap_pos,
    &config3_bitmap_pos
};


#endif //MAINGAME_H 