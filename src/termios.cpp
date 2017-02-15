#include <nan.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#define EXPORT_SYMBOL(o, s) o->Set(Nan::New<v8::String>(#s).ToLocalChecked(), Nan::New<v8::Number>(s));

static struct bauds {
	int baud, mask;
} bauds[] =
{
{0, B0},
{50, B50},
{75, B75},
{110, B110},
{134, B134},
{150, B150},
{200, B200},
{300, B300},
{600, B600},
{1200, B1200},
{1800, B1800},
{2400, B2400},
{4800, B4800},
{9600, B9600},
{19200, B19200},
{38400, B38400},
{57600, B57600},
{115200, B115200},
{230400, B230400},
#ifndef __APPLE__
{460800, B460800},
{500000, B500000},
{576000, B576000},
{921600, B921600},
{1000000, B1000000},
{1152000, B1152000},
{1500000, B1500000},
{2000000, B2000000},
{2500000, B2500000},
{3000000, B3000000},
{3500000, B3500000},
{4000000, B4000000},
#endif
{-1, 0},
};

#ifndef CBAUD
#define CBAUD 0000017
#endif

static int mask_to_baud(int mask) {
	int i;
	for(i=0;bauds[i].baud >= 0;++i) {
		if(mask == bauds[i].mask)
			return bauds[i].baud;
	}
	return -1;
}

static int baud_to_mask(int baud) {
	int i;
	for(i=0;bauds[i].baud >= 0;++i) {
		if(baud == bauds[i].baud)
			return bauds[i].mask;
	}
	return 0;
}

NAN_METHOD(Setattr) {
	struct termios t;
	Nan::HandleScope scope;
	v8::Local<v8::Object> obj = info[2]->ToObject();
	v8::Local<v8::String> iflag = Nan::New<v8::String>("iflag").ToLocalChecked();
	v8::Local<v8::String> oflag = Nan::New<v8::String>("oflag").ToLocalChecked();
	v8::Local<v8::String> cflag = Nan::New<v8::String>("cflag").ToLocalChecked();
	v8::Local<v8::String> cbaud = Nan::New<v8::String>("cbaud").ToLocalChecked();
	v8::Local<v8::String> lflag = Nan::New<v8::String>("lflag").ToLocalChecked();

	if(!info[0]->IsNumber())
		return Nan::ThrowError("fd must be a number");
	if(tcgetattr(info[0]->Uint32Value(), &t) < 0)
		return Nan::ThrowError(strerror(errno));
	if(obj->Has(iflag))
		t.c_iflag = obj->Get(iflag)->Uint32Value();
	if(obj->Has(oflag))
		t.c_oflag = obj->Get(oflag)->Uint32Value();
	if(obj->Has(cflag))
		t.c_cflag = obj->Get(cflag)->Uint32Value();
	if(obj->Has(cbaud)) {
		int mask = baud_to_mask(obj->Get(cbaud)->Uint32Value());
		if(mask>=0) {
			t.c_cflag &= ~CBAUD;
			t.c_cflag |= mask;
		} else
			return Nan::ThrowError("invalid baud value");
	}
	if(obj->Has(lflag))
		t.c_lflag = obj->Get(lflag)->Uint32Value();
	if(tcsetattr(info[0]->Uint32Value(), TCSADRAIN, &t) < 0)
		return Nan::ThrowError(strerror(errno));
}

NAN_METHOD(Getattr) {
	struct termios t;
	Nan::HandleScope scope;
	if(tcgetattr(info[0]->Uint32Value(), &t) < 0)
		return Nan::ThrowError(strerror(errno));
	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	obj->Set(Nan::New<v8::String>("iflag").ToLocalChecked(), Nan::New<v8::Number>(t.c_iflag));
	obj->Set(Nan::New<v8::String>("oflag").ToLocalChecked(), Nan::New<v8::Number>(t.c_oflag));
	obj->Set(Nan::New<v8::String>("cflag").ToLocalChecked(), Nan::New<v8::Number>(t.c_cflag));
	int baud = mask_to_baud(t.c_cflag & CBAUD);
	if(baud>=0)
		obj->Set(Nan::New<v8::String>("cbaud").ToLocalChecked(), Nan::New<v8::Number>(baud));
	obj->Set(Nan::New<v8::String>("lflag").ToLocalChecked(), Nan::New<v8::Number>(t.c_lflag));

	info.GetReturnValue().Set(obj);
}

void Init(v8::Handle<v8::Object> exports) {
	v8::Handle<v8::Object> modes;

	Nan::SetMethod(exports, "setattr", Setattr);
	Nan::SetMethod(exports, "getattr", Getattr);

	EXPORT_SYMBOL(exports, TCSANOW);
	EXPORT_SYMBOL(exports, TCSADRAIN);
	EXPORT_SYMBOL(exports, TCSAFLUSH);

	/* Input Modes */
	modes = Nan::New<v8::Object>();
	EXPORT_SYMBOL(modes, IGNBRK);  /* ignore BREAK condition */
	EXPORT_SYMBOL(modes, BRKINT);  /* map BREAK to SIGINT */
	EXPORT_SYMBOL(modes, IGNPAR);  /* ignore (discard) parity errors */
	EXPORT_SYMBOL(modes, PARMRK);  /* mark parity and framing errors */
	EXPORT_SYMBOL(modes, INPCK);   /* enable checking of parity errors */
	EXPORT_SYMBOL(modes, ISTRIP);  /* strip 8th bit off chars */
	EXPORT_SYMBOL(modes, INLCR);   /* map NL into CR */
	EXPORT_SYMBOL(modes, IGNCR);   /* ignore CR */
	EXPORT_SYMBOL(modes, ICRNL);   /* map CR to NL (ala CRMOD) */
	EXPORT_SYMBOL(modes, IXON);    /* enable output flow control */
	EXPORT_SYMBOL(modes, IXOFF);   /* enable input flow control */
	EXPORT_SYMBOL(modes, IXANY);   /* any char will restart after stop */
	EXPORT_SYMBOL(modes, IMAXBEL); /* ring bell on input queue full */
	//EXPORT_SYMBOL(modes, IUCLC);   /* translate upper case to lower case */
	exports->Set(Nan::New<v8::String>("iflag").ToLocalChecked(), modes);

	/* Output Modes */
	modes = Nan::New<v8::Object>();
	EXPORT_SYMBOL(modes, OPOST);   /* enable following output processing */
	EXPORT_SYMBOL(modes, ONLCR);   /* map NL to CR-NL (ala CRMOD) */
	EXPORT_SYMBOL(modes, OCRNL);   /* map CR to NL */
	//EXPORT_SYMBOL(modes, OLCUC);   /* translate lower case to upper case */
	EXPORT_SYMBOL(modes, ONOCR);   /* No CR output at column 0 */
	EXPORT_SYMBOL(modes, ONLRET);  /* NL performs the CR function */
	exports->Set(Nan::New<v8::String>("oflag").ToLocalChecked(), modes);

	/* Control Modes */
	modes = Nan::New<v8::Object>();
	EXPORT_SYMBOL(modes, CSIZE);   /* character size mask */
	EXPORT_SYMBOL(modes, CS5);     /* 5 bits (pseudo) */
	EXPORT_SYMBOL(modes, CS6);     /* 6 bits */
	EXPORT_SYMBOL(modes, CS7);     /* 7 bits */
	EXPORT_SYMBOL(modes, CS8);     /* 8 bits */
	EXPORT_SYMBOL(modes, CSTOPB);  /* send 2 stop bits */
	EXPORT_SYMBOL(modes, CREAD);   /* enable receiver */
	EXPORT_SYMBOL(modes, PARENB);  /* parity enable */
	EXPORT_SYMBOL(modes, PARODD);  /* odd parity, else even */
	EXPORT_SYMBOL(modes, HUPCL);   /* hang up on last close */
	EXPORT_SYMBOL(modes, CLOCAL);  /* ignore modem status lines */
	exports->Set(Nan::New<v8::String>("cflag").ToLocalChecked(), modes);

	/* Local Modes */
	modes = Nan::New<v8::Object>();
	EXPORT_SYMBOL(modes, ECHOKE);  /* visual erase for line kill */
	EXPORT_SYMBOL(modes, ECHOE);   /* visually erase chars */
	EXPORT_SYMBOL(modes, ECHOK);   /* echo NL after line kill */
	EXPORT_SYMBOL(modes, ECHO);    /* enable echoing */
	EXPORT_SYMBOL(modes, ECHONL);  /* echo NL even if ECHO is off */
	EXPORT_SYMBOL(modes, ECHOPRT); /* visual erase mode for hardcopy */
	EXPORT_SYMBOL(modes, ECHOCTL); /* echo control chars as ^(Char) */
	EXPORT_SYMBOL(modes, ISIG);    /* enable signals INTR, QUIT, [D]SUSP */
	EXPORT_SYMBOL(modes, ICANON);  /* canonicalize input lines */
	EXPORT_SYMBOL(modes, IEXTEN);  /* enable DISCARD and LNEXT */
#ifdef EXTPROC
	EXPORT_SYMBOL(modes, EXTPROC); /* external processing */
#endif
	EXPORT_SYMBOL(modes, TOSTOP);  /* stop background jobs from output */
	EXPORT_SYMBOL(modes, FLUSHO);  /* output being flushed (state) */
	EXPORT_SYMBOL(modes, PENDIN);  /* XXX retype pending input (state) */
	EXPORT_SYMBOL(modes, NOFLSH);  /* don't flush after interrupt */
	//EXPORT_SYMBOL(modes, XCASE);   /* canonical upper/lower case */
	exports->Set(Nan::New<v8::String>("lflag").ToLocalChecked(), modes);
}

NODE_MODULE(pty, Init)
