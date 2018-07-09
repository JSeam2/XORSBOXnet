from distutils.core import setup, Extension

cNetwork = Extension('cNetwork', sources=['./cNetwork.cpp'])

setup(name='cNetwork',
	  version='1.0',
	  description="Module for calculating tanh",
	  ext_modules=[cNetwork])