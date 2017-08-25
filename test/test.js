var assert = require('assert');
var termios = require('../index');

describe('termios.getattr()', function() {
	assert.ok(termios.getattr(process.stdin.fd));
});
