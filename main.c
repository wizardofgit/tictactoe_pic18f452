/*
 * File:   main.c
 * Author: swirk
 *
 * Created on 16 kwietnia 2024, 20:32
 */

#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#pragma config WDT = 0


#include <xc.h>
// #include <htc.h>

// set bit (takes pointer to a PORT and bit number)
void set_bit(volatile unsigned char *reg, unsigned char bit_number) {
    *reg |= (1 << bit_number); // Set the bit at the specified position
}

// clear bit (takes pointer to a PORT and bit number)
void clear_bit(volatile unsigned char *reg, unsigned char bit_number) {
    *reg &= ~(1 << bit_number); // Clear the bit at the specified position
}

// chech if bit is turned on on PORTC
int is_bit_on(unsigned char bit_number) {
    // Mask to isolate the specific bit
    unsigned char mask = 1 << bit_number;
    
    // Perform a bitwise AND operation with PORTC and the mask
    // If the result is non-zero, the bit is turned on
    return (PORTC & mask) != 0;
}

// display temporarily chosen x or y value on PORTB
void debug_position(short x) {
    // Mask to isolate the bit corresponding to the value of x
    unsigned char mask = 0x01 << x;
    
    // Clear all bits of PORTB except for the one corresponding to the value of x
    PORTB &= ~0x07; // Clear bits 0, 1, and 2
    
    // Set the bit corresponding to the value of x
    PORTB |= mask;
}

// wait time in ms
void wait(int x) {
    for(int i = x*1000; i > 0; i--) {
        
    }
    return;
}

// function for drawing the game staus
void draw_grid(short grid[3][3]) {
    // check every cell for coresponding values
    if(grid[0][0]) {
        if(grid[0][0] == 1)
            set_bit(&PORTA, 0);
        else
            set_bit(&PORTA, 1);
    }
    if(grid[0][1]) {
        if(grid[0][1] == 1)
            set_bit(&PORTA, 2);
        else
            set_bit(&PORTA, 3);
    }
    if(grid[0][2]) {
        if(grid[0][2] == 1)
            set_bit(&PORTA, 4);
        else
            set_bit(&PORTA, 5);
    }
    if(grid[1][0]) {
        if(grid[1][0] == 1)
            set_bit(&PORTD, 0);
        else
            set_bit(&PORTD, 1);
    }
    if(grid[1][1]) {
        if(grid[1][1] == 1)
            set_bit(&PORTD, 2);
        else
            set_bit(&PORTD, 3);
    }
    if(grid[1][2]) {
        if(grid[1][2] == 1)
            set_bit(&PORTD, 4);
        else
            set_bit(&PORTD, 5);
    }
    if(grid[2][0]) {
        if(grid[2][0] == 1)
            set_bit(&PORTD, 6);
        else
            set_bit(&PORTD, 7);
    }
    if(grid[2][1]) {
        if(grid[2][1] == 1)
            set_bit(&PORTC, 3);
        else
            set_bit(&PORTC, 4);
    }
    if(grid[2][2]) {
        if(grid[2][2] == 1)
            set_bit(&PORTC, 5);
        else
            set_bit(&PORTC, 6);
    }
    
        
    return;
}

// function to determine if player won; returns 0 if no-one won and 1 if player
// 1 won and 2 if player 2 won and 3 if tie
short check_if_won(short grid[3][3]) {
    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j++)
            if(grid[i][j] != 0) {
                // vertical line
                if(grid[i+1][j] == grid[i][j] && grid[i+2][j] == grid[i][j])
                    return grid[i][j];
                // horizontal line
                if(grid[i][j+1] == grid[i][j] && grid[i][j+2] == grid[i][j])
                    return grid[i][j];
            }
    
    // check for diagonals
    // right diagonal ("backslash")
    if(grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
        return grid[0][0];
    // left diagonal ("forward slash")
    if(grid[2][0] == grid[1][1] && grid[1][1] == grid[0][2])
       return grid[2][0];
    
    // check if tie
    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j++)
            if(grid[i][j] == 0)
                return 0;
    
    // if tie detected
    return 3;
}

void reset_portc(void) {
    // Mask to clear bits 3-7
    unsigned char mask = 0x07; // 00000111
    
    // Clear bits 3-7 on PORTC
    PORTC &= ~mask;
    
    return;
}

void reset_screen(void) {
    //reset_portc();
    PORTC = 0;
    PORTD = 0;
    PORTA = 0;
    
    return;
}

void display_score(short short1, short short2) {
    // Mask to isolate bits 0-3 of PORTB
    unsigned char mask1 = 0x0F;
    // Mask to isolate bits 4-7 of PORTB
    unsigned char mask2 = 0xF0;
    
    // Clear bits 0-3 of PORTB and then set them according to short1
    PORTB &= ~mask1;
    PORTB |= (short1 & mask1);
    
    // Clear bits 4-7 of PORTB and then set them according to short2
    PORTB &= ~mask2;
    PORTB |= (short2 << 4) & mask2; // Shift short2 by 4 bits to the left to align with bits 4-7
    
    return ;
}

void main(void) {
    // turn off watchdogtimer
    // WDTCONbits.SWDTEN = 0;
    
    // PORTE as output for current player playing - 0 indicating first player
    // and 1 indicating second player
    TRISE = 0;
    
    // PORTC as input - 0,1,2 to select the row and then the column
    // and bits 3,4,5,6 for showing cells (2,1), (2,2)
    TRISC = 0b00000111;
    
    // PORTA for showing cells (0,0), (0,1), (0,2)
    TRISA = 0;
    
    // PORTD for showing cells (1,0), (1,1), (1,2), (2,0)
    TRISD = 0;
    
    // PORTB for showing the score for player 1 and 2
    TRISB = 0;
    
    short score_player_1 = 0;
    short score_player_2 = 0;
    
    // initialize game grid, where 1 - x, 2 - circle, 0 - not used
    short grid[3][3];
    
    // which player is now playing
    short player_turn = 1;
    
    // zero out the gamemap
    for(short i = 0; i < 3; i++)
        for(short j = 0; j < 3; j++)
            grid[i][j] = 0;
    
    // game main loop
    while(1){
        if(check_if_won(grid) == 1) {
            score_player_1++;
            reset_screen();
            player_turn = 1;
            
            for(short i = 0; i < 3; i++)
                for(short j = 0; j < 3; j++)
                    grid[i][j] = 0;
        }
        else if(check_if_won(grid) == 2) {
            score_player_2++;
            reset_screen();
            player_turn = 1;
            
            for(short i = 0; i < 3; i++)
                for(short j = 0; j < 3; j++)
                    grid[i][j] = 0;
        }
        else if(check_if_won(grid) == 3) {
            reset_screen();
            player_turn = 1;
            
            for(short i = 0; i < 3; i++)
                for(short j = 0; j < 3; j++)
                    grid[i][j] = 0;
        }
        
        draw_grid(grid);
        display_score(score_player_1, score_player_2);
        
        
        // indicate which player plays now
        PORTE = player_turn;
        
        
        // flag for tracking if player chose valid cell
        short position_chosen = 1;
        
        while(position_chosen) {
            // x, y coordinates for putting symbol on map
            short x = -1;
            short y = -1;

            // choose the row
            while(x < 0) {
                wait(500);
                if(is_bit_on(0))
                    x = 0;
                else if(is_bit_on(1))
                    x = 1;
                else if(is_bit_on(2))
                    x = 2; 
            }

            // for debugging purposes
            debug_position(x);
            
            wait(1000);

            while(y < 0) {
                wait(500);
                if(is_bit_on(0))
                    y = 0;
                else if(is_bit_on(1))
                    y = 1;
                else if(is_bit_on(2))
                    y = 2;            
            }
            // for debugging purposes
            debug_position(y);
            
            wait(1000);

            // if cell clear, overwrite
            if(grid[x][y] == 0) {
                grid[x][y] = player_turn;

                // let other player play
                if(player_turn == 1)
                    player_turn = 2;
                else
                    player_turn = 1;

                position_chosen = 0;
            }
        }
    }
    return;
}
