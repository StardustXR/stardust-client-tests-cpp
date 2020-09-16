project('fields', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')
dependencies = [libstardustxr]

lifecycle_sources = ['src/fields.cpp']
executable('fields',
	lifecycle_sources,
	link_args: '-lflatbuffers',
	dependencies: dependencies,
)
