#include <iostream>
#include <ncurses.h>
#include "./ascii_box.hpp"
#include <unistd.h>

// Main function
int main() {
    // Initialize ncurses
    setlocale(LC_ALL, ""); // Needed for extended ASCII
    initscr(); // Init screen
    cbreak(); // Live user input (could use raw())
    noecho(); // Stops user input appearing
    keypad(stdscr, TRUE); // Enable FN & arrow keys etc
    curs_set(0); // Make the terminal cursor invisible

    // Get terminal resolution
    int w_height, w_width;
    getmaxyx(stdscr, w_height, w_width);

    // Make box and make it go
    int dir = 1;
    int x = 1;

    // Graphics loop
    while (true) {
        // Draw window border
        draw_box(stdscr,0,0,w_width,w_height);

        // Print terminal size
        mvprintw(1, 1, "The overall resolution is %dx%d", w_width, w_height);

        // Draw box
        if (x+9==w_width-1) {dir=-1;}
        if (x==1) {dir=1;}
        draw_box(stdscr, x, 5, 9, 3);
        mvprintw(6,x+1,"Box! c:");

        // Display update
        refresh();

        // Clean up the box
        if (dir == 1) {
            mvaddch(5,x,' ');
            mvaddch(6,x,' ');
            mvaddch(7,x,' ');
        } else {
            mvaddch(5,x+9,' ');
            mvaddch(6,x+9,' ');
            mvaddch(7,x+9,' ');
        }

        // Move the box
        x+=dir;

        // Delay
        usleep(1000000/30); // 30fps
    }

    // End program
    endwin();

    // Exit Successfully
    return 0;
}
