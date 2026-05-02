#!/bin/bash

g++ -std=c++20 regex2lnfa.cpp -o regex2lnfa-e
g++ -std=c++20 ../../tema1/lnfa/lambda-nfa.cpp -o lambda-nfa-e

echo $1 | ./regex2lnfa-e > ./lnfa.in

> ./words.in
shift 
for arg in "$@"; do 
    echo "$arg" >> ./words.in
done

./lambda-nfa-e

cat b.out
rm -f regex2lnfa-e lambda-nfa-e b.out words.in lnfa.in
