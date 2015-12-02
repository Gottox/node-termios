{
	'targets': [{
		'target_name': 'termios',
		"sources": [ "src/termios.cpp" ],
		'default_configuration': 'Release',
		"include_dirs" : [
			"<!(node -e \"require('nan')\")"
		]
	}]
}
