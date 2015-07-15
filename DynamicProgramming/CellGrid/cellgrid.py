from sys import stdin
# recursion max
import math

row = int(stdin.readline())
col = int(stdin.readline())
matrix = [[0]*(col+2) for r in range(row+2)]
maxi = 0
coord = (0,0)

def travel(x,y):
    if (matrix[x][y] == -1 or matrix[x][y] == 0): return 0
    
    matrix[x][y] = -1

    return 1 + travel(x-1, y-1) + travel(x-1, y) + travel(x-1, y+1) + travel(x, y-1) + travel(x, y+1) +                    travel(x+1, y-1) + travel(x+1, y) + travel(x+1, y+1)

for i in range(1, row+1):
    line = stdin.readline().split( )
    for j in range(1, col+1):
        if (int(line[j-1]) == 0): matrix[i][j] = 0
        else: matrix[i][j] = 1

for i in range(1, row+1):
    for j in range(1, col+1):
        if (matrix[i][j] == 0): continue
        count = 0
        current = matrix[i][j]
        # look top-left
        if (matrix[i-1][j-1] == 1): count += 1
        # look top
        if (matrix[i-1][j] == 1): count += 1
        # look top right
        if (matrix[i-1][j+1] == 1): count += 1
        # look left
        if (matrix[i][j-1] == 1): count += 1
        # look right
        if (matrix[i][j+1] == 1): count += 1
        # look bottom left
        if (matrix[i+1][j-1] == 1): count += 1
        # look bottom
        if (matrix[i+1][j] == 1): count += 1
        # look bottom right
        if (matrix[i+1][j+1] == 1): count += 1
        
        update = current+count
        # update top-left
        if (update > matrix[i-1][j-1] and matrix[i-1][j-1] != 0): matrix[i-1][j-1] = update
        # update top
        if (update > matrix[i-1][j] and matrix[i-1][j] != 0): matrix[i-1][j] = update
        # update top right
        if (update > matrix[i-1][j+1] and matrix[i-1][j+1] != 0): matrix[i-1][j+1] = update
        # update left
        if (update > matrix[i][j-1] and matrix[i][j-1] != 0): matrix[i][j-1] = update
        # update right
        if (update > matrix[i][j+1] and matrix[i][j+1] != 0): matrix[i][j+1] = update
        # update bottom left
        if (update > matrix[i+1][j-1] and matrix[i+1][j-1] != 0): matrix[i+1][j-1] = update
        # update bottom
        if (update > matrix[i+1][j] and matrix[i+1][j] != 0): matrix[i+1][j] = update
        # update bottom right
        if (update > matrix[i+1][j+1] and matrix[i+1][j+1] != 0): matrix[i+1][j+1] = update
        # update current
        matrix[i][j] = update      
        # update maxi
        if (update > maxi): 
            maxi = update
            coord = (i,j)
            
print travel(coord[0], coord[1])