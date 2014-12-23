ResourceLock = function(max) {
	this.max       = max || 0;
	this.connected = 0;
	this.PeerStart = function() {
		if( this.Available() ) {
			this.connected++;
		}
		else return false;
	}
	this.PeerEnd  = function() {
		this.connected--;
		if( this.connected < 0 )
			this.connected = 0;
	}
	this.Available= function() {
		return (this.max - this.connected) > 0;
	}
	this.AvailableCount = function() {
		return this.max - this.connected;
	}
	return this;
};

exports.PeerManager = ResourceLock;