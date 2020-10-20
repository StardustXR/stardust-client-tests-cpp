project('set_values', 'cpp', default_options : ['cpp_std=c++11'])

libstardustxr = dependency('stardustxr')
dependencies = [libstardustxr]

# Position
set_position_sources = ['src/set_position.cpp']
executable('set_position',
	set_position_sources,
	link_args: '-lflatbuffers',
#	include_directories: include,
	dependencies: dependencies,
)
#
# # Rotation
# set_rotation_sources = ['src/set_rotation.cpp']
# executable('set_rotation',
# 	set_rotation_sources,
# 	link_args: '-lflatbuffers',
# #	include_directories: include,
# 	dependencies: dependencies,
# )
