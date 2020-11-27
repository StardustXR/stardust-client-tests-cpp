project('create_model', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')

#include = include_directories('include')
sources = ['src/create_model.cpp']
dependencies = [libstardustxr]

executable('create_model',
	sources,
#	include_directories: include,
	link_args: '-lflatbuffers',
	dependencies: dependencies,
)
