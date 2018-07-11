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
