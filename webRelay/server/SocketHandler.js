var io = require('socket.io')
  , G  = require('./global.js').g;

Connect = function(app) {
	var ioh     = io.listen(app, { log: false });
	ioh.handler = SocketHandler 
	return ioh;
};

SocketHandler  = function(socket,peers) {
	if( !peers.Available() ) { 
		socket.disconnect();
		return false
	} else {	
		peers.PeerStart();
		console.log( '> peer accepted' )		
		socket.emit('handshake', G.ioperiod);

/*		var pd = new Driver(PATH,EXEC, function(out) {
			echo(out);
			socket.emit("msg",out);
		}, function() {
			pd.clear();
			pd.start();
		});
*/

		setInterval(function(){
			socket.emit('heartbeat', G.ioperiod);
		}, G.ioperiod);  

		socket.on('start', function(cmd) {
/*			pd.start();*/
		});

		socket.on('cmd', function(cmd) {
/*			echo( cmd )
			pd.send(cmd)*/
		});

		socket.on('update', function(msg) {
/*			echo( msg )
			pd.send(msg.data)*/
		});

		socket.on('disconnect',function() {
			peers.PeerEnd();

/*			echo('disconnected')
			lock   = false;
			pd.kill();*/
		});
	}
}

exports.Handler = SocketHandler;
exports.Connect = Connect;