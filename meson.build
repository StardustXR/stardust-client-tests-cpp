project('stardustxr-client-tests', 'cpp', default_options : ['cpp_std=c++11'])
includes = include_directories('include')

fusion = dependency('stardustxr-fusion')
dependencies = [fusion]

sublibs = []

subdir('src/interaction')
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

executable('environmentitem',
	'src/environmentitem.cpp',
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
	link_args: '-lflatbuffers',
	dependencies: dependencies,
)
