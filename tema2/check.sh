#!/bin/bash

g++ -std=c++20 ./lnfa2regex/lnfa2regex.cpp -o lnfa2regex-e
g++ -std=c++20 ./regex2lnfa/regex2lnfa.cpp -o regex2lnfa-e
g++ -std=c++20 ./iso.cpp -o iso-e 
g++ -std=c++20 ../tema1/mini/mini.cpp -o mini-e
g++ -std=c++20 ../tema1/lnfa-to-dfa/lnfa2dfa.cpp -o lnfa2dfa-e