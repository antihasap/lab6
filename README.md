compilation:
1. using make

''' bash
make clean
make

./airport my_flights.txt
'''

2. manually
''' bash
g++ -std=c++11 -Wall -Wextra main.cpp airport.cpp -o airport
'''
