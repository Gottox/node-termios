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

function loadModes(bf, masks) {
	var keys = Object.keys(masks), res = {};
	for(var i = 0; i < keys.length; i++) {
		res[keys[i]] = !!(bf & masks[keys[i]]);
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
		cbaud: modes.cbaud,
		lflag: loadModes(modes.lflag, termios.lflag)
	};
};
