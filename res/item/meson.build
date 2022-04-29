item_resources = [
	'crt.glb',
	'environmentitemacceptor.glb',
	'floppydisk.glb',
	'panelitem.glb',
	'scrollpanel.glb',
	'scrollpanelhandle.glb',
	'skyball.glb',
]

custom_target(
	'cp_resources',
	input : item_resources,
	output : item_resources,
	command : ['cp', '@INPUT@', '@OUTDIR@'],
	install : false,
	build_by_default : true
)