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

//https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm
/*float q_acos(float x) {
  float negate = float(x < 0);
  x = abs(x);
  float ret = -0.0187293;
  ret = ret * x;
  ret = ret + 0.0742610;
  ret = ret * x;
  ret = ret - 0.2121144;
  ret = ret * x;
  ret = ret + 1.5707288;
  ret = ret * sqrt(1.0-x);
  ret = ret - 2 * negate * ret;
  return negate * 3.14159265358979 + ret;
}*/

float q_acos(float x) {
   return (-0.6981317 * x * x - 0.8726646) * x + 1.5707963;
}