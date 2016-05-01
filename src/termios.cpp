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
	v8::Local<v8::String> iflag = Nan::New<v8::String>("iflag").ToLocalChecked();
	v8::Local<v8::String> oflag = Nan::New<v8::String>("oflag").ToLocalChecked();
	v8::Local<v8::String> cflag = Nan::New<v8::String>("cflag").ToLocalChecked();
	v8::Local<v8::String> lflag = Nan::New<v8::String>("lflag").ToLocalChecked();
	v8::Local<v8::String> cc = Nan::New<v8::String>("cc").ToLocalChecked();

	if(!info[0]->IsNumber())
		return Nan::ThrowError("fd must be a number");
	if(obj->Has(iflag))
		t.c_iflag = obj->Get(iflag)->Uint32Value();
	if(obj->Has(oflag))
		t.c_oflag = obj->Get(oflag)->Uint32Value();
	if(obj->Has(cflag))
		t.c_cflag = obj->Get(cflag)->Uint32Value();
	if(obj->Has(lflag))
		t.c_lflag = obj->Get(lflag)->Uint32Value();

	if(obj->Has(cc)) {
		v8::Local<v8::Object> obj_cc = obj->Get(cc)->ToObject();
		if (obj_cc->Has(VDISCARD))
			t.c_cc[VDISCARD] = obj_cc->Get(VDISCARD)->Uint32Value();
		else
			t.c_cc[VDISCARD] = _POSIX_VDISABLE;
		if (obj_cc->Has(VDSUSP))
			t.c_cc[VDSUSP] = obj_cc->Get(VDSUSP)->Uint32Value();
		else
			t.c_cc[VDSUSP] = _POSIX_VDISABLE;
		if (obj_cc->Has(VEOF))
			t.c_cc[VEOF] = obj_cc->Get(VEOF)->Uint32Value();
		else
			t.c_cc[VEOF] = _POSIX_VDISABLE;
		if (obj_cc->Has(VEOL))
			t.c_cc[VEOL] = obj_cc->Get(VEOL)->Uint32Value();
		else
			t.c_cc[VEOL] = _POSIX_VDISABLE;
		if (obj_cc->Has(VEOL2))
			t.c_cc[VEOL2] = obj_cc->Get(VEOL2)->Uint32Value();
		else
			t.c_cc[VEOL2] = _POSIX_VDISABLE;
		if (obj_cc->Has(VERASE))
			t.c_cc[VERASE] = obj_cc->Get(VERASE)->Uint32Value();
		else
			t.c_cc[VERASE] = _POSIX_VDISABLE;
		if (obj_cc->Has(VINTR))
			t.c_cc[VINTR] = obj_cc->Get(VINTR)->Uint32Value();
		else
			t.c_cc[VINTR] = _POSIX_VDISABLE;
		if (obj_cc->Has(VKILL))
			t.c_cc[VKILL] = obj_cc->Get(VKILL)->Uint32Value();
		else
			t.c_cc[VKILL] = _POSIX_VDISABLE;
		if (obj_cc->Has(VLNEXT))
			t.c_cc[VLNEXT] = obj_cc->Get(VLNEXT)->Uint32Value();
		else
			t.c_cc[VLNEXT] = _POSIX_VDISABLE;
		if (obj_cc->Has(VMIN))
			t.c_cc[VMIN] = obj_cc->Get(VMIN)->Uint32Value();
		else
			t.c_cc[VMIN] = _POSIX_VDISABLE;
		if (obj_cc->Has(VQUIT))
			t.c_cc[VQUIT] = obj_cc->Get(VQUIT)->Uint32Value();
		else
			t.c_cc[VQUIT] = _POSIX_VDISABLE;
		if (obj_cc->Has(VREPRINT))
			t.c_cc[VREPRINT] = obj_cc->Get(VREPRINT)->Uint32Value();
		else
			t.c_cc[VREPRINT] = _POSIX_VDISABLE;
		if (obj_cc->Has(VSTART))
			t.c_cc[VSTART] = obj_cc->Get(VSTART)->Uint32Value();
		else
			t.c_cc[VSTART] = _POSIX_VDISABLE;
		if (obj_cc->Has(VSTATUS))
			t.c_cc[VSTATUS] = obj_cc->Get(VSTATUS)->Uint32Value();
		else
			t.c_cc[VSTATUS] = _POSIX_VDISABLE;
		if (obj_cc->Has(VSTOP))
			t.c_cc[VSTOP] = obj_cc->Get(VSTOP)->Uint32Value();
		else
			t.c_cc[VSTOP] = _POSIX_VDISABLE;
		if (obj_cc->Has(VSUSP))
			t.c_cc[VSUSP] = obj_cc->Get(VSUSP)->Uint32Value();
		else
			t.c_cc[VSUSP] = _POSIX_VDISABLE;
		if (obj_cc->Has(VTIME))
			t.c_cc[VTIME] = obj_cc->Get(VTIME)->Uint32Value();
		else
			t.c_cc[VTIME] = _POSIX_VDISABLE;
		if (obj_cc->Has(VWERASE))
			t.c_cc[VWERASE] = obj_cc->Get(VWERASE)->Uint32Value();
		else
			t.c_cc[VWERASE] = _POSIX_VDISABLE;
	}

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
	v8::Local<v8::Object> obj_cc = Nan::New<v8::Object>();
	obj->Set(Nan::New<v8::String>("cc").ToLocalChecked(), obj_cc);

	if (t.c_cc[VDISCARD] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VDISCARD").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VDISCARD]));
	if (t.c_cc[VDSUSP] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VDSUSP").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VDSUSP]));
	if (t.c_cc[VEOF] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VEOF").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VEOF]));
	if (t.c_cc[VEOL] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VEOL").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VEOL]));
	if (t.c_cc[VEOL2] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VEOL2").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VEOL2]));
	if (t.c_cc[VERASE] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VERASE").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VERASE]));
	if (t.c_cc[VINTR] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VINTR").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VINTR]));
	if (t.c_cc[VKILL] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VKILL").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VKILL]));
	if (t.c_cc[VLNEXT] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VLNEXT").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VLNEXT]));
	if (t.c_cc[VMIN] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VMIN").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VMIN]));
	if (t.c_cc[VQUIT] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VQUIT").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VQUIT]));
	if (t.c_cc[VREPRINT] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VREPRINT").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VREPRINT]));
	if (t.c_cc[VSTART] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VSTART").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VSTART]));
	if (t.c_cc[VSTATUS] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VSTATUS").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VSTATUS]));
	if (t.c_cc[VSTOP] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VSTOP").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VSTOP]));
	if (t.c_cc[VSUSP] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VSUSP").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VSUSP]));
	if (t.c_cc[VTIME] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VTIME").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VTIME]));
	if (t.c_cc[VWERASE] != _POSIX_VDISABLE)
		obj_cc->Set(Nan::New<v8::String>("VWERASE").ToLocalChecked(), Nan::New<v8::Number>(t.c_cc[VWERASE]));

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
	EXPORT_SYMBOL(modes, IUTF8);   /* input is utf8 */
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

	/* Control Characters */
	modes = Nan::New<v8::Object>();
	EXPORT_SYMBOL(modes, VDISCARD);/* start/stop discarding pending output */
	EXPORT_SYMBOL(modes, VDSUSP);  /* delayed suspend */
	EXPORT_SYMBOL(modes, VEOF);    /* end of file */
	EXPORT_SYMBOL(modes, VEOL);    /* additional end-of-line */
	EXPORT_SYMBOL(modes, VEOL2);   /* another additional end-of-line */
	EXPORT_SYMBOL(modes, VERASE);  /* erase character */
	EXPORT_SYMBOL(modes, VINTR);   /* send a SIGINT signal */
	EXPORT_SYMBOL(modes, VKILL);   /* erase input */
	EXPORT_SYMBOL(modes, VLNEXT);  /* literal next */
	EXPORT_SYMBOL(modes, VMIN);    /* min characters for noncanonical read */
	EXPORT_SYMBOL(modes, VQUIT);   /* send SIGQUIT signal */
	EXPORT_SYMBOL(modes, VREPRINT);/* reprint unread characters */
	EXPORT_SYMBOL(modes, VSTART);  /* resume output */
	EXPORT_SYMBOL(modes, VSTATUS); /* foreground process character */
	EXPORT_SYMBOL(modes, VSTOP);   /* pause output */
	EXPORT_SYMBOL(modes, VSUSP);   /* send SIGTSTP signal */
	EXPORT_SYMBOL(modes, VTIME);   /* timeout in 0.1s for noncanonical read */
	EXPORT_SYMBOL(modes, VWERASE); /* word erase */
	exports->Set(Nan::New<v8::String>("cc").ToLocalChecked(), modes);
}

NODE_MODULE(pty, Init)
