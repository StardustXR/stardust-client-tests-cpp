project('simple_input_handler', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')
dependencies = [libstardustxr]

lifecycle_sources = ['src/simple_input_handler.cpp']
executable('simple_input_handler',
	lifecycle_sources,
	link_args: '-lflatbuffers',
	dependencies: dependencies,
)
