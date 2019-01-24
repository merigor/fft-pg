#! /bin/bash

## array of compiler names
declare -a comp=("intel")
# ("clang8mac" "gcc7mac")
# ("clang4" "gcc6" "intel")
# ("pgi")

## array of input sizes
declare -a lens=("32" "64" "128" "256" "512" "1024" "2048" "4096" "8192" "16384" "32768" "65536")

## array of repetitions
declare -a reps=("8")

for c in "${comp[@]}";
do
    make clean;
    make clean-out;
    make -f "$c"".mk" all;
    for i in `ls *.x`;
    do
        for j in "${lens[@]}";
        do
            ./"$i" "$j" "${reps[0]}" >> "numbers/""$c""_""$i"".txt";
        done;
    done;
done;
