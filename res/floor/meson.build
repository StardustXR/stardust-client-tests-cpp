floor_resources = [
	'floor.glb',
	'square_floor.glb',
]

custom_target(
	'cp_resources',
	input : floor_resources,
	output : floor_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)