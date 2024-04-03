//https://web.archive.org/web/20100613230051/http://www.devmaster.net/forums/showthread.php?t=5784
#include <math.h>

const float pi= M_PI;
const float pi_2= M_PI*2;
const float B = 4/pi;
const float C = -4/(pi*pi);
const float D = M_PI/2;

float q_sin(float x) {
    float y = B * x + C * x * abs(x);

    return y;
}

float q_cos(float x) {x+=D; if (x>pi){x -= pi_2;} return q_sin(x);}
float q_tan(float x) {return q_sin(x)/q_cos(x);}