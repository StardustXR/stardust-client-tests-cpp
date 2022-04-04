item_includes = include_directories('.')

lib_item = static_library(
	'item',
        files(
                'environmentitem.cpp',
                'panel.cpp',
	),
	include_directories: [item_includes],
	dependencies: [fusion],
)

sublibs += lib_item
