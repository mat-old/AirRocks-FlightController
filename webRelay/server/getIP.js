var os=require('os');
var ifaces=os.networkInterfaces();

function get( face ) { /* heheh */
	face = face || 'eth1'
	for (var dev in ifaces) {
	  var alias=0;
	  ifaces[dev].forEach(function(details){
	    if (details.family=='IPv4') {
	      if( face == dev ) {
	      	face = details.address;
	      	return;
	      }
	      ++alias;
	    }
	  });
	}	
	return face;	
}



exports.get = get;