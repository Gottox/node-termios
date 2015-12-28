[![Build Status](https://travis-ci.org/Gottox/node-termios.png)](https://travis-ci.org/Gottox/node-termios)

termios bindings for node.js
============================

node-termios is a module for getting and setting terminal attributes.

API
---


* an `attr` object may contain on or more of the following boolean fields:
  * `attr.iflag`: 
    * `IGNBRK`: ignore BREAK condition
    * `BRKINT`: map BREAK to SIGINT
    * `IGNPAR`: ignore (discard) parity errors
    * `PARMRK`: mark parity and framing errors
    * `INPCK`: enable checking of parity errors
    * `ISTRIP`: strip 8th bit off chars
    * `INLCR`: map NL into CR
    * `IGNCR`: ignore CR
    * `ICRNL`: map CR to NL (ala CRMOD)
    * `IXON`: enable output flow control
    * `IXOFF`: enable input flow control
    * `IXANY`: any char will restart after stop
    * `IMAXBEL`: ring bell on input queue full
  * `attr.oflag`: 
    * `OPOST`: enable following output processing
    * `ONLCR`: map NL to CR-NL (ala CRMOD)
    * `OCRNL`: map CR to NL
    * `ONOCR`: No CR output at column 0
    * `ONLRET`: NL performs the CR function
  * `attr.cflag`: 
    * `CSIZE`: character size mask
    * `CS5`: 5 bits (pseudo)
    * `CS6`: 6 bits
    * `CS7`: 7 bits
    * `CS8`: 8 bits
    * `CSTOPB`: send 2 stop bits
    * `CREAD`: enable receiver
    * `PARENB`: parity enable
    * `PARODD`: odd parity, else even
    * `HUPCL`: hang up on last close
    * `CLOCAL`: ignore modem status lines
  * `attr.lflag`: 
    * `ECHOKE`: visual erase for line kill
    * `ECHOE`: visually erase chars
    * `ECHOK`: echo NL after line kill
    * `ECHO`: enable echoing
    * `ECHONL`: echo NL even if ECHO is off
    * `ECHOPRT`: visual erase mode for hardcopy
    * `ECHOCTL`: echo control chars as ^(Char)
    * `ISIG`: enable signals INTR, QUIT, [D]SUSP
    * `ICANON`: canonicalize input lines
    * `IEXTEN`: enable DISCARD and LNEXT
    * `EXTPROC`: external processing
    * `TOSTOP`: stop background jobs from output
    * `FLUSHO`: output being flushed (state)
    * `PENDIN`: XXX retype pending input (state)
    * `NOFLSH`: don't flush after interrupt
    * `XCASE`: canonical upper/lower case

* `.setattr(fd, attr)`: Sets attributes of the terminal bound to the
  file descriptor:
  * fd: file descriptor
  * attr: object as described above
  * returns: undefined

* `.getattr(fd)`: Returns an object describing the current settings
  of the terminal bound to the file descriptor:
  * fd: file descriptor

Examples
--------

```
var termios = require('termios'), data = '';
console.log("Please type something:");
process.stdin.on("data", function(d) {data += d.toString()});
setTimeout(function() {
	console.log("Disabling ECHO. You won't see what you type now");
	termios.setattr(process.stdin.fd, {lflag: { ECHO: false }})
	console.log("\nYou typed '"+data+"'");
	data = "";
}, 3000);
setTimeout(function() {
	console.log("Enabling ECHO.");
	termios.setattr(process.stdin.fd, {lflag: { ECHO: false }})
	console.log("\nYou typed '"+data+"'");
}, 6000);
```

Changelog
---------

* v0.1 - initial release
