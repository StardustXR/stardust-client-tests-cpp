media_player_resources = [
	'body.glb',
	'notrack.png',
	'music.svg',
]

custom_target(
	'cp_resources',
	input : media_player_resources,
	output : media_player_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)