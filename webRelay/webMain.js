/*
*	this is a nodejs server for testing and tuning ARFC PID values
*	:requires
*		SocketIO
*
*	command scheme: "name:value"
*
*		roll       RP:#  RI:#  RD:#	
*		pitch      PP:#  PI:#  PD:#
*		yaw        YP:#  YI:#  YD:#
*		throttle   T:#
*/
var app 
  , io
  , sys   = require('sys')
  , utils = require('./utils.js')
  , Driver= require('./ProcessDriver.js').PD
  , echo  = utils.echo
  , send  = utils.send
  , lock  = false
  , cycle = 1000;

function run() {
	server = utils.CreateServer( requestCB );
	io     = utils.CreateIO( server );

	echo( server.address() );
	function requestCB( req, res ) {
		return send( req, res )
	}

	io.sockets.on('connection', function (socket) {
		if( lock ) return false
		echo( 'connected' )
		lock = true;
		
		var pd = new Driver("/home/arfc/arfc/flightController/ARFC", function(out) {
			echo(out)
		});

		setInterval(function(){
			socket.emit('heartbeat',cycle)
		}, cycle); // 5s 

		socket.on('start', function() {
			if( pd.running() ) {
				return;
			}
			echo('start request')
			if(!pd.start()) {
				echo('failed to start')
			}
		});

		socket.on('shutdown', function() {
			pd.kill()
		});

		socket.on('update', function(msg) {
			pd.send(msg.data)
		});

		socket.on('disconnect',function() {
			echo('disconnected')
			lock   = false;
			pd.kill();
		});

	});

};

run();
















