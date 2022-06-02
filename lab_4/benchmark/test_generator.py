import random 
import sys    
import os

testFile = open('test.txt', 'w')
testFile.write('123' + '\n')

amount = int(sys.argv[1])
for i in range(0, amount):
    #value = random.randrange(100, 700)
    testFile.write('123 354 567 678' + '\n')

testFile.close()