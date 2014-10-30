var echo  = require('./utils.js').echo
  , spawn = require('child_process').spawn;

ProcessDriver = function(exec, stdoutCB) {
	echo( 'PD', {'interface':'ready'} )
	this.executable = exec;
	this.process    = { connected : false };
	this.stdoutCB   = stdoutCB
	this.stdoutCB( 'stdCB' )
	this.stdBuffer 	= []
	this.start   = function() {
		var cb = this.stdoutCB
		this.process = spawn(this.executable,[/*args*/],[/*opts*/])
		this.process.stdout.setEncoding('utf-8');
	
		this.process.stdout.on('data', function (data) {
				var res = data.toString()
				cb(res)
		});
		return this.running()
	}
	this.kill    = function() {
		try {
			echo('kill request ' + this.process.pid)
			this.process.kill('SIGKILL')
		} catch(e) {
			echo( e )
		}
	}
	this.send    = function(s) {
		try {
			this.process.stdin.setEncoding = 'utf-8'
			this.process.stdin.write( s + '\n')
		} catch(e) {
			echo(e)
		}
	}
	this.stopped = function() {
		return !this.process.connected;
	};
	this.running = function() {
		return this.process.connected;
	};
};

exports.PD = ProcessDriver