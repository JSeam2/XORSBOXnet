from distutils.core import setup, Extension

cNetwork = Extension('cNetwork', sources=['./cNetwork.cpp'])

setup(name='cNetwork',
	  version='1.0',
	  description="Module for calculating output of xor sbox system",
	  ext_modules=[cNetwork])