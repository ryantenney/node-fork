// node-fork v0.0.2
// Ryan W Tenney (ryan@10e.us)
// http://github.com/ryantenney/node-fork/

// Copyright (c) 2010 Ryan W Tenney
// Distributed under the MIT License, see LICENSE

// Thanks to http://www.theorie.physik.unizh.ch/~dpotter/howto/daemonize
// for the function daemonize, modified slightly to fit this purpose.
// daemonize() function source released into the Public Domain by Doug Potter

// TODO: a second call to fork() may be required to fully detach the process from the controller terminal.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include <v8.h>
#include <unistd.h>
#include "ev.h"

#include "fork.h"


using namespace v8;


extern "C" void
init (Handle<Object> target)
{
	HandleScope scope;

	EXPORTS_FN("fork", fork_fn);
	EXPORTS_FN("daemonize", daemonize_fn);
}


static Handle<Value>
fork_fn (const Arguments& args)
{
	HandleScope scope;

	pid_t pid = fork();
	ev_default_fork();

	if (pid < 0)
	{
		return ThrowException(Exception::Error(String::New("Unable to fork daemon, pid < 0.")));
	}

	return Number::New((int) pid);
}


static Handle<Value>
daemonize_fn (const Arguments& args)
{
	pid_t pid, sid, parent;
	int lfp = -1;

	/* already a daemon */
	if (getppid() == 1) return False();

	/* Trap signals that we expect to recieve */
	signal(SIGCHLD, child_handler);
	signal(SIGUSR1, child_handler);
	signal(SIGALRM, child_handler);

	/* Fork off the parent process */
	pid = fork();
	ev_default_fork();

	if (pid < 0)
	{
		return ThrowException(Exception::Error(String::New("Unable to fork daemon, pid < 0.")));
	}
	/* If we got a good PID, then we can exit the parent process. */
	else if (pid > 0)
	{
		/* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
		   for two seconds to elapse (SIGALRM).  pause() should not return. */
		alarm(2);
		pause();

		return ThrowException(Exception::Error(String::New("Unable to fork daemon.")));
	}

	ev_default_fork();

	/* At this point we are executing as the child process */
	parent = getppid();

	/* Cancel certain signals */
	signal(SIGCHLD, SIG_DFL); /* A child process dies */
	signal(SIGTSTP, SIG_IGN); /* Various TTY signals */
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP,  SIG_IGN); /* Ignore hangup signal */
	signal(SIGTERM, SIG_DFL); /* Die on SIGTERM */

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0)
	{
		return ThrowException(Exception::Error(String::New("Unable to create a new session.")));
	}

	/* Redirect standard files to /dev/null */
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);

	/* Tell the parent process that we are A-okay */
	kill(parent, SIGUSR1);

	return True();
}


static void
child_handler (int signum)
{
	switch (signum)
	{
		case SIGUSR1: exit(EXIT_SUCCESS); break;
		case SIGALRM: exit(EXIT_FAILURE); break;
		case SIGCHLD: exit(EXIT_FAILURE); break;
	}
}
