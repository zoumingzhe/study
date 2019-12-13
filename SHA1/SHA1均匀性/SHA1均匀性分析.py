#coding:utf-8
import sys
import random
import hashlib
from math import log

# key   : index number
# value : list of appeared chars

def number_sequence(loop_number):
    HexBitDict = {}
    for i in range(loop_number):
        hash = hashlib.sha1()
        hash.update(str(i).encode("utf-8"))
        s = hash.hexdigest()
        for j in range(40):
            if not j in HexBitDict:
                HexBitDict[j] = [s[j]]
            else:
                HexBitDict[j].append(s[j])
        if (i % 100000 == 99999):
            print("%s %d" % (sys._getframe().f_code.co_name, i + 1))
    return HexBitDict

def number_random(loop_number):
    HexBitDict = {}
    for i in range(loop_number):
        hash = hashlib.sha1()
        hash.update(str(random.randint(1,loop_number*10)).encode("utf-8"))
        s = hash.hexdigest()
        for j in range(40):
            if not j in HexBitDict:
                HexBitDict[j] = [s[j]]
            else:
                HexBitDict[j].append(s[j])
        if (i % 100000 == 99999):
            print("%s %d" % (sys._getframe().f_code.co_name, i + 1))
    return HexBitDict

def entropy(HexBitList):
    HexBitSet   = set(HexBitList)
    lenList     = len(HexBitList)
    rate        = {}
    for i in HexBitSet:
        rate[i] = float(HexBitList.count(i)) / lenList
    return sum([-p * log(p, 2) for p in rate.values()])

def show(HexBitDict):
    print("Bit\t | entropy  , delta")
    for i in range(len(HexBitDict)):
        ret = entropy(HexBitDict[i])
        print("%d\t : %0.6f , %0.6f" % \
            (i + 1, ret, 4 - ret))

if __name__ == '__main__':
    show(number_sequence(1000000))
    show(number_random(1000000))