from hashlib import md5
from math import log
import hashlib

def entropy(wkList):
    wkSet   = set(wkList)
    rate    = {}
    lenList = len(wkList)
    for i in wkSet:
        rate[i] = float(wkList.count(i)) / lenList
    return sum([-p * log(p, 2) for p in rate.values()])

if __name__ == '__main__':
    wkDict = {} # key: index number; value: list of appeared chars
    for i in range(1000000):
        hash = hashlib.md5()
        hash.update(str(i).encode("utf-8"))
        s = hash.hexdigest()
        for j in range(32):
            if not j in wkDict:
                wkDict[j] = [s[j]]
            else:
                wkDict[j].append(s[j])

    for j in range(32):
        print(j, '\t', entropy(wkDict[j]))