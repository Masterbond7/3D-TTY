#include <iostream>
#include <ncurses.h>
#include "./moller_trumbore.hpp"
#include "./vector_math.hpp"
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

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
    std::ifstream infile("./assets/lowpolyturtlele.3dtty");
    std::string line;

    int tris;
    if (infile) {

        getline(infile, line);
        std::istringstream iss(line);
        iss >> tris;
        if (tris == 0) {return 2;}
    }
    double triangle[tris][4][3]; // v0, v1, v2, normal

    float v1_x, v1_y, v1_z, v2_x, v2_y, v2_z, v3_x, v3_y, v3_z, n_x, n_y, n_z;
    for (int i=0; i<tris; i++) {
        getline(infile, line);
        std::istringstream iss(line);
        iss >> v1_x >> v1_y >> v1_z >> v2_x >> v2_y >> v2_z >> v3_x >> v3_y >> v3_z >> n_x >> n_y >> n_z;
        triangle[i][0][0] = v1_x; triangle[i][0][1] = v1_y; triangle[i][0][2] = v1_z;
        triangle[i][1][0] = v2_x; triangle[i][1][1] = v2_y; triangle[i][1][2] = v2_z;
        triangle[i][2][0] = v3_x; triangle[i][2][1] = v3_y; triangle[i][2][2] = v3_z;
        triangle[i][3][0] =  n_x; triangle[i][3][1] =  n_y; triangle[i][3][2] =  n_z;
    }
    
    double new_tri[4][3];

    // Init variables
    double c_angle[2] = {0.0, 0.0}; // (deg) Angle of rotation from x+, (vert, hor)
    double c_fov = 90.0f; // degrees
    double c_pos[3] = {-125.0, 0.0, 10.0}; //(x,y,z)
    double c_rdist = 1.0f; // Render distance

    // Init gradient & lighting_delta (angle between camera and face normal
    double lighting_dot = 0;
    double lighting_dangle = 0.0;
    double l_ray[3] = {0.0, 0.0, 0.0};
    const char gradient[] = {'#', 'M', 'W', 'N', 'X', 'K', '0', 'O', 'k', 'd', 'x', 'o', 'l', 'c', ';', ':', ',', '\'', '.'};
    int shades = 19;

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
        //c = getch();
        rot+=0.1;
        //usleep(1000);
        if (c=='w') {c_pos[0]+=0.25*10;}
        if (c=='s') {c_pos[0]-=0.25*10;}
        if (c=='d') {c_pos[1]+=0.25*10;}
        if (c=='a') {c_pos[1]-=0.25*10;}
        if (c=='r') {c_pos[2]+=0.25*10;}
        if (c=='f') {c_pos[2]-=0.25*10;}

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
                double h_angle = ((c_fov/-2.0f)+(c_angle_step) * x); // Angle is in degrees
                h_angle = (h_angle*M_PI)/180.0; // Converts angle to radians

                // Initialize ray destination variables
                double dest[3];
                double dest_x, dest_y, dest_z;

                // Go through each row of the screen
                for (int y=0; y<w_height; y++) {
                    // Get the vertical angle of the ray
                    double v_angle = (c_angle_step*-1*(w_height/2)) + (c_angle_step * y);
                    v_angle *= 2; // x2 because text is twice as tall as wide
                    v_angle = (v_angle*M_PI)/180.0; // Converts angle to radians

                    // Calculate ray destination
                    dest[0]=c_rdist;dest[1]=0;dest[2]=0;          // Initialize dest
                    dest[0]=dest[0];dest[1]=c_rdist*tan(h_angle);dest[2]=-c_rdist*tan(v_angle); // Plot ray points on plane

                    // Rotate plane
                    v_ry(dest, (c_angle[0]*M_PI)/180.0, &dest_x, &dest_y, &dest_z);     // Rotate dest up/down
                    dest[0]=dest_x;dest[1]=dest_y;dest[2]=dest_z; // Set new dest
                    v_rz(dest, (c_angle[1]*M_PI)/180.0, &dest_x, &dest_y, &dest_z); // Rotate dest left/right
                    dest[0]=dest_x;dest[1]=dest_y;dest[2]=dest_z; // Set new dest

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
                }
            }
            //char tri_n[16];
            //sprintf(tri_n, "%d", t);
            //mvprintw(0,0,tri_n);
            //refresh();
        }

        // Display update
        char text[64];
        sprintf(text, "%s", "If whatsapp was in my computer: ");
        mvprintw(0,0,text);
        sprintf(text, "%s", "factorio <(vibin')");
        mvprintw(1,19,text);
        sprintf(text, "%s", "(i NEED cpu)>");
        mvprintw(2,25,text);
        sprintf(text, "%s", "steam <(vibin')");
        mvprintw(3,22,text);
        sprintf(text, "%s", "terminal <(vibin')");
        mvprintw(4,19,text);
        sprintf(text, "%s", "firefox <(vibin')");
        mvprintw(5,20,text);
        //move(2,25); std::cout << "(i \e[5mNEED\e[25m cpu]])>";
        refresh();

        // Delay
        //usleep(1000000/30); // 30fps
    }

    // End program
    endwin();

    // Exit Successfully
    return 0;
}
