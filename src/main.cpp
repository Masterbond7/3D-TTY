#include <iostream>
#include <ncurses.h>

// Main function
int main() {
    // Initialize ncurses
    initscr(); // Init screen
    cbreak(); // Live user input (could use raw())
    noecho(); // Stops user input appearing
    keypad(stdscr, TRUE); // Enable FN & arrow keys etc

    // Get terminal resolution
    int w_height, w_width;
    getmaxyx(stdscr, w_height, w_width);

    // Print terminal size
    mvprintw(0, 0, "#"); mvprintw(w_height-1, w_width-1, "#");
    mvprintw(1, 1, "Resolution is %dx%d", w_width, w_height);
    refresh();

    // End program on user input
    getch();
    endwin();

    // Exit Successfully
    return 0;
}
