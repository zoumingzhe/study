from ztools import filebase
import time
import sys


fb = filebase()
file_list = fb.scan(".\\", postfix=".csv")
if file_list != []:
    file_list.sort()
else:
    sys.exit()
file_name = fb.get_name(file_list)


strtime = time.strftime("%Y%m%d%H%M%S", time.localtime())
fb.ensure(".\\result")
file_result = ".\\result\\" + strtime + "-detail.csv"

with open(file_result, 'w') as r:
    for file in file_list:
        with open(file, 'r') as f:
            line = f.readline()
            r.write(file)
            n = 0
            s = 0
            result = []
            while line:
                eachline = line.split(',')
                resultline = {
                    "thd" : int(eachline[0]),
                    "sum" : int(eachline[-2]),
                    "avg" : int(eachline[-1]),
                    "val" : eachline[1:-2],
                }
                result.append(resultline)
                s = s + resultline["avg"]
                n = n + 1
                line = f.readline()
            r.write(',')
            r.write(str(n))
            r.write(',')
            r.write(str(s))
            r.write(',')
            r.write(str(round(s/n,1)))
            for item in result:
                r.write(',')
                r.write(str(resultline['avg']))
            r.write("\n")
            f.close()
    r.close()

for file in file_name:
    fb.move(file, ".\\result\\" + strtime + "\\" + file)
