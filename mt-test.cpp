#include <iostream>

#define EPSILON 0.000001
#define CROSS(dest, v1, v2)\
        dest[0]=v1[1]*v2[2]-v1[2]*v2[1];\
        dest[1]=v1[2]*v2[0]-v1[0]*v2[2];\
        dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1, v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest, v1, v2)\
        dest[0]=v1[0]-v2[0];\
        dest[1]=v1[1]-v2[1];\
        dest[2]=v1[2]-v2[2];

int intersects_triangle(double orig[3], double dest[3],
                        double vert0[3], double vert1[3], double vert2[3]) {
    // Create variables
    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det, inv_det; // Determinant and 1/det
    double u, v;

    // Find vectors for edges from vert0
    SUB(edge1, vert1, vert0);
    SUB(edge2, vert2, vert0);

    std::cout << "Edge 1: [" << edge1[0] << ", " << edge1[1] << ", " << edge1[2] << "]\n";
    std::cout << "Edge 2: [" << edge2[0] << ", " << edge2[1] << ", " << edge2[2] << "]\n";

    // Calculate P
    CROSS(pvec, dest, edge2);
    std::cout << "  Pvec: [" << pvec[0] << ", " << pvec[1] << ", " << pvec[2] << "]\n";

    // Calculate determinant
    det = DOT(edge1, pvec);
    std::cout << "   Det: " << det << "\n";

    // If ~0 then ray is paralell to the triangle (no hit)
    if (det > -EPSILON && det < EPSILON) {return 0;}
    //if (det<EPSILON) {return 0;}

    // Calculate inv_det
    inv_det = 1.0/det;
    std::cout << " I_Det: " << inv_det << "\n";

    // Calculate T (distance from vert0 to origin of ray)
    SUB(tvec, orig, vert0);
    std::cout << "  Tvec: [" << tvec[0] << ", " << tvec[1] << ", " << tvec[2] << "]\n";

    // Calculate U and test if inside triangle
    u = DOT(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) {return 0;}
    std::cout << "     U: " << u << "\n";

    // Calculate Q
    CROSS(qvec, tvec, edge1);
    std::cout << "  Qvec: [" << qvec[0] << ", " << qvec[1] << ", " << qvec[2] << "]\n";

    // Calculate V and test if instide triangle
    v = DOT(dest, qvec) * inv_det;
    if (v < 0.0 || u+v > 1.0) {return 0;}
    std::cout << "     V: " << v << "\n";

    double t = DOT(edge2, qvec);
    std::cout << "     T: " << t << "\n";

    // If the ray didn't not hit the triangle then it hit
    return 1;
}

int main() {
    double origin[3] = { 1.00,  0.25,  0.25};
    double   dest[3] = {-1.00,  0.0,  0.0};
    double  vert0[3] = { 0.00,  0.00,  0.00};
    double  vert1[3] = { 0.00,  0.00,  1.00};
    double  vert2[3] = { 0.00,  1.00,  0.00};

    std::cout << intersects_triangle(origin, dest, vert0, vert1, vert2) << std::endl;
}
