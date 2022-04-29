panel_shell_resources = [
	'crt.glb',
]

custom_target(
	'cp_resources',
	input : panel_shell_resources,
	output : panel_shell_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)