interaction_includes = include_directories('.')

lib_interaction = static_library(
	'interaction',
	files(
		'contactable.cpp',
		'grabbable.cpp',
		'screen.cpp',
		'slider.cpp',
	),
	include_directories: [interaction_includes],
	dependencies: [fusion],
	link_args: '-lflatbuffers',
)

sublibs += lib_interaction
