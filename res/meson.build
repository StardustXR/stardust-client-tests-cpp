resources = [
	'axes.glb',
	'discord_panel.glb',
	'dome.glb',
	'gem.glb',
	'realitycomposer.glb',
	'touch_dome.glb',
]

custom_target(
	'cp_resources',
	input : resources,
	output : resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)

subdir('environments')
subdir('floor')
subdir('hexy')
subdir('item')
subdir('mediaplayer')
subdir('panelshell')
subdir('slider')