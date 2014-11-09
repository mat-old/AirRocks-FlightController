var fs      = require('fs')
  , url     = require('url')
  , mime    = require('mime-types')
  , template= require('jade')
  , XHR     = { test_results: null }
  , G       = require('./global.js').g;

	function getType( path ) {
		var res = G.responseType[ path ];  
		if( res == undefined ) {
			if( path.indexOf('.') == -1 )
				res = 'jade'
			else if( path.indexOf('.js') > -1 )
				res = 'js'
			else if( path.indexOf('.css') > -1 )
				res = 'css'
			else
				res = 'err'
		}
		return res;
	}

	function getPath( req ) {
		var path;
		if( typeof req === 'string' )
			path = url.parse(req).pathname.replace(/\//,'')
		else
			path = url.parse(req.url).pathname.replace(/\//,'')
		return G.route[path] || path;				
	}

	function finalizeResponse( res, code, content, type ) {
		res.writeHead(code, {
			'Content-Length': content.length,
			'Content-Type': type
		});
		res.end(content);
	}

	function templateResponse( res, info ) {
		var s = G.viewpath + info.type + '/' + info.target + '.jade'
		  , env = G.resource[ info.target ]
		template.renderFile(s, env, function(err,html){
			if( err ) {
				console.log( '[bad template res]', s , err )
				return errResponse( res, info );
			} else {
				return finalizeResponse( res, 200, html, 'text/html' )
			}
		})
	}
	function xhrResponse( res, info ) {
		try {
			var ret = XHR[info.target]()
			return finalizeResponse( res, 200, ret, mime.lookup('.txt') );
		}
		catch( err ) {
			console.log( '[bad XHR res]', info , err )
			return errResponse( res, { target:'NULL', path:'NULL' } )
		}
	}
	function stdResponse( res, info ) {
		var s = G.viewpath + info.type + '/' + info.target;
		fs.readFile( s, function(err, file) {
			if( err ) {
				console.log( '[bad standard res]', s , err )
				return errResponse( res, info );
			}
			else {
				return finalizeResponse( res, 200, file, mime.lookup(s) )
			}
		});
	}
	function errResponse( res, info ) {
		return finalizeResponse(
			res
		  , 404
		  , 'Sorry, '+info.target+' doesn\'t exist.'
		  , 'text/html'
		);
	}

	XHR.test_results = function() {
		return '{ "data":"true" }';
	}

	/* exported functions*/
	route = function(req, res, info) {
		switch( info.type || 'err' ) {
			case 'xhr':
				return xhrResponse( res, info )
			case 'template':
			case 'jade':
				return templateResponse( res, info );
			case 'js':
			case 'css':
				return stdResponse( res, info );
			default:
				return errResponse( res, info );
		}
	};
	getRouteInfo = function( req ) {	
		this.target = getPath( req )
		this.type   = getType(this.target)
		return {target:this.target,type:this.type};
	};
exports.route = route;
exports.info  = getRouteInfo;