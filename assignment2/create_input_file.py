import random

filename = "input.txt"

f = open(filename, 'w')

#m = random.randint(5, 20)
m = 10
f.write("%d \n" % (m))

for ii in range(m):
    for jj in range(m):
        n = random.randint(1,100)
        f.write("%02d " % (n))
    f.write("\n")


