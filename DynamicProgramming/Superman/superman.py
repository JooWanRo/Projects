from sys import stdin
from operator import itemgetter
import math
memo = []
sliceCol = []
sub = []

# for loop
def MaxAns(row):
    maxi = 0
    for i in range(row):
        if (memo[i][0] > maxi): maxi = memo[i][0]
    return maxi
        
l = stdin.readline().split( )
N = int(l[0])
B = int(l[1])
I = int(l[2])
sub = range(N)
# ------------------------------
memo = [[0]*B for r in range(N)]

if (B == 1):
    maxi = 0
    for i in range(N):
        line = stdin.readline().split( )
        for j in range(int(line[0])):
            memo[i][int(line[j+1])-1] += 1
    print MaxAns(N)
    
else:
    for i in range(N):
        line = stdin.readline().split( )
        for j in range(int(line[0])):
            memo[i][int(line[j+1])-1] += 1
    for i in range(B-2, B-1-I, -1):
        for j in range(N):
            memo[j][i] += memo[j][i+1]
    for i in range(B-1-I, -1, -1):
        alpha = (0,0)
        beta = (0,0)
        for n in range(N):
            if (memo[n][i+I] > alpha[0]):
                beta = alpha
                alpha = (memo[n][i+I], n)
            elif (memo[n][i+I] > beta[0]):
                beta = (memo[n][i+I], n)
        for j in range(N):
            if (alpha[1] != j): memo[j][i] = memo[j][i] + max(memo[j][i+1], alpha[0])
            else: memo[j][i] = memo[j][i] + max(memo[j][i+1], beta[0])
    print MaxAns(N)

    