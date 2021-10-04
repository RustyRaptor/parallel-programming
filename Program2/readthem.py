"""
This simple program is to quickly parse out the outputs so we can get the average
times of the runs. outs.txt must contain the raw console outputs

"""

from statistics import mean
par = []
ser = []

avgp = []
avgs = []

threads = 0
vals = 0


with open('./outs.txt') as reader:
        lines = reader.readlines()

        for line in lines:
                if vals == 5:
                        avgp.append(mean(par))
                        avgs.append(mean(ser))
                        par=[]
                        ser=[]
                        vals = 0
                        threads += 1
                word = line.split(' ')
                if 'parallel:' in word:
                        par.append(int(word[2]))
                if 'serial:' in word:
                        ser.append(int(word[4]))
                        vals += 1

print("ser: ", avgs, "par: ", avgp, threads)



