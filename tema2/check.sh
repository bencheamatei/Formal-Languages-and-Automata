#!/bin/bash

g++ -std=c++20 ./lnfa2regex/lnfa2regex.cpp -o lnfa2regex-e
g++ -std=c++20 ./regex2lnfa/regex2lnfa.cpp -o regex2lnfa-e
g++ -std=c++20 ./iso.cpp -o iso-e 
g++ -std=c++20 ../tema1/mini/mini.cpp -o mini-e
g++ -std=c++20 ../tema1/lnfa-to-dfa/lnfa2dfa.cpp -o lnfa2dfa-e

# practic eu vreau ceva de genul 
# primesc un regex, din regex -> lnfa
# apoi din lnfa -> regex 
# acum ar trebui sa am r1 si r2 2 regexuri diferite. 
# r1, r2 -> lnfa 
# apoi le minimizez si vad daca sunt izomorfe, in caz afirmativ totul ar trebui sa fie okay 

r=$1
# echo "$r"

echo "$r" | ./regex2lnfa-e > a.txt
cat a.txt | ./lnfa2regex-e > b.txt 

echo "$r" | ./regex2lnfa-e | ./lnfa2dfa-e | ./mini-e > m1.txt
cat b.txt | ./regex2lnfa-e | ./lnfa2dfa-e | ./mini-e > m2.txt

ans=$(./iso-e m1.txt m2.txt)
echo "$ans"

rm -f lnfa2dfa-e lnfa2regex-e regex2lnfa-e iso-e mini-e
rm -f a.txt b.txt m1.txt m2.txt