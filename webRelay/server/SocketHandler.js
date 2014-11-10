var io = require('socket.io')
  , pro= require('./ProcessDriver.js')
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

		setInterval(function(){
			socket.emit('heartbeat', G.ioperiod);
		}, G.ioperiod);



		socket.on('update', function(req) {
			var s = JSON.stringify( req )
			switch( req.action || 'false' ) {
				case 'set':
					//return forwardPair(req);
				break;
				case 'Throttle-arm':
					console.log('attempting to arm... ' + req.action );
					child.send( s );
				break;
				case 'Pitch-activate':
				case 'Roll-activate':
				case 'Yaw-activate':

				break;
				case 'Throttle-start':
				case 'Throttle-stop':
				case 'Pitch-reset':
				case 'Pitch-save':
				case 'Roll-reset':
				case 'Roll-save':
				case 'Yaw-reset':
				case 'Yaw-save':

				break;
				case 'false':
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