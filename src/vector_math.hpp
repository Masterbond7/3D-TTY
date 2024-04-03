#include "./fast_math.hpp"

int v_ry(double vin[3], double angle, double *x, double *y, double *z) {
    // Get the rotation matrix
    double m[3][3] = {
           {q_cos(angle),  0, q_sin(angle)},
           {0,           1, 0         },
           {-q_sin(angle), 0, q_cos(angle)}};

    // Multiply by the rotation matrix
    *x = vin[0]*m[0][0] + vin[1]*m[0][1] + vin[2]*m[0][2];
    *y = vin[0]*m[1][0] + vin[1]*m[1][1] + vin[2]*m[1][2];
    *z = vin[0]*m[2][0] + vin[1]*m[2][1] + vin[2]*m[2][2];

    // Return 0
    return 0;
}

int v_rz(double vin[3], double angle, double *x, double *y, double *z) {
    // Get the rotation matrix
    double m[3][3] = {
           {q_cos(angle), -q_sin(angle), 0},
           {q_sin(angle), q_cos(angle),  0},
           {0,          0,           1}};

    // Multiply by the rotation matrix
    *x = vin[0]*m[0][0] + vin[1]*m[0][1] + vin[2]*m[0][2];
    *y = vin[0]*m[1][0] + vin[1]*m[1][1] + vin[2]*m[1][2];
    *z = vin[0]*m[2][0] + vin[1]*m[2][1] + vin[2]*m[2][2];

    // Return 0
    return 0;
}

/* Example code
int main() {
    double vin[3] = {2, 1, 5};
    double angle = M_PI / 2.0;
    double x, y, z;
    v_rz(vin, angle, &x, &y, &z);
    double vout[3] = {x, y, z};
    std::cout << "X: " << vout[0] << ", Y: " << vout[1] << ", Z: " << vout[2] << std::endl;
}
*/
