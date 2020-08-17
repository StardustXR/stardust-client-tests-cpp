project('mesh_scale', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')

#include = include_directories('include')
sources = ['src/mesh_scale.cpp']
dependencies = [libstardustxr]

executable('mesh_scale',
	sources,
#	include_directories: include,
	dependencies: dependencies,
)
