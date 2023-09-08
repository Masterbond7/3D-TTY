echo -e "Compiling..."
g++ ./src/main.cpp -Wall -o ./bin/app.o

echo -e "\nRunning...\n"
./bin/app.o $@; echo "Exit code: "$?

echo -e "\nCleaning up..."
rm ./bin/app.o
