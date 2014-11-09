var router  = require('./RequestHandler.js')
  , G       = require('./global.js');

function httpserver(iom, peers, port) {

	function requestCB( req, res ) {
		var info = router.info( req );
		router.route( req, res, info );
	}

	var server = CreateServer( requestCB, port )
	  , io     = iom.Connect( server );
	
	io.sockets.on('connection', function(socket){
		if( peers.Available() ) {
			iom.Handler( socket, peers, router.info(socket.handshake.headers.referer ) )
		}
		else
			socket.disconnect();
	});
};

CreateServer = function(cb, port) {
	return require('http').createServer(cb).listen( port )
};

exports.server = httpserver;