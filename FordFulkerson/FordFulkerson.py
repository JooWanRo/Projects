# @author: Joo Wan Ro (jr3fs)
# @due date: 4/13/2015

import sys
import math
import Queue
from collections import defaultdict

students = {}
classes = {}
edges = []
aList = []
aMatrix = []
parent = []
color = []

def bfs(nCount, start, end):
	global parent
	global color
	# 0 => WHITE
	# 1 => GREY 
	# 2 => BLACK
	# instantiate a Queue
	queue = Queue.Queue()
	# instantiate parent and color lists
	parent = [0]*(nCount+1)
	color = [0]*(nCount+1)
	# initialize source node to grey color
	color[0] = 1
	queue.put(0)
	# start BFS
	while(not(queue.empty())):
		u = queue.get()
		for v in range(len(aList[u])):
			# check if the color of v is white AND if capacity allows flow
			if (color[aList[u][v]] == 0 and aMatrix[u][aList[u][v]] > 0):
				# set color to grey and add v into the queue
				color[aList[u][v]] = 1
				queue.put(aList[u][v])
				# set parent of v to u
				parent[aList[u][v]] = u
		# set color of u to black
		color[u] = 3
	# after BFS is finished, we must check if we found an augmenting path
	return (color[end] == 3)

def FordFulkerson(nCount, start, end):
	# instantiate return variable
	maxFlow = 0
	# while there is an augmenting path
	while (bfs(nCount, start, end)):
		# bottle-neck flow for the current augmenting path
		bNeckFlow = sys.maxint

		# variable for node v of (u, v)
		v = nCount
		# backtrace the parent array from sink and find bottle-neck flow
		for i in range(len(parent)):
			# get the parent
			u = parent[v]
			# find the bottle-neck flow
			bNeckFlow = min(bNeckFlow, aMatrix[u][v])
			# check if we reached the source; else, update v
			if (u == 0): break
			v = u

		# variable for node v of (u, v)
		v = nCount		
		# update residual graph
		for i in range(len(parent)):
			# get the parent
			u = parent[v]
			# take away flow from (u, v)
			aMatrix[u][v] -= bNeckFlow
			# check if edge (v, u) needs to be added into aList
			if (aMatrix[v][u] == 0): aList[v].append(u)
			# add flow onto (v, u), the back-flow
			aMatrix[v][u] += bNeckFlow
			# check if we reached the source; else, update v
			if (u == 0): break
			v = u

		# update maxFlow
		maxFlow += bNeckFlow
	return maxFlow

with open(sys.argv[1], 'r') as f:
	while (True):
		# retrieve r, c, n
		case = f.readline().split( )
		r = int(case[0])
		c = int(case[1])
		n = int(case[2])
		# check for end
		if (r == 0 and c == 0 and n == 0): break
		# node counter
		nCount = 1
		# process r requests
		for i in range(r):
			# grab line
			line = f.readline().split( )
			# insert student
			try: 
				if (students[line[0]] != nCount): pass
			except:
				students[line[0]] = nCount
				# increment nCount
				nCount += 1
			# insert class
			try:
				if (classes[line[1]] != nCount): pass
			except:
				classes[line[1]] = nCount
				# increment nCount
				nCount += 1
			# add this edge
			edges.append((students[line[0]], classes[line[1]]))

		# generate aList and fill it
		aList = [[] for r in range(nCount+1)]
		# generate aMatrix and fill it
		aMatrix = [[0]*(nCount+1) for r in range(nCount+1)]
		# start with source node
		for k in students:
			# filling aList
			aList[0].append(students[k])
			# filling aMatrix
			aMatrix[0][students[k]] = n
		# then with the students/classes
		for i in range(len(edges)):
			# filling aList
			aList[edges[i][0]].append(edges[i][1])
			# filling aMatrix
			aMatrix[edges[i][0]][edges[i][1]] = 1
		# process c classes
		for i in range(c):
			# grab line
			line = f.readline().split( )
			# filling aList
			aList[classes[line[0]]].append(nCount)
			# filling aMatrix
			aMatrix[classes[line[0]]][nCount] = int(line[1])

		# run Ford-Fulkerson and return result
		if (FordFulkerson(nCount, 0, nCount) == (n*len(students))): print "Yes"
		else: print "No"

		# empty data structures
		edges[:] = []
		students.clear()
		classes.clear()
		# grab empty line
		f.readline()