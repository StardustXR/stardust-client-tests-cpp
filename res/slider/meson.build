slider_resources = [
	'base.glb',
	'orb.glb',
	'pincher.glb',
]

custom_target(
	'cp_resources',
	input : slider_resources,
	output : slider_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)