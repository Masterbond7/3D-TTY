#include <iostream>
#include <cmath>

// Function to rotate a 3D vector around the x-axis.
void rotateX(double& x, double& y, double& z, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    double newY = cosA * y - sinA * z;
    double newZ = sinA * y + cosA * z;
    y = newY;
    z = newZ;
}

// Function to rotate a 3D vector around the y-axis.
void rotateY(double& x, double& y, double& z, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    double newX = cosA * x + sinA * z;
    double newZ = -sinA * x + cosA * z;
    x = newX;
    z = newZ;
}

int main() {
    // Initial camera direction along the z-axis.
    double rayDirectionX = 0.0;
    double rayDirectionY = 1.0;
    double rayDirectionZ = 0.0;

    // Vertical and horizontal rotation angles (in radians).
    double verticalAngle = M_PI_4; // Adjust this angle for vertical rotation.
    double horizontalAngle = M_PI_4; // Adjust this angle for horizontal rotation.

    // Rotate the camera direction.
    rotateX(rayDirectionX, rayDirectionY, rayDirectionZ, verticalAngle);
    rotateY(rayDirectionX, rayDirectionY, rayDirectionZ, horizontalAngle);

    // The resulting ray direction vector is (rayDirectionX, rayDirectionY, rayDirectionZ).
    std::cout << "Ray Direction: (" << rayDirectionX << ", " << rayDirectionY << ", " << rayDirectionZ << ")\n";

    return 0;
}
