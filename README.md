# XORSBOX network for Python
Simplified implementation of symmetric encryption schemes for security testing. This is written in C++ with python wrapping for speed. 

# Build instructions
To build you can do the following.

1. This will install into your python env
```
$ pip install .
```

2. Alternatively, for testing we can build locally. 
```
$ python setup.py build
```

3. To build inplace such that the files can be found n the same folder as module.cpp
```
$ python setup.py build_ext --inplace

OR

$ ./setup.sh
```


# To Run
```
$ python
>>> import cNetwork
>>> cNetwork.evaluate((1, [2,1], {0:1, 1:2, 2:3, 3:0}))
```

Instructions on using evaluate
```
evaluate(tuple(input_val -> int, key -> list[int], sbox -> dict{int: int} ))
	
	Evaluates input_val with list of key and sbox provided. Be sure to wrap the
	values in a tuple.
``` 