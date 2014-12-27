var os=require('os');
var ifaces=os.networkInterfaces();

function get( s ) {
	var interfaces = os.networkInterfaces()
	  , addresses = []
	  , ret = 'none';
	for (var k in interfaces) {
	    for (var k2 in interfaces[k]) {
	        var address = interfaces[k][k2];
	        console.log( " > " + k )
	        if (address.family === 'IPv4' && !address.internal) {
	            addresses.push(address.address);
	        }
	    }
	}
	return addresses[0];
}

//console.log( get() )

exports.get = get;