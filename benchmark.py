# Benchmark
import time
import cNetwork as net

input = 1
key = [2,1,2,1,2,3,1,3,2,3,1,1,1,1,2,2,1,1,1,1,1,1,1,1,2,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
sbox = {0:1, 1:2, 2:3, 3:0}

cTime = []
pyTime = []

for x in range(100):
	print('-----------------------------------')
	print("C Implementation")
	start = time.time()
	out = net.evaluate((input, key, sbox))
	print("Output: {}".format(out))
	finish_time = time.time() - start
	print("Time elapsed: {}".format(finish_time))
	cTime.append(finish_time)



	print("Python Implementation")
	start = time.time()
	out = input
	for i in range(len(key)):
		out ^= key[i]
		out = sbox[out]
	print("Output: {}".format(out))
	finish_time = time.time() - start
	print("Time elapsed: {}".format(finish_time))
	pyTime.append(finish_time)


print("Average C Time= {}".format(sum(cTime)/len(cTime)))
print("Average Py Time= {}".format(sum(pyTime)/len(pyTime)))