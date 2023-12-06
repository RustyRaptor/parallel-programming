"""This script will test all the implementations of count_threes
and record the running times of each implementation. Each implementation
will be tested on sizes of 10000, 100000, 1000000, 10000000. It will be run 
5 times each and the average will be taken. The results will be saved to a csv.
The implementations are C programs and a Java Program contained in subfolders. 
Each one has a makefile to compile it and can be run from the command line as
follows:
        
            $ make
            $ ./count_threes 
"""

import time
import csv
