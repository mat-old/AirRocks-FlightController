var io = require('socket.io')
  , pro= require('./ProcessDriver.js')
  , dgram = require('dgram')
  , G  = require('./global.js').g;

Connect = function(app) {
	var ioh     = io.listen(app, { log: false });
	ioh.handler = SocketHandler 
	return ioh;
};

SocketHandler  = function(socket,peers,info) {
	if( !peers.Available() ) { 
		socket.disconnect();
		return false
	} else {
		var client = dgram.createSocket('udp4');
		child = new pro.Driver();
		child.setPath(G.uavpath);
		child.setExec(G.uavexec);
		child.setArgs( [info.target] );
		//child.setOpts( ['pipe','pipe','pipe'] );
		child.on('out', function(data){
			socket.emit( 'res', data );
		});
		


		peers.PeerStart();
		console.log( '> peer accepted' )		
		socket.emit('handshake', G.ioperiod);

		var heartbeat = setInterval(function(){
			socket.emit('heartbeat', G.ioperiod);
		}, G.ioperiod);


		/* the switch case is used to filter */
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
					var message = new Buffer( JSON.stringify( req ) )
					client.send(message, 0, message.length, G.destport, G.destIP, function(err, bytes){
						console.log( message.toString('utf8') )
					});
				break;
				default:
				console.log( 'bad request' + req )
			}
		});

		socket.on('control',function(s){
			switch(s) {
				case 'reset':
					child.reset();
				break;
				case 'kill':
					console.log( 'killing child' )
					child.kill();
				break;
				case 'start':
					console.log( 'killing child' )
					child.start();
				break;
				case 'killall':
					child.killAll();
				break;
				case 'settest':
					child.setExec(G.uavAltEx);
					child.setPath(G.uavAltP);
				break;
				case 'setuav':
					child.setExec(G.uavexec);
					child.setPath(G.uavpath);
				break;
			}	
		});

		socket.on('ready', function(){
		});

		socket.on('signal-test', function(dt){
			socket.emit('signal-test-res', '');
		});

		socket.on('disconnect',function() {
			peers.PeerEnd();
		});
	}
}

exports.Handler = SocketHandler;
exports.Connect = Connect;