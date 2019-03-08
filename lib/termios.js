var termios = require('../build/Release/termios.node');

function applyModes(bf, obj, masks) {
	if(!obj) return bf;
	var keys = Object.keys(obj);
	for(var i = 0; i < keys.length; i++) {
		if(obj[keys[i]])
			bf |= masks[keys[i]];
		else
			bf &= ~masks[keys[i]];
	}
	return bf;
}

function applyArray(res, values, mapping) {
	var keys = Object.keys(values);
	res = res || {};
	for(var i = 0; i < keys.length; i++) {
		res[mapping[keys[i]]] = values[keys[i]];
	}
	return res;
}

function loadModes(bf, masks) {
	var keys = Object.keys(masks), res = {};
	for(var i = 0; i < keys.length; i++) {
		res[keys[i]] = !!(bf & masks[keys[i]]);
	}
	return res;
}

function loadArray(values, mapping) {
	var keys = Object.keys(mapping), res = {};
	for(var i = 0; i < keys.length; i++) {
		res[keys[i]] = values[keys[i]];
	}
	return res;
}

/**
* sets the parameters associated with the terminal
*/
module.exports.setattr = function(fd, attr) {
	var modes = termios.getattr(fd);
	modes.iflag = applyModes(modes.iflag, attr.iflag, termios.iflag);
	modes.oflag = applyModes(modes.oflag, attr.oflag, termios.oflag);
	modes.cflag = applyModes(modes.cflag, attr.cflag, termios.cflag);
	modes.lflag = applyModes(modes.lflag, attr.lflag, termios.lflag);
	modes.cc = applyArray(modes.cc, attr.cc, termios.cc);
	if(attr.cbaud)
		modes.cbaud = attr.cbaud;
	return termios.setattr(fd, 0, modes);
};

/**
* gets the parameters associated with the terminal
*/
module.exports.getattr = function(fd) {
	var modes = termios.getattr(fd);
	return {
		iflag: loadModes(modes.iflag, termios.iflag),
		oflag: loadModes(modes.oflag, termios.oflag),
		cflag: loadModes(modes.cflag, termios.cflag),
		lflag: loadModes(modes.lflag, termios.lflag),
		cc: loadArray(modes.cc, termios.cc),
		cbaud: modes.cbaud,
	};
};
