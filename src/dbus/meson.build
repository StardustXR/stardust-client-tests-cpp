dbus_includes = include_directories('.')

lib_dbus = static_library(
	'dbus',
	files(
		'player.cpp',
	),
	include_directories: [dbus_includes],
	dependencies: [dbus],
)

sublibs += lib_dbus
