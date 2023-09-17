#include <iostream>
#include <ncurses.h>
#include "./moller_trumbore.hpp"
#include "./vector_math.hpp"
#include <unistd.h>
#include <cmath>

double temp=0.0;

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

    // Triangle
    double triangle[4][3] = { // v0, v1, v2, normal
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {-1.0, 0.0, 0.0}};

    // Init variables
    double c_angle[2] = {0.0, 0.0}; // (deg) Angle of rotation from x+, (vert, hor)
    double c_fov = 90.0f; // degrees
    double c_pos[3] = {-5.0, 0.0, 0.0}; //(x,y,z)
    double c_rdist = 1.0f; // Render distance

    // Init gradient & lighting_delta (angle between camera and face normal
    double lighting_delta[3] = {0, 0, 0};
    double lighting_dangle = 0.0;
    const char gradient[7] = {'-', '^', '!', '%', '>', '$', '#'};

    // Graphics loop
    while (true) {
        // Get input
        char c = ' ';
        c = getch();
        //usleep(1000);
        if (c=='w') {c_pos[0]+=0.25;}
        if (c=='s') {c_pos[0]-=0.25;}
        if (c=='d') {c_pos[1]+=0.25;}
        if (c=='a') {c_pos[1]-=0.25;}
        if (c=='r') {c_pos[2]+=0.25;}
        if (c=='f') {c_pos[2]-=0.25;}

        if (c=='k') {c_angle[0]+=2.5;}
        if (c=='i') {c_angle[0]-=2.5;}
        if (c=='l') {c_angle[1]+=10.0;}
        if (c=='j') {c_angle[1]-=10.0;}

        // Go through each column of the screen
        for (int x=0; x<w_width; x++) {
            // Get the camera angle step size
            double c_angle_step = c_fov / (double)w_width; // double when working with height (if char is 8x16) <-- assumed

            // Get the horizontal angle of the ray
            double h_angle = ((c_fov/-2.0f)+c_angle[1] + (c_angle_step) * x); // Angle is in degrees
            h_angle = (h_angle*M_PI)/180.0; // Converts angle to radians

            // Initialize ray destination varaibles
            double dest[3];
            double dest_x, dest_y, dest_z;

            // Go through each row of the screen
            for (int y=0; y<w_height; y++) {
                // Get the vertical angle of the ray
                double v_angle = (c_angle_step*-1*(w_height/2)) + c_angle[0] + (c_angle_step * y);
                v_angle *= 2; // x2 because text is twice as tall as wide
                v_angle = (v_angle*M_PI)/180.0; // Converts angle to radians

                // Calculate ray destination
                dest[0]=c_rdist;dest[1]=0;dest[2]=0;          // Initialize dest
                v_ry(dest, v_angle, &dest_x, &dest_y, &dest_z);     // Rotate dest up/down
                dest[0]=dest_x;dest[1]=dest_y;dest[2]=dest_z; // Set new dest
                v_rz(dest, h_angle, &dest_x, &dest_y, &dest_z); // Rotate dest left/right
                dest[0]=dest_x;dest[1]=dest_y;dest[2]=dest_z; // Set new dest

                // Check for intersection, if = #, else = -
                char state[2];
                lighting_delta[0] = (triangle[3][0] + dest[0]) / 2; // x
                lighting_delta[1] = (triangle[3][1] + dest[1]) / 2; // y
                //lighting_delta[2] = (triangle[3][2] + dest[2]) / 2; // z
                lighting_dangle = sqrt(pow(lighting_delta[0],2) + pow(lighting_delta[1],2)) / 1.0; // Adj/Hyp; Adj = sqrt(x^2 + y^2); Hyp = 1
                if (lighting_dangle < 0) {lighting_dangle *= -1;}
                if (lighting_dangle > 9) {lighting_dangle = ((int)lighting_dangle) % 7;}
                if (intersects_triangle(c_pos, dest, triangle[0], triangle[1], triangle[2]) == 1) {sprintf(state,"#");}//state[0]=gradient[7-((int)(lighting_dangle*7)-1)]; state[1]='\0';}
                else {sprintf(state,"-");}
                mvprintw(y,x,state);

                // Debug
                temp = lighting_dangle;
                char debug[256];
                sprintf(debug, "TMP:%f, Ox:%f, Oy:%f, Oz:%f, Ex:%f, Ey:%f, Ez:%f, Ah:%f, Av:%f",temp, c_pos[0], c_pos[1], c_pos[2], dest[0], dest[1], dest[2], c_angle[1], c_angle[0]);
                mvprintw(0,0,debug);

                // Delay
                //usleep(1000000/10000);
            }
        }

        // Display update
        refresh();

        // Delay
        //usleep(1000000/30); // 30fps
    }

    // End program
    endwin();

    // Exit Successfully
    return 0;
}
