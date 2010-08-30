// node-fork
// Ryan W Tenney (ryan@10e.us)
// http://ryan.10e.us/

#include <v8.h>
#include <unistd.h>
#include "ev.h"

using namespace v8;


static Handle<Value>
fork_fn (const Arguments& args)
{
	HandleScope scope;

	int pid = fork();
	ev_default_fork();
	return Number::New(pid);
}


extern "C" void
init (Handle<Object> target)
{
	HandleScope scope;

	Local<String> _fork = String::NewSymbol("fork");

	Local<FunctionTemplate> t;
	Local<Function> f;

	t = FunctionTemplate::New(fork_fn);
	f = t->GetFunction();

	f->SetName(_fork);
	target->Set(_fork, f);
}
