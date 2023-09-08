#include <iostream>
#include <ncurses.h>

// Main function
int main() {
    // Initialize ncurses
    initscr(); // Init screen
    cbreak(); // Live user input (could use raw())
    noecho(); // Stops user input appearing
    keypad(stdscr, TRUE); // Enable FN & arrow keys etc

    printw("Type any charachter to see it in bold: ");
    refresh();

    attron(A_BOLD);
    printw("%c\n", getch());
    attroff(A_BOLD);

    printw("Hello World!");
    refresh();
    getch();
    endwin();

    // Exit Successfully
    return 0;
}
