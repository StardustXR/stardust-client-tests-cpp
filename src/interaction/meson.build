interaction_includes = include_directories('.')

lib_interaction = static_library(
	'interaction',
	files(
                'button.cpp',
                'grabbable.cpp',
		'mediaplayer.cpp',
		'screen.cpp',
		'slider.cpp',
		'xinteract.cpp',
	),
	include_directories: [interaction_includes],
	dependencies: [fusion],
)

sublibs += lib_interaction
