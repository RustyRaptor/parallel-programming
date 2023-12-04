#!/bin/bash

for i in 1 2 4 8 16
do
        for j in 1 2 3 4 5
        do
                echo "running with $i threads and 1000 operations for the $j time"
                ./while_loops $i 1000
        done
    
done