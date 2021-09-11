item_includes = include_directories('.')

lib_item = static_library(
	'item',
	files(
		'item.cpp',
		'environmentitem.cpp'
	),
	include_directories: [item_includes],
	dependencies: [fusion],
)

sublibs += lib_item
