#include <iostream>
#include <ncurses.h>
#include "./moller_trumbore.hpp"
#include "./vector_math.hpp"
#include <unistd.h>
#include <cmath>

double temp=0.0;
double rot=0;

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

    // Make z_buffer
    double zbuf[w_width][w_height];

    // Triangle
    int tris = 12;
    double triangle[12][4][3] = { // v0, v1, v2, normal
   {{ 1.0,  0.0,  0.0},
    { 0.0, -1.0,  0.0},
    { 0.0,  0.0,  0.0},
    { 0.0,  0.0, -1.0}},
   {{ 0.0, -1.0,  0.0},
    { 1.0,  0.0,  0.0},
    { 1.0, -1.0,  0.0},
    { 0.0,  0.0, -1.0}},
   {{ 1.0, -1.0,  1.0},
    { 0.0, -1.0,  0.0},
    { 1.0, -1.0,  0.0},
    { 0.0, -1.0,  0.0}},
   {{ 0.0, -1.0,  0.0},
    { 1.0, -1.0,  1.0},
    { 0.0, -1.0,  1.0},
    { 0.0, -1.0,  0.0}},
   {{ 1.0,  0.0,  0.0},
    { 1.0, -1.0,  1.0},
    { 1.0, -1.0,  0.0},
    { 1.0,  0.0,  0.0}},
   {{ 1.0, -1.0,  1.0},
    { 1.0,  0.0,  0.0},
    { 1.0,  0.0,  1.0},
    { 1.0,  0.0,  0.0}},
   {{ 1.0, -1.0,  1.0},
    { 0.0,  0.0,  1.0},
    { 0.0, -1.0,  1.0},
    { 0.0,  0.0,  1.0}},
   {{ 0.0,  0.0,  1.0},
    { 1.0, -1.0,  1.0},
    { 1.0,  0.0,  1.0},
    { 0.0,  0.0,  1.0}},
   {{ 0.0,  0.0,  1.0},
    { 0.0, -1.0,  0.0},
    { 0.0, -1.0,  1.0},
    {-1.0,  0.0,  0.0}},
   {{ 0.0, -1.0,  0.0},
    { 0.0,  0.0,  1.0},
    { 0.0,  0.0,  0.0},
    {-1.0,  0.0,  0.0}},
   {{ 0.0,  0.0,  1.0},
    { 1.0,  0.0,  0.0},
    { 0.0,  0.0,  0.0},
    { 0.0,  1.0,  0.0}},
   {{ 1.0,  0.0,  0.0},
    { 0.0,  0.0,  1.0},
    { 1.0,  0.0,  1.0},
    { 0.0,  1.0,  0.0}}};

    double new_tri[4][3];

    // Init variables
    double c_angle[2] = {0.0, 0.0}; // (deg) Angle of rotation from x+, (vert, hor)
    double c_fov = 90.0f; // degrees
    double c_pos[3] = {-5.0, 0.0, 0.0}; //(x,y,z)
    double c_rdist = 1.0f; // Render distance

    // Init gradient & lighting_delta (angle between camera and face normal
    double lighting_dot = 0;
    double lighting_dangle = 0.0;
    double l_ray[3] = {0.0, 0.0, 0.0};
    const char gradient[] = {'M', 'W', 'N', 'X', 'K', '0', 'O', 'k', 'x', 'd', 'o', 'l', 'c', ':', ';', ',', '\'', '.'};
    int shades = 18;
    //const char gradient[7] = {'#', '&', '=', '+', '!', '-', '\''};//{'\'', '-', '!', '+', '=', '&', '#'};
    //int shades = 7;
    //const char gradient[1] = {'#'};
    //int shades = 1;

    // Graphics loop
    while (true) {
        // Reset z_buffer
        for (int i=0; i<w_width; i++) {
            for (int j=0; j<w_height; j++) {
                zbuf[i][j] = 0;
            }
        }

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

        if (c=='q') {rot-=0.1;}
        if (c=='e') {rot+=0.1;}

        for (int t=0; t<tris; t++) {
            // Rotate triangle v_rz
            double tx, ty, tz;
            v_rz(triangle[t][0], rot, &tx, &ty, &tz); new_tri[0][0]=tx; new_tri[0][1]=ty; new_tri[0][2]=tz;
            v_rz(triangle[t][1], rot, &tx, &ty, &tz); new_tri[1][0]=tx; new_tri[1][1]=ty; new_tri[1][2]=tz;
            v_rz(triangle[t][2], rot, &tx, &ty, &tz); new_tri[2][0]=tx; new_tri[2][1]=ty; new_tri[2][2]=tz;
            v_rz(triangle[t][3], rot, &tx, &ty, &tz); new_tri[3][0]=tx; new_tri[3][1]=ty; new_tri[3][2]=tz;

            if (c_angle[1] < -180) {c_angle[1] = 360 + c_angle[1];}
            if (c_angle[1] >  180) {c_angle[1] =-360 + c_angle[1];}
            if (c_angle[0] <  -90) {c_angle[0] = -90;}
            if (c_angle[0] >   90) {c_angle[0] =  90;}

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

                    //dest[0]=0;dest[1]=-1;dest[2]=0;

                    // Determine light ray direction (ray now plane behind camera)
                    double tx, ty, tz;
                    l_ray[0] = c_rdist; l_ray[1]=0.0; l_ray[2]=0.0;
                    v_ry(l_ray, (c_angle[0]*M_PI)/180.0, &tx, &ty, &tz);
                    l_ray[0]=tx;l_ray[1]=ty;l_ray[2]=tz;
                    v_rz(l_ray, (c_angle[1]*M_PI)/180.0, &tx, &ty, &tz);
                    l_ray[0]=tx;l_ray[1]=ty;l_ray[2]=tz;

                    // Determine colour for lighting
                    char state = ' ';
                    int shade;
                    lighting_dot = l_ray[0]*new_tri[3][0]*-1 +  l_ray[1]*new_tri[3][1]*-1 + l_ray[2]*new_tri[3][2]*-1;
                    if (lighting_dot < -1) {lighting_dot = -1;}
                    if (lighting_dot >  1) {lighting_dot =  1;}
                    lighting_dangle = 1.0 - (acos(lighting_dot)*(2.0/M_PI));
                    if (lighting_dangle < 0) {lighting_dangle *= -1;}
                    shade = shades - (int)round(lighting_dangle*shades);
                    if (shade > shades-1) {shade = shades-1;}

                    // Check for intersection with triangle
                    double is_intersection = intersects_triangle(c_pos, dest, new_tri[0], new_tri[1], new_tri[2]);
                    if ((is_intersection != 0) && (zbuf[x][y] == 0 || is_intersection < zbuf[x][y])) {state=gradient[shade]; zbuf[x][y]=is_intersection;mvaddch(y,x,state);}//state=gradient[shade]; zbuf[x][y]=is_intersection;}
                    else if (zbuf[x][y] == 0) {state=' ';mvaddch(y,x,state);}
                    //else {state='8';}
                    //sprintf(&state, "%d", (int)zbuf[x][y]);
                   // mvaddch(y,x,state);

                    // Debug
                    //temp = rot;
                    //char debug[256];
                    //sprintf(debug, "TMP:%f, Ox:%f, Oy:%f, Oz:%f, Lx:%f, Ly:%f, Lz:%f, Ah:%f, Av:%f",temp, c_pos[0], c_pos[1], c_pos[2], l_ray[0], l_ray[1], l_ray[2], c_angle[1], c_angle[0]);
                    //mvprintw(0,0,debug);

                    // Delay
                    //usleep(1000000/10000);
                }
            }
            //char tri_n[16];
            //sprintf(tri_n, "%d", t);
            //mvprintw(0,0,tri_n);
            //refresh();
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
