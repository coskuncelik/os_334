import random

number_of_files = 10


for ii in range(number_of_files+1):
    filename = "input"+str(ii)+".txt"

    f = open(filename, 'w')

    nums = random.randint(5, 20)
    f.write("%d \n" % (nums))

    for num in range(nums):
        n = random.randint(1,100)
        f.write("%d " % (n))
