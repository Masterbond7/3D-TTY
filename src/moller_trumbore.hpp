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

double intersects_triangle(double orig[3], double dest[3],
                        double vert0[3], double vert1[3], double vert2[3]) {
    // Create variables
    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det; // Determinant
    double u, v, t;

    // Find vectors for edges from vert0
    SUB(edge1, vert1, vert0);
    SUB(edge2, vert2, vert0);

    // Calculate P
    CROSS(pvec, dest, edge2);

    // Calculate determinant
    det = DOT(edge1, pvec);

    // If ~0 then ray is paralell to the triangle (no hit)
    if (det < EPSILON) {return 0;}

    // Calculate T (distance from vert0 to origin of ray)
    SUB(tvec, orig, vert0);

    // Calculate U and test if inside triangle
    u = DOT(tvec, pvec);
    if (u < 0.0 || u > det) {return 0;}

    // Calculate Q
    CROSS(qvec, tvec, edge1);

    // Calculate V and test if instide triangle
    v = DOT(dest, qvec);
    if (v < 0.0 || u+v > det) {return 0;}

    // Calculate T (distance to triangle plane?)
    t = DOT(edge2, qvec)/det;

    // If the ray didn't not hit the triangle then it hit
    return t;
}

// NOTE: NO BACKFACE CULLING
double old_intersects_triangle(double orig[3], double dest[3],
                        double vert0[3], double vert1[3], double vert2[3]) {
    // Create variables
    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det, inv_det; // Determinant and 1/det
    double u, v, t;

    // Find vectors for edges from vert0
    SUB(edge1, vert1, vert0);
    SUB(edge2, vert2, vert0);

    // Calculate P
    CROSS(pvec, dest, edge2);

    // Calculate determinant
    det = DOT(edge1, pvec);

    // If ~0 then ray is paralell to the triangle (no hit)
    if (det > -EPSILON && det < EPSILON) {return 0;}

    // Calculate inv_det
    inv_det = 1.0/det;

    // Calculate T (distance from vert0 to origin of ray)
    SUB(tvec, orig, vert0);

    // Calculate U and test if inside triangle
    u = DOT(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) {return 0;}

    // Calculate Q
    CROSS(qvec, tvec, edge1);

    // Calculate V and test if instide triangle
    v = DOT(dest, qvec) * inv_det;
    if (v < 0.0 || u+v > 1.0) {return 0;}

    // Calculate T (distance to triangle plane?)
    t = DOT(edge2, qvec);

    // check if cam is infront of and looking at normal or behind and looking at inverse normal
    //if (t*det < EPSILON) {return 0;}
    if (t < EPSILON) {return 0;}
    if (det < EPSILON) {return 0;}

    // If the ray didn't not hit the triangle then it hit
    //if (t < 0) {t *= -1;}
    return t;
}