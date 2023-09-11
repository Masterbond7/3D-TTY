int draw_box(WINDOW *win, int x, int y, int width, int height) {
    // Print border edges around the screen
    for (int rel_y=0; rel_y<height; rel_y++) {
        // If top/bottom row fill row with ═, otherwise first and last col with ║
        if (rel_y==0 || rel_y==height-1) {
            for (int rel_x=0; rel_x<width; rel_x++) {mvwprintw(win, y+rel_y, x+rel_x, "═");}
        } else {
            mvwprintw(win, y+rel_y,x,"║"); mvwprintw(win, y+rel_y,x+width-1,"║");
        }
        refresh();
    }

    // Print border corners
    mvwprintw(win, y,x,"╔"); mvwprintw(win, y,x+width-1,"╗"); mvwprintw(win, y+height-1,x,"╚"); mvwprintw(win, y+height-1,x+width-1,"╝");

    return 0;
}
