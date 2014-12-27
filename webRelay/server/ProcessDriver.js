var spawn_cp = require('child_process').spawn
  , assert   = require('assert')
  , exec_cp  = require('child_process').exec;
/* better stack overhead */
function Driver() {
	this.exec 	 = '';
	this.path    = '';
	this.args    = [];
	this.opts    = [];
	this.child   = null;
	this.out     = null;
	this.uptime  = 0;
	this.safety  = false;
};
Driver.prototype.setPath = function(path) {
	this.path = path;
};
Driver.prototype.setExec = function(exec){
	this.exec = exec;
};
Driver.prototype.setOpts = function( opts ){
	this.opts = opts
};
Driver.prototype.setArgs = function( args ){
	this.args = args
};
Driver.prototype.killAll = function() {
	this.safety = false;
	//var trykill = 'pkill ' + this.exec
	//exec_cp(trykill, function (err , stdout, stderr){});
};
Driver.prototype.on      = function( e, cb ) {
	assert.equal( typeof e, 'string' );
	switch(e) {
		case 'out': this.out = cb; break;
	}
};
/* there WILL be a context switch */
Driver.prototype.start   = function() {
	if( this.safety ) return;
	console.log( this.path+this.exec,this.args )
	var out = this.out;
	try {
		//this.killAll();
		delete this.child;
		this.child = null;
		this.child = spawn_cp(this.path+this.exec,this.args,this.opts);
		this.child.stdout.setEncoding('utf8');
		this.child.stdout.on('data', function (chunk) {
			console.log( chunk )
			out( chunk );
		});
		this.child.stderr.on('data', function (chunk) {
			console.log( '>err :: '+chunk )
			out( chunk );
		});
		this.safety = true;
	}
	catch( e ) {
		console.log(e)		
	}
};
Driver.prototype.reset   = function( cb ) {
	this.safety = false;
	/* do things */
	try{
		//this.child.kill('SIGKILL');
		this.start();
	} catch(e) {
		console.log(e)
	}
};
Driver.prototype.kill    = function() {
	this.safety = false;
	try{
		//this.child.kill('SIGKILL');
	} catch(e) {
		console.log(e)
	}
};
Driver.prototype.send    = function( s ) {
	//console.log( s )
	//assert.equal( typeof s, 'string' );
	//console.log( this.child.stdin )
	//this.child.stdin.resume();
	//if( this.child != null   )
	//if( this.child.connected ) {
	//	console.log( s )
	//	this.child.stdin.write( s+'\n', 'utf8' )
	//}
	/*only unsafe way works... this is retarded...*/
	//try {
	//	this.child.setEncoding = 'utf-8' /*this isnt even a proper enc type  utf8 vs utf-8 */
	//	this.child.write( s + "\n" );
	//} catch(e) {
	//	console.log( e )
	//}
};



exports.Driver = Driver
//exports.PT = childTest