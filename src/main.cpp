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

    // Init variables
    float c_angle = 0.0f;
    float c_fov = 90.0f;

    // Graphics loop
    while (true) {
        // Draw window border
        draw_box(stdscr,0,0,w_width,w_height);

        // Print terminal size
        mvprintw(1, 1, "The overall resolution is %dx%d", w_width, w_height);

        // Go through each column of the screen
        for (int x=0; x<w_width; x++) {
            float angle = ((c_fov/-2.0f)+c_angle + (c_fov/w_width) * x);
            
            char buffer[3]={0};
            sprintf(buffer, "%d", (int)angle);

            for (int y=0; y<w_height; y++) {mvprintw(y,x,buffer);}

            refresh();
            usleep(1000000/2);
        }

        // Display update
        refresh();

        // Delay
        usleep(1000000/30); // 30fps
    }

    // End program
    endwin();

    // Exit Successfully
    return 0;
}
