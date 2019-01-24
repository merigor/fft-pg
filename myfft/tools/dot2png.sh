#! /bin/bash

# converts dot files to png (x.dot -> x.png)

for i in `ls *.dot`; do dot $i -Tpng -o${i%.*}.png; done;