// utils.js
var http = require('http')
  , io   = require('socket.io')
  , url  = require('url')
  , fs   = require('fs')
  , mime = require('mime-types')
  , jade = require('jade')
  , dir  = process.argv[2]||"./";

var route = {
	"/home" :"/index.jade",
	"/tuner":"/tuner.jade",
	"/"     :"/landingPad.jade",
	""      :"/landingPad.jade"
};

var resource = {
	"/tuner.jade" : {
		"buttons" : ["START","ABORT","RESET","ARM"],
		"PIDS"    : ["Roll","Pitch","Yaw"],
		"ITEMS"   : ["P","I","D"],
		"GAUGES"  : [
			{"id":"throttle","class":'width:380px ;height:301px;',"container":"gauge-med","subtext":"Average","min":0,"max":100},
			{"id":"Motor_A","class":'width:93px ;height:75px;',"container":"gauge-small","subtext":"+Roll","min":0,"max":100},
			{"id":"Motor_B","class":'width:93px ;height:75px;',"container":"gauge-small","subtext":"+Pitch","min":0,"max":100},
			{"id":"Motor_C","class":'width:93px ;height:75px;',"container":"gauge-small","subtext":"-Roll","min":0,"max":100},
			{"id":"Motor_D","class":'width:93px ;height:75px;',"container":"gauge-small","subtext":"-Pitch","min":0,"max":100}		
		],
		"GROUP2"  : [
			{"id":"pitch","class":'width:170px ;height:130px;',"container":"gauge-small","subtext":"y'","min":-1,"max":1},
			{"id":"roll","class":'width:170px ;height:130px;',"container":"gauge-small","subtext":"x'","min":-1,"max":1},
			{"id":"yaw","class":'width:170px ;height:130px;',"container":"gauge-small","subtext":"z'","min":-1,"max":1}
		]
	}	
};


CreateServer = function(cb) {
	echo(  'starting server' )
	return http.createServer(cb).listen(process.env.PORT || 8888)
};

CreateIO     = function(app) {
	return io.listen(app, { log: false })
};

echo         = function() {
	var args = Array.prototype.slice.call(arguments);
	console.log( args )
};

path         = function(req) {
	var s = url.parse(req.url).pathname 
	return route[s] || s
}

send         = function( req, res ) {
	var p = path(req)
	if( p.indexOf('jade') == -1 )
		fsRender(res, p)
	else
		jadeRender(res,p)
}

jadeRender   = function(res,p) {
	var s = dir+'view'+p
	jade.renderFile(s, resource[p] , function(err,html){
		if( err ) {
			echo( 'cannot find', s , err )
			return notFound(res);
		}
		res.writeHead(200, {
			'Content-Length': html.length,
			'Content-Type': 'text/html'
		});
		res.end(html);
	})

}

fsRender     = function(res,p) {	
	var s = dir+'view'+p
	fs.readFile( s, function(err, file) {
		if( err ) {
			echo( 'cannot find', s )
			return notFound(res);
		}
		res.writeHead(200, {
			"content-type":mime.lookup(s)
		})
		res.end(file)
	});
}


notFound     = function(res) {
	res.writeHead(404, {
		"content-type":"text/html"
	})
	res.end('Not found.')
}

exports.CreateServer = CreateServer
exports.CreateIO     = CreateIO
exports.notFound     = notFound
exports.echo         = echo
exports.send         = send