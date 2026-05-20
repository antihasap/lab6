compilation:
1. using make

make clean

make

./airport my_flights.txt

2. manually

g++ -std=c++11 -Wall -Wextra main.cpp airport.cpp -o airport
