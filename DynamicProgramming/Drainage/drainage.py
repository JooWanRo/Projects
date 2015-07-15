# @author: Joo Wan Ro (jr3fs)
# @due date: 3/6/2015

import sys
import math
sys.setrecursionlimit(100000)

grid = []
memo = []
r = 0
c = 0
Max = 1

# drainage function (bottom-up dynamic programming / runtime: Theta(r*c))
def drainage(x, y):
	global Max
	# check memo
	if (memo[x][y] != 0): return memo[x][y]
	# case variables
	caseA, caseB, caseC, caseD = (0,)*4
	# look up
	if (x != 0 and grid[x][y] > grid[x-1][y]):
		caseA = drainage(x-1, y)
	# look down
	if (x != (r-1) and grid[x][y] > grid[x+1][y]):
		caseB = drainage(x+1, y)
	# look left
	if (y != 0 and grid[x][y] > grid[x][y-1]):
		caseC = drainage(x, y-1)
	# look right
	if (y != (c-1) and grid[x][y] > grid[x][y+1]):
		caseD = drainage(x, y+1)
	# base case
	if (max(caseA, caseB, caseC, caseD) == 0): memo[x][y] = 1
	# else
	else: memo[x][y] = 1 + max(caseA, caseB, caseC, caseD)
	# check with Max
	if (memo[x][y] > Max): Max = memo[x][y]
	return memo[x][y]

with open(sys.argv[1], 'r') as f:
	# retrieve number of cases
	case = int(f.readline())
	# work with each case
	for i in range(0, case):
		# retrieve case info
		caseInfo = (f.readline().split( ))
		title = caseInfo[0]
		r = int(caseInfo[1])
		c = int(caseInfo[2])
		# initialize and generate grid[][]
		grid = [[0]*c for a in range(r)]
		# initialize memo table
		memo = [[0]*c for b in range(r)]
		for j in range(0, r):
			# get row line
			line = (f.readline().split( ))
			# fill grid and gridList with row line
			for k in range(0, c):
				# generate grid
				grid[j][k] = int(line[k])
		# reset Max
		Max = 1
		# # drainage algorithm (bottom-up dynamic programming / runtime: Theta(r*c))
		for x in range(0, r):
			for y in range(0, c):
				memo[x][y] = drainage(x, y)
		# print result
		print title + ": " + str(Max)