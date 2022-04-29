environments_resources = [
	'comfy_cafe.hdr',
	'DefaultEnvironment.glb',
	'earth.hdr',
	'lakemichigan.jpg',
	'space.hdr',
]

custom_target(
	'cp_resources',
	input : environments_resources,
	output : environments_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)