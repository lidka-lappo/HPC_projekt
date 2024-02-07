#!/bin/bash

echo "Running finging path algorythm."
g++ -o best_route best_route.cpp -std=c++11 -fopenmp
./best_route