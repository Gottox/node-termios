#include <nan.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#define EXPORT_SYMBOL(o, s) o->Set(Nan::New<v8::String>(#s).ToLocalChecked(), Nan::New<v8::Number>(s));

NAN_METHOD(Setattr) {
	struct termios t;
	Nan::HandleScope scope;
	bzero(&t, sizeof(struct termios));
	v8::Local<v8::Object> obj = info[2]->ToObject();

	if(obj->Has(Nan::New<v8::String>("iflag").ToLocalChecked()))
		t.c_iflag = obj->Get(Nan::New<v8::String>("iflag").ToLocalChecked())->Uint32Value();
	if(obj->Has(Nan::New<v8::String>("oflag").ToLocalChecked()))
		t.c_oflag = obj->Get(Nan::New<v8::String>("oflag").ToLocalChecked())->Uint32Value();
	if(obj->Has(Nan::New<v8::String>("cflag").ToLocalChecked()))
		t.c_cflag = obj->Get(Nan::New<v8::String>("cflag").ToLocalChecked())->Uint32Value();
	if(obj->Has(Nan::New<v8::String>("lflag").ToLocalChecked()))
		t.c_lflag = obj->Get(Nan::New<v8::String>("lflag").ToLocalChecked())->Uint32Value();
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
	EXPORT_SYMBOL(modes, EXTPROC); /* external processing */
	EXPORT_SYMBOL(modes, TOSTOP);  /* stop background jobs from output */
	EXPORT_SYMBOL(modes, FLUSHO);  /* output being flushed (state) */
	EXPORT_SYMBOL(modes, PENDIN);  /* XXX retype pending input (state) */
	EXPORT_SYMBOL(modes, NOFLSH);  /* don't flush after interrupt */
	//EXPORT_SYMBOL(modes, XCASE);   /* canonical upper/lower case */
	exports->Set(Nan::New<v8::String>("lflag").ToLocalChecked(), modes);
}

NODE_MODULE(pty, Init)
