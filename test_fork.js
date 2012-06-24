var util = require('util'),
	fork = require('./fork').fork;

var i = 0, pid = -1;

var timer = setInterval(function () {
	console.log("PID " + pid + ": " + (++i));
	if (i == 3) {
		pid = fork();
		console.log("Forkd: " + pid);
	} else if ((i == 10 && pid == 0) || (i == 15 && pid > 0)) {
		clearInterval(timer);
	}
}, 1000);
