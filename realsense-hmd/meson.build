project('realsense-hmd', 'cpp')

libstardustxr = dependency('stardustxr-server')

include = include_directories('include')
sources = ['src/realsense-client.cpp']
dependencies = [libstardustxr]

shared_library('realsense-hmd',
	sources,
	include_directories: include,
	link_args: '-lflatbuffers',
	dependencies: dependencies
)
