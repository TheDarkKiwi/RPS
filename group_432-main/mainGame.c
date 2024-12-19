/* Authors: Jack Edwards and Conor McNeill */
/* Rock, Paper, Scissors game */

/* helper files */
#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "font.h"
#include "../../fonts/font3x5_1.h"
#include <stdbool.h>
#include <stdio.h>
#include "ir_uart.h"
#include "led.h"
#include "mainGame.h"

void display_array(uint8_t (*bitmap)[5][7]) {

    /* Called using display_array(&config3_bitmap_pos); */
    /* Display bitmap */

    tinygl_clear();

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 7; y++) {
            if ((*bitmap)[x][y] == 1) {
                tinygl_draw_point(tinygl_point(x, y), 1);
            }
        }
    }
    tinygl_update();
}

uint8_t (*choose_array(void))[5][7] {
    /* scroll through and select player choice  */
    uint8_t i = 0;

    while (1) {
        pacer_wait();
        navswitch_update();
        
        if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            /*Wrap around to the beginning when it reaches the end*/ 
            i = (i + 1) % 3; 
        }
        if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            /* Wrap around to the end when it reaches the beginning */
            i = (i + 2) % 3; 
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            /* Select configuration */
            player_selected_array_num = i;
            break; 
        }
        /* display current configuration */
        display_array(config_arrays[i]);
    }
    return 0;
}


void txt_init(void) 
{
    /* initalise tinygl */
    tinygl_init (300);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (25);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}

void txt_display_push(char* text)
{
    /* Define polling rate in Hz.  Loop rate*/
    /* Define text update rate (characters per 10 s). Message rate*/
    /* display scroll text and break on push */

    txt_init();
    tinygl_text (text);

    /* initalise player score text */
    char combined_string[30];
    sprintf(combined_string, "P1: %d - P2: %d", player1_score, player2_score);

    while (1)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update (); 

        /* break condition */
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            tinygl_clear();
            break;
        } 

        if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            /* display player scores */
            tinygl_text(combined_string);
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            /* reset to previous text */
            tinygl_text (text);
        }
    }
    
}


int game(void) 
{
    /* singular game of RPS */
    bool gameover = false;
    while (gameover == false) {


        /* select rock paper or scissors */
        choose_array();
        uint8_t ticks = 0;

        while (state == player1){
            /* send choice, recieve game outcome from player 2, display outcome */
            pacer_wait();

            /* send my choice - +B to put into range - used to read */
            ir_uart_putc(player_selected_array_num + 'B');

            /* get and print outcome from player 2 */
            if (ir_uart_read_ready_p()) {
                outcome_player1_temp = ir_uart_getc();
                /* range of outputs */
                if (outcome_player1_temp > 'M' && outcome_player1_temp < 'Q') {
                    while(ticks < 10) {
                        ir_uart_putc(output_recieve);
                        pacer_wait();
                        ticks++;
                    }
                    outcome_player1 = outcome_player1_temp;
                    if (outcome_player1 == draw) {
                        led_set(LED1,0);
                        txt_display_push("DRAW - PRESS DOWN FOR NEXT ROUND");
                        gameover = true;
                        break;
                    } else if (outcome_player1 == win) {
                        player1_score++;
                        led_set(LED1,1);
                        txt_display_push("WIN - PRESS DOWN FOR NEXT ROUND");
                        gameover = true;
                        break;
                    } else if (outcome_player1 == lose) {
                        player2_score++;
                        led_set(LED1,0);
                        txt_display_push("LOSE - PRESS DOWN FOR NEXT ROUND");
                        gameover = true;
                        break;
                    }
                }   
                
            }  

        }

        while (state == player2) {

            /* recieve player 1 choice, compare choices, send outcome to player 1, display outcome */
            /* P is a random check */        

            pacer_wait();
            if (ir_uart_read_ready_p()) {
                outcome_player2_temp = ir_uart_getc();
                /* range for selected */
                if (outcome_player2_temp > 'A' && outcome_player2_temp < 'E') {
                    /* game outcome comparisons */
                    outcome_player2 = outcome_player2_temp;

                    if (player_selected_array_num == ((outcome_player2 - 1) % 3) ) {
                        /* player 1 win - player 2 lose */
                        while (ticks < 10) {
                            /* sending check */
                            if (ir_uart_read_ready_p()) {
                                output_send_check = ir_uart_getc();
                            } 
                            if (output_send_check == 'V') {
                                break;
                            }
                            ir_uart_putc(win);
                            pacer_wait();
                            ticks++;
                        }
                        /* updates score and ends round */
                        player1_score++;
                        led_set(LED1,0);
                        txt_display_push("LOSE - PRESS FOR NEXT ROUND");
                        gameover = true;
                        break;
                    } else if (player_selected_array_num == ((outcome_player2 - 2) % 3)) {
                        while (ticks < 10) {
                            /* recieve signal for set ticks */
                            if (ir_uart_read_ready_p()) {
                                output_send_check = ir_uart_getc();
                            } 
                            if (output_send_check == 'V') {
                                break;
                            }
                            ir_uart_putc(lose);
                            pacer_wait();
                            ticks++;
                        }
                        player2_score++;
                        led_set(LED1,1);
                        txt_display_push("WIN - PRESS DOWN FOR NEXT ROUND");
                        gameover = true;
                        break;

                    } else if (player_selected_array_num == ((outcome_player2) % 3)) {
                        while (ticks < 10) {
                            /* recieve signal for set ticks */
                            if (ir_uart_read_ready_p()) {
                                output_send_check = ir_uart_getc();                   
                            } 
                            if (output_send_check == 'V') {
                                break;
                            }
                            ir_uart_putc(draw);
                            /* send outcome to other player */
                            pacer_wait();
                            ticks++;
                        }
                        led_set(LED1,0);
                        txt_display_push("DRAW - PRESS DOWN FOR NEXT ROUND");
                        gameover = true;
                        break;
                    }
                }   
            }  
        }
    }
    return 0;
}


int main (void)
{    
    /* main operating function */
    uint8_t gameover_delay = 50;
    uint8_t gameover_ticks = 0;
    bool ready = false;

    /* initalise functions */
    ir_uart_init();
    led_init();
    led_set(LED1, 0);
    system_init ();
    pacer_init (400);
    navswitch_init();

    /* Rock is 1, Paper is 2, Scissors is 3 */
    txt_display_push("ROCK, PAPER, SCISSORS - PRESS DOWN TO PLAY");

    /* assign player 1 and player 2 */
    uint8_t pushed = 0;
    state = unassigned;

    while (ready == false) {
        /* sync controllers by sending and recieveing Z */
        ir_uart_putc('Z');
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 'Z') {
                ready = true;
            }
        }
    }

    /* delay to make sure Z character is not picked up */
    uint8_t tick = 0 ;
    while (tick < 10) {
        pacer_wait();
        tick++;
    }
    
    txt_init();
    tinygl_text("READY?");

    while (pushed == 0) {
        /* display ready text, wait for button to be pushed */
        pacer_wait();
        navswitch_update();
        tinygl_update();
            

        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 1) {
                /* recieved 1 so make controller player 2 */
                state = player2;
                pushed = 1;
            }
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            /* on push send 1 */
            ir_uart_putc(1);
            if (state == player1) {
                /* check if sent at same time */
                state = player2;
            } else {
                /* set player state to player 1 as they have mot recieved 1 */
                state = player1;

            } 
            pushed = 1;
        }
    }
    
    while(1)
    {
        /* main game routine */
        /* initalise game */
        gameover_ticks = 0;
        outcome_player1_temp = 0;
        outcome_player2_temp = 0;
        outcome_player1 = 0;
        outcome_player2 = 0;
        
        output_send_check = 'P';
        output_recieve = 'V';
        ready = false;

        game();
        
        while (ready == false) {
            /* sync controllers by sending and recieveing Z */
            ir_uart_putc('Z');
            if (ir_uart_read_ready_p()) {
                if (ir_uart_getc() == 'Z') {
                    ready = true;
                }
            }
        }
        /* game over, reset without initial config */
        while(gameover_ticks < gameover_delay) {
            pacer_wait();
            gameover_ticks++;
        }
    }
}