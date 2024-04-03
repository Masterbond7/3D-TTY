#include "./fast_math.hpp"

int q_v_ry(double vin[3], double angle, double *x, double *y, double *z) {
    float cos, sin;
    cos = q_cos(angle);
    sin = q_sin(angle);

    // Multiply by the rotation matrix
    *x =  vin[0]*cos + vin[2]*sin;
    *y =  vin[1];
    *z = -vin[0]*sin + vin[2]*cos;

    // Return 0
    return 0;
}

int q_v_rz(double vin[3], double angle, double *x, double *y, double *z) {
    float cos, sin;
    cos = q_cos(angle);
    sin = q_sin(angle);

    // Multiply by the rotation matrix
    *x = vin[0]*cos - vin[1]*sin;
    *y = vin[0]*sin + vin[1]*cos;
    *z = vin[2];

    // Return 0
    return 0;
}

int v_rz(double vin[3], double angle, double *x, double *y, double *z) {
    float n_cos, n_sin;
    n_cos = cos(angle);
    n_sin = sin(angle);

    // Multiply by the rotation matrix
    *x = vin[0]*n_cos - vin[1]*n_sin;
    *y = vin[0]*n_sin + vin[1]*n_cos;
    *z = vin[2];

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
