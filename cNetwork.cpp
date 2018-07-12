#include <Python.h>
#include <iostream>
#include <limits>
#include <vector>
//#include <cstdint>


// Definitions
# define size_of(type) ((char *)(&type+1)-(char*)(&type)) 
typedef std::vector<int> IntVector;


/*
 * Helper function
 * prints out an array
 */
// static void printArray(int arr[]) {
// 	for(int x = 0; x < size_of(arr)/size_of(arr[0]); x++){
// 		std::cout << arr[x] << " ";
// 	}
// 	std::cout << "\n" << std::endl;
// }

/*
 * Helper function
 * prints out a vector
 */
static void printVector(IntVector arr) {
	int size = arr.size();
    for(int i = 0; i < size; i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}


/* 
 * evaluate()
 * ::Params::
 * int depth: this is the input to the network
 * int[] keys: array of keys to use. depth of network is inferred on the length of keys 
 * map &sbox: reference to sbox map
 *
 * ::Returns::
 * Int output from the network
 */
static int evaluate(int input, IntVector keys, IntVector sbox) {
	
	// accumulator 
	int out = 0;
	
	// add input to out
	out += input;
	
	// get depth
	int depth = keys.size();
	
	// loop through depth
	for(int i = 0; i < depth; i++){
		// XOR
		out ^= keys[i];

		// Pass through SBOX mapping
		out = sbox[out];
	}
	
	return out;
}


/*
 * Safely converts long to int, if long exceed int returns 
 * NULL.
 */
static int safeLongToInt(long toConvert) {
	int output;

	if(toConvert >= INT_MIN && toConvert <= INT_MAX) {
		 output = toConvert;
	} else if (toConvert == 0) {
		return 0;
	} else {
		return -1;
	}

	return output;
}


/*
 * Parse the inputs from python object for use in the C++ context
 * NOTE: PyObject* args is just a python tuple, we can treat it
 * like any other Python tuple. 
 * 
 * Documentation found here:
 * https://docs.python.org/3/c-api/tuple.html
 */
static PyObject *evaluateWrapper(PyObject *self, PyObject *args) {
	// Get size of PyObject* args
	Py_ssize_t tupleSize = PyTuple_Size(args);

	// Check for Argument errors
	if(!tupleSize) {
		if(!PyErr_Occurred()) {
			PyErr_SetString(PyExc_TypeError, "You must supply at least one argument.");
		}

		return NULL;
	}

	// Extract objects from args
	PyObject *obj; 
	if(!PyArg_ParseTuple(args, "O", &obj)) {
		// ERROR
		if(!PyErr_Occurred()){
			PyErr_SetString(PyExc_TypeError, "Argument is not a valid python object.");
		}

		return NULL;
	}

	/*****************************
	 * 1. Get int input out      *
	 *****************************/

	// Treat obj as an iterator
	PyObject *iter = PyObject_GetIter(obj);
	if(!iter){
		// ERROR object is not an iterator
		PyErr_SetString(PyExc_TypeError, "Object is not an iterator");
		return NULL;
	}

	PyObject *next;

	// Extact first element: int input
	next = PyIter_Next(iter);

	// Build the resulting int into a Python object
	// NOTE: All integers are implemented as "long"
	// integer objects of arbitary size
	if(!PyLong_Check(next)) {
		// We didn't get a Integer
		PyErr_SetString(PyExc_TypeError, "1st argument not an integer.");
		return NULL;
	}

	// Get the long object out and convert to int
	long longInput = PyLong_AsLong(next);
	int input = safeLongToInt(longInput); 

	if(input == -1) {
		// We could not safely convert to int
		if(!PyErr_Occurred()) {
			PyErr_SetString(PyExc_ValueError, "Unable to convert input to integer.");
		}
		return NULL;
	}


	/*****************************
	 * 2. Get int[] keys out     *
	 *****************************/
	// Extact first element: int input
	next = PyIter_Next(iter);

	if(!PyList_Check(next)) {
		// We didn't get a list
		PyErr_SetString(PyExc_TypeError, "2nd argument not a list.");
		return NULL;
	}

	// Get size of list
	Py_ssize_t listSize = PyList_Size(next);

	/* DEBUG */
	std::cout << "listSize: " << listSize << std::endl;

	IntVector keys(listSize);

	// Check for Argument errors
	if(!listSize) {
		if(!PyErr_Occurred()) {
			PyErr_SetString(PyExc_TypeError, "You must supply at least one key to the 2nd argument.");
		}

		return NULL;
	}

	// extract value from the python list
	PyObject *tempPointer;
	for(Py_ssize_t i = 0; i < listSize; i++) {
		tempPointer = PyList_GetItem(next, i);
		long longKey = PyLong_AsLong(tempPointer);
		int intKey = safeLongToInt(longKey);

		if(intKey == -1) {
			// ERROR
			if(!PyErr_Occurred()) {
				PyErr_SetString(PyExc_ValueError, "Unable to convert list key to integer.");
			}
			return NULL;
		}

		std::cout << "Size of intKeys: " << size_of(intKey);
		std::cout << "\tSize of py i: " << size_of(i);
		int cast_i = (int) i;
		std::cout << " \tSize of int i: " << size_of(cast_i) << "\n" << std::endl;

		keys[cast_i] = intKey;

		/* DEBUG */
		std::cout << "Keys: ";
		printVector(keys);
	}


	/*****************************
	 * 3. Get map sbox   out     *
	 *****************************/
	// Extact first element: int input
	next = PyIter_Next(iter);

	if(!PyDict_Check(next)) {
		// We didn't get a list
		PyErr_SetString(PyExc_TypeError, "3rd argument not a dictionary.");
		return NULL;
	}

	// malloc space for sbox array
	Py_ssize_t dictSize = PyDict_Size(next);

	/* DEBUG */
	std::cout << "dictSize: " << dictSize << std::endl;

	IntVector sbox(dictSize);

	/* Debug */
	std::cout << "Size of allocated *sbox: " << dictSize * sizeof(int) << "\n" << std::endl;


	// iterate through dictionary
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	while(PyDict_Next(next, &pos, &key, &value)) {
		// get key
		long keyExtract = PyLong_AsLong(key);
		long valueExtract = PyLong_AsLong(value);

		int key_int = safeLongToInt(keyExtract);
		int value_int = safeLongToInt(valueExtract);

		if(key_int == -1) {
			// ERROR
			if(!PyErr_Occurred()) {
				PyErr_SetString(PyExc_ValueError, "Unable to convert dictionary key to integer.");
			}
			return NULL;
		}

		if(value_int == -1) {
			// ERROR
			if(!PyErr_Occurred()) {
				PyErr_SetString(PyExc_ValueError, "Unable to convert dictionary value to integer.");
			}
			return NULL;
		}

		std::cout << "Size of key_int: " << size_of(key_int) ;
		std::cout << "\tSize of value_int: " << size_of(value_int) << "\n" << std::endl;

		sbox[key_int] = value_int;
		std::cout << "sbox: " ;
		/* DEBUG */
		printVector(sbox);
	}

	/* DEBUG */
	// input
	std::cout << "input: " << input << "\n"<< std::endl;
	
	// keys
	std::cout << "size of keys: " << size_of(keys)/size_of(keys[0]) << std::endl;
	std::cout << "keys: " ;
	printVector(keys);
	std::cout << "\n" << std::endl;

	// dict
	std::cout << "size of sbox: " << size_of(sbox)/size_of(sbox[0]) << std::endl;
	std::cout << "sbox: " ;
	printVector(sbox);
	// Run the evaluate function
	long result = evaluate(input, keys, sbox);

	// return PyObject containing a int result
	return PyLong_FromLong(result);
}



/*
 * Define the C method for use in python
 */
static PyMethodDef cNetwork_methods[] = {
	{"evaluate", (PyCFunction) evaluateWrapper, METH_VARARGS, "Return output from the network."},
	{NULL, NULL, 0, NULL} /* SENTINEL */
};

/*
 * Add a structure that defines the module we want to refer in the Python code
 */
static struct PyModuleDef cNetwork_module = {
	PyModuleDef_HEAD_INIT,
	"cNetwork",						// module name to use with python import statement
	"C implementation of the sbox net", 	// module description
	0,
	cNetwork_methods					// Structure that defines the methods of the module
};


/*
 * Add a method that Python calls when it loads the module
 * This must be named PyInit_<module-name>. Where <module-name> matches 
 * the C++ project's General > Target Name property
 */
PyMODINIT_FUNC PyInit_cNetwork(void) {
	return PyModule_Create(&cNetwork_module);
}
