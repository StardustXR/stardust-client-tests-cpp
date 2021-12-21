project('stardustxr-client-tests', 'cpp', default_options : ['cpp_std=c++17'])
includes = include_directories('include')

fusion = dependency('stardustxr-fusion')
dbus = dependency('sdbus-c++')

dependencies = [fusion]

sublibs = []

subdir('src/dbus')
subdir('src/interaction')
subdir('src/item')
subdir('src/physics')

executable('axes',
	'src/axes.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('basicbody',
	'src/basicbody.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('directactions',
	'src/directactions.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

# executable('environmentitem_selfimpl',
# 	'src/environmentitem_selfimpl.cpp',
# 	include_directories: includes,
# 	link_whole: sublibs,
# 	dependencies: dependencies,
# )

executable('environmentitemspawner',
	'src/environmentitemspawner.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('environmentitemui',
	'src/environmentitemui.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('floor',
	'src/floor.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('mediaplayer',
        'src/mediaplayer.cpp',
        include_directories: includes,
        link_whole: sublibs,
        dependencies: [
                dependencies,
                dbus,
        ],
)

executable('observatory',
        'src/observatory.cpp',
        include_directories: includes,
        link_whole: sublibs,
        dependencies: dependencies,
)

executable('panelui',
        'src/panelui.cpp',
        include_directories: includes,
        link_whole: sublibs,
        dependencies: dependencies,
)

executable('patternlock',
        'src/patternlock.cpp',
        include_directories: includes,
        link_whole: sublibs,
        dependencies: dependencies,
)

executable('screen',
	'src/screen.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('slider',
	'src/slider.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)

executable('xinteract',
	'src/xinteract.cpp',
	include_directories: includes,
	link_whole: sublibs,
	dependencies: dependencies,
)
