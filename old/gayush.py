import random


# n=int(input())
# m=int(input())
n=5
m=4

a=(0,0)
mark_position = set()
# mark_position.add(a)

MAP = {i:c for i,c in enumerate("RLUD")}

def createA(n,m):
    B=list()
    for i in range(m):
        l= list()
        for j in range(n):
            l.append(MAP[random.randint(0,3)])
        B.append(l)
    return B

def printF(F):
    for f in F:
        print("".join(f))
A=createA(n,m)
printF(A)


def left(a):
    r,c = a
    return (r,c-1)

def right(a):
    r,c = a
    return (r,c+1)
def up(a):
    r,c = a
    return (r-1,c)

def down(a):
    r,c = a
    return (r+1,c)

def prog1(a):
    print("going to -->", a)
    if a not in mark_position:
        mark_position.add(a)
        prog2(a)
    else:
        print("cell does not exist")
        exit(0)

def is_out(a):
    r,c = a
    if r>=m or r<0:
        return True
    if c>=n or c<0:
        return True
    return False


def prog2(a):
    r,c = a
    if A[r][c]=="R":
        new_a = right(a)
    if A[r][c]=="L":
        new_a = left(a)
    if A[r][c]=="U":
        new_a = up(a)
    if A[r][c]=="D":
        new_a = down(a)
        
    q = is_out(new_a)
    if q == True:
        print("out from ", a)
        exit(0)
    else:
        prog1(new_a)
    
       
prog1((0,0))