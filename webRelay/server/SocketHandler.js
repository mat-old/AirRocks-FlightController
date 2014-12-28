var io = require('socket.io')
  , pro= require('./ProcessDriver.js')
  , dgram = require('dgram')
  , IP = require('./getIP.js')
  , G  = require('./global.js').g;

var client = dgram.createSocket('udp4');
var relay = dgram.createSocket('udp4');
var globalSocket = false;


Connect = function(app) {
	var ioh     = io.listen(app, { log: false });
	ioh.handler = SocketHandler 
	return ioh;
};

try {
		relay.on("listening", function () {
			var addr = relay.address();
			console.log("> listening "+addr.address+":"+addr.port);
		});

/*		relay.on("message", function (msg, rinfo) {
			if( globalSocket ) {
				console.log( rinfo.address, ">>" )
				socket.emit('res', msg.toString())
			}
		});*/

	relay.bind( 5001 );
} catch( e ) {
	//socket.emit('res',{error:"cannot connect"})
}


function SocketHandler(socket,peers,info) {
	this.heartbeat = setInterval(function(){
		socket.emit('heartbeat', G.ioperiod);
	}, G.ioperiod);

	if( !peers.Available() ) { 
		socket.disconnect();
		return false
	} else {
		peers.PeerStart();
		console.log( '> peer accepted' )		

		socket.emit('handshake', G.ioperiod);


		socket.on('starthandshake', function() {
			var message = new Buffer( JSON.stringify({
					action: 'handshake'
				  , name  : IP.get()
				  , value : G.recvport
				})
			);
			client.send(message, 0, message.length, G.destport, G.destIP, function(err, bytes){
				console.log( message.toString('utf8') )
			});
		})

		relay.removeListener("message", function(){
		})
		relay.addListener("message", function (msg, rinfo) {
			console.log( rinfo.address, ">>" )
			socket.emit('res', msg.toString())
		});			

		/* the switch is used to filter */
		socket.on('update', function(req) {
			switch( req.action || '' ) {
				case 'set':
				case 'Throttle-arm':
				case 'Pitch-activate':
				case 'Roll-activate':
				case 'Yaw-activate':
				case 'Throttle-start':
				case 'Throttle-stop':
				case 'Pitch-reset':
				case 'Pitch-save':
				case 'Roll-reset':
				case 'Roll-save':
				case 'Yaw-reset':
				case 'Yaw-save':
				case 'Mode-select':
				case 'reset':
				case 'reset-hard':
					var message = new Buffer( JSON.stringify( req ) )
					client.send(message, 0, message.length, G.destport, G.destIP, function(err, bytes){
						console.log( message.toString('utf8') )
					});
				break;
				default:
				console.log( 'bad request' + req )
			}
		});

		socket.on('ready', function(){
		});

		socket.on('signal-test', function(dt){
			socket.emit('signal-test-res', '');
		});

		socket.on('disconnect',function() {
			try{
			}
			catch(e) { /*no issues*/ }
			peers.PeerEnd();
		});
	}
}




exports.Handler = SocketHandler;
exports.Connect = Connect;