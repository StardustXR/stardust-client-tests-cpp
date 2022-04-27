project('stardustxr-client-tests', 'cpp', default_options : ['cpp_std=c++17'])
includes = include_directories('include')

fusion = dependency('stardustxr-fusion')
flatbuffers = dependency('flatbuffers', version: '>=2.0.6')
dbus = dependency('sdbus-c++', required : false)
xcb = dependency('xcb')
xcb_xinput = dependency('xcb-xinput')
xkb = dependency('xkbcommon')
xkb_x11 = dependency('xkbcommon-x11')

dependencies = [fusion]

common_files = []
dbus_files = []

subdir('src/dbus')
subdir('src/interaction')
subdir('src/item')
subdir('src/physics')

executable('axes',
        ['src/axes.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)

executable('basicbody',
        ['src/basicbody.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)

# executable('environmentitem_selfimpl',
# 	'src/environmentitem_selfimpl.cpp',
# 	include_directories: includes,
# 	link_whole: sublibs,
# 	dependencies: dependencies,
# )

executable('environmentitemspawner',
        ['src/environmentitemspawner.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)

executable('environmentitemui',
        ['src/environmentitemui.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)

executable('floor',
        ['src/floor.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('hostkeyboard',
        ['src/hostkeyboard.cpp', common_files],
        include_directories: includes,
        dependencies: [dependencies, flatbuffers, xcb, xcb_xinput, xkb, xkb_x11],
)

executable('inputactions',
        ['src/inputactions.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('inputbenchmark',
        ['src/inputbenchmark.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

if dbus.found()
executable('mediaplayer',
        ['src/mediaplayer.cpp', common_files, dbus_files],
        include_directories: includes,
        dependencies: [dependencies, dbus],
)
endif

executable('observatory',
        ['src/observatory.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('panelshell',
        ['src/panelshell.cpp', common_files],
        include_directories: includes,
        dependencies: [dependencies, flatbuffers],
)

executable('panelui',
        ['src/panelui.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('patternlock',
        ['src/patternlock.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('screen',
        ['src/screen.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)

executable('scrollpanel',
        ['src/scrollpanel.cpp', common_files],
        include_directories: includes,
        dependencies: [dependencies, xcb, xkb, xkb_x11],
)

executable('slider',
        ['src/slider.cpp', common_files],
        include_directories: includes,
        dependencies: dependencies,
)

executable('xinteract',
        ['src/xinteract.cpp', common_files],
        include_directories: includes,
	dependencies: dependencies,
)
