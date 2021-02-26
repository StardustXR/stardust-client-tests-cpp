project('stardustxr-client-tests', 'cpp', default_options : ['cpp_std=c++11'])
includes = include_directories('include')

libstardustxr = dependency('stardustxr')
dependencies = [libstardustxr]

executable('create_model',
	'src/create_model.cpp',
	link_args: '-lflatbuffers',
	include_directories: includes,
	dependencies: dependencies,
)

executable('fields',
	'src/fields.cpp',
	link_args: '-lflatbuffers',
	include_directories: includes,
	dependencies: dependencies,
)

executable('input',
	'src/input.cpp',
	link_args: '-lflatbuffers',
	include_directories: includes,
	dependencies: dependencies,
)
