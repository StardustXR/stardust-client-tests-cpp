project('lifecycle', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')
dependencies = [libstardustxr]

lifecycle_sources = ['src/lifecycle.cpp']
executable('lifecycle',
	lifecycle_sources,
	link_args: '-lflatbuffers',
	dependencies: dependencies,
)
