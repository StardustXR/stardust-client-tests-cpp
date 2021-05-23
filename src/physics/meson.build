physics_includes = include_directories('.')

lib_physics = static_library(
	'physics',
	files(
		'physicsbody.cpp',
	),
	include_directories: [physics_includes],
	dependencies: [fusion],
)

sublibs += lib_physics
