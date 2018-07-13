# test cases
import pytest
import cNetwork

def test0():
	assert cNetwork.evaluate((0, [0], {0:0})) == 0

def test1():
	assert cNetwork.evaluate((1, [0,0], {0:1, 1:2, 2:3, 3:0})) == 3

def test2():
	assert cNetwork.evaluate((1, [1,1], {0:1, 1:2, 2:3, 3:0})) == 1

def test3():
	assert cNetwork.evaluate((1, [1,1], {0:1})) == 1

def test4():
	assert cNetwork.evaluate((2, [0,1,2], {0:1, 1:3, 2:1, 3:0})) == 0

def test5():
	assert cNetwork.evaluate((0, [0,0,0,0,0,0,0,0,0,0], {0:0})) == 0

def test6():
	assert cNetwork.evaluate((3, [1,1], {0:1, 1:2, 2:3, 3:0})) == 3

def test7():
	assert cNetwork.evaluate((2, [0,1,2,4,0,0,0],  {0: 1, 1: 6, 2: 10, 3: 4, 4: 15, 5: 8, 6: 13, 7: 14, 8: 3, 9: 2, 10: 5, 11: 0, 12: 9, 13: 11, 14: 12, 15: 7} )) == 10

def test8():
	assert cNetwork.evaluate((0, [0,1,2,4,0,0,0],  {0: 1, 1: 6, 2: 10, 3: 4, 4: 15, 5: 8, 6: 13, 7: 14, 8: 3, 9: 2, 10: 5, 11: 0, 12: 9, 13: 11, 14: 12, 15: 7} )) == 11

def test9():
	assert cNetwork.evaluate((10, [0,1,2,4,0,0,0],  {0: 1, 1: 6, 2: 10, 3: 4, 4: 15, 5: 8, 6: 13, 7: 14, 8: 3, 9: 2, 10: 5, 11: 0, 12: 9, 13: 11, 14: 12, 15: 7} )) == 9