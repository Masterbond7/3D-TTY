#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::ifstream infile("./test.txt");

    if (infile) {
        std::string line;

        int num_lines;
        getline(infile, line);
        std::istringstream iss(line);
        iss >> num_lines;
        std::cout << "Num lines" << num_lines << std::endl;

        float v1_x, v1_y, v1_z, v2_x, v2_y, v2_z, v3_x, v3_y, v3_z, n_x, n_y, n_z;
        for (int i=0; i<num_lines; i++) {
            getline(infile, line);
            std::istringstream iss(line);
            iss >> v1_x >> v1_y >> v1_z >> v2_x >> v2_y >> v2_z >> v3_x >> v3_y >> v3_z >> n_x >> n_y >> n_z;
            std::cout << v1_x << v1_y << v1_z << v2_x << v2_y << v2_z << v3_x << v3_y << v3_z << n_x << n_y << n_z << std::endl;
        }
    }

    return 0;
}
