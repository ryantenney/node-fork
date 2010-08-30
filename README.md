node-fork
=========

Simple process forking for Node.js  
Please read me in full before using.

EXPERIMENTAL
------------
Use at your own risk.  
Warning: Pregnant women, the elderly, and children under 10 should avoid prolonged exposure to `node-fork`.  
Caution: `node-fork` may suddenly accelerate to dangerous speeds.  
If `node-fork` begins to smoke, get away immediately. Seek shelter and cover head.  
Do not taunt `node-fork`.  
Accept no substitutes!

**DO NOT** invoke `fork()` while running node-repl. I can't guarantee similar results, but when I did it, my Mac threw up a grey screen of death: http://img.ly/1ZxD

Information
-----------
Consists of functions `fork()`, equivalent to calling the function of the same name from `unistd.h`, and `daemonize()`, which forks, redirects stdin/out/err to `/dev/null`, and kills the parent process.

To fetch and install with `npm`:
    npm install fork

To build lastest from source (currently v0.0.2):
    git clone git://github.com/ryantenney/node-fork.git node-fork && cd node-fork && node-waf configure build

Only tested on OS X 10.6.4.  
Windows users, prerequisite for this to work: `format c:`, then install any other operating system.

License
-------
Copyright 2010 Ryan W Tenney  
http://github.com/ryantenney/node-fork/  
Released under the MIT Licence  

Thanks to http://www.theorie.physik.unizh.ch/~dpotter/howto/daemonize  
for the function daemonize, modified slightly to fit this purpose.  
daemonize() function source released into the Public Domain by Doug Potter  