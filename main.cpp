#include <iostream>
#include <unordered_map>
#include <Python.h>

// Definitions
# define size_of(type) ((char *)(&type+1)-(char*)(&type)) 


/*
 * int evaluate(input, keys, sbox mapping)
 * 
 * ::Params::
 * int depth: this is the input to the network
 * int[] keys: array of keys to use. depth of network is inferred on the length of keys 
 * map &sbox: reference to sbox map
 *
 * ::Returns::
 * Int output from the network
 */
int evaluate(int input, 
			 int keys[], 
			 std::unordered_map<int, int> &sbox) {
	
	// accumulator 
	int out = 0;
	
	// add input to out
	out += input;
	
	// get depth
	int depth = size_of(keys)/size_of(keys[0]);
	
	// loop through depth
	for(int i = 0; i < depth; i++){
		// XOR
		out ^= keys[i];

		// Pass through SBOX mapping
		out = sbox[out];
	}
	
	return out;
}


int main(int argc, char *argv[]) {
	int result;
	int keys[] = {1,2};
	
	// Declare variables 
	std::unordered_map<int, int> sbox = {
		{0,1},{1,2},{2,3},{3,0}
	};
	
	
	result = evaluate(3, keys, sbox);
	
	std::cout<< "We obtained : " << result << " from the network.";
	
	return 0;
} 


