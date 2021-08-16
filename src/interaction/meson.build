interaction_includes = include_directories('.')

lib_interaction = static_library(
	'interaction',
	files(
		'screen.cpp',
		'slider.cpp',
	),
	include_directories: [interaction_includes],
	dependencies: [fusion],
)

sublibs += lib_interaction
