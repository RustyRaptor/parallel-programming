"""
This simple program is to quickly parse out the outputs so we can get the average
times of the runs. outs.txt must contain the raw console outputs

"""

from statistics import mean
par = []
ser = []
pro = []

avgp = []
avgs = []
avgpr = []

threads = 0
vals = 0


with open('./outs.txt') as reader:
        lines = reader.readlines()

        for line in lines:
                if vals == 5:
                        avgp.append(mean(par))
                        avgs.append(mean(ser))
                        avgpr.append(mean(pro))
                        par=[]
                        ser=[]
                        pro = []
                        vals = 0
                        threads += 1
                word = line.split(' ')
                if 'process:' in word:
                        pro.append(int(word[2]))
                        # print(word)
                if 'thread:' in word:
                        par.append(int(word[3]))
                        # print(word)
                if 'serial:' in word:
                        # print(word)
                        ser.append(int(word[3]))
                        vals += 1
                
                        

print("ser: ", avgs, "par: ", avgp, "pro: ", avgpr, "threads: ", threads)



