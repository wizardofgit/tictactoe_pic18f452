# Project
The project aims to build a tic-tac-toe game in C using the PIC18F452 microcontroller.

# The game
<p> The status of the games is displayed on 9 RGB LEDs - each LED corresponding to a cell in a 3x3 grid.
Two players place their symbols (i.e. coloring the LEDs) on the grid. </p> 

<p> To choose a cell use buttons (RC0-RC2).
Firstly choose the row (RC0 - first row, RC1 - second row, RC2 - third row) and then choose the column (respectively).
Which button was chosen is displayed temporarily on the LED strip with PORTB (first bit for RC0 and so on). </p>

<p> Then the corresponding LED will turn on with the color of the player - red for player 1 and green for player 2.
Two LEDs connected to PORTE represent which player now plays - the first bit is player 1 and the second bit is player 2. </p>

<p> The score (stored separately for each player) is displayed on the LED strip - the first 4 bits for player 1 and the second 4 for player 2. </p>

# PICSIMLAB workplace
This project includes a workplace file for PICSIMLAB named `picsimlabworkspace.pzw`.

# The .hex file
This project includes a hex file named `tictactoe.X.production.hex`.
