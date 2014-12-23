var echo  = require('./utils.js').echo
  , spawn = require('child_process').spawn
  , exec  = require('child_process').exec;

ProcessDriver = function(epath, eexec, stdoutCB, faultCB) {
	echo( 'PD', {'interface':'ready'} )
	this.executable = eexec;
	this.path       = epath
	this.process    = { connected : false };
	this.stdoutCB   = stdoutCB
	this.stdoutCB( '{"type":"status","data":"Interface Ready"}' )
	this.onFaultCB = faultCB
	this.stdBuffer 	= []

	this.sysKill = function() {
		var trykill = 'pkill ' + this.executable
		echo( 'doubletap', trykill )
		exec(trykill, function (err , stdout, stderr){});
	}

	this.start   = function() {
		var cb = this.stdoutCB
		var of = this.onFaultCB
		/*make sure its not already running*/
		try {
			this.sysKill();
			this.process = spawn(this.path+this.executable,[/*args*/],[/*opts*/])
			this.process.stdout.setEncoding('utf-8');
		
			this.process.stdout.on('data', function (data) {
					var res = data.toString().split('\n');
					for(var s in res)
						if( res[s].length > 3 )
						cb(res[s]);
			});
			this.process.stderr.on('data', function (data) {
				var res = data.toString();
				cb(res);
			})
			this.process.on('exit', function(code){
				//of()
			})
			this.process.on('error', function(code){
				//of()
			})
		}
		catch( e ) {
			echo( 'PD FAIL 1', e )
			
		}
		return true;
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
			if( e.code == 'EPIPE' ) {
				echo( '{"type":"error","data":{["ARFC IS CRITICAL FAILURE"]}}' )
				this.sysKill();
				this.start();
			}
		}
	}
	this.clear   = function() {
		delete this.process
	}
	this.stopped = function() {
		return !this.process.connected;
	};
	this.running = function() {
		return this.process.connected;
	};

	this.instance= function(){

	}
};


ProcessTest = function(PATH,EXEC,ARGS,cb) {
		var p = PATH+EXEC+" "+ARGS
		exec(p, function (err , stdout, stderr){
			if( err || stdout.length == 0 )
				cb(false);
			cb(stdout)
		});
} 

exports.PD = ProcessDriver
exports.PT = ProcessTest