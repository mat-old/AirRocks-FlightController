var PidCtrls = []
  , socket   = io() 
  , lastBeat = new Date()
  , signal;

function slideUpdate(id,val) {
	var s = id+':'+val
	socket.emit('update', {data:s});
}

function updateSignal() {
	var num = Math.floor(signal*100);
	if( num <= 100 )
		$('.signal').text( num+'%');
	else
		$('.signal').text( "Activity" );
	if( num < 90 ) 
		$('.NSR').removeClass('nsr-ok').addClass('nsr-bad')
	else
		$('.NSR').removeClass('nsr-bad').addClass('nsr-ok')
}

socket.on('heartbeat',function(td){
	var d = new Date();
	signal = (d.getTime() - lastBeat.getTime()) / td;
	lastBeat = d;
	updateSignal() 
});

	window.setInterval(function(){
		var d = new Date();
		if( d - lastBeat > 1000 ) {
			signal = signal - (signal*0.5)
			updateSignal()
		}
	},500);
	window.setTimeout(function(){
		PidCtrls.forEach(function(p){
			var id = '#'+p
			$(id).children(0).attr('data-content', 0.0)
		});
	},1000)


$(document).ready(function(){
	var msgCount = 0;
	function nextChar(c) {
	    return String.fromCharCode(c.charCodeAt(0) + 1);
	}

	function processPID(p) {
		/* p.input is what the IMU reported p.output is the response */
		global_hash[ p.name ].refresh( p.input );
	}

	function processMotorGroup(d) {
		var avg = 0, frac = 254-125, mchar = 'A';
		for (var i = 0; i < d.length; i++) {
			global_hash["Motor_"+mchar].refresh( Math.floor(frac/d[i]*100) );
			mchar = nextChar(mchar);
			avg += parseInt(d[i], 10);
		}
		avg /= 4;
		/* (max - min) / average */
		avg = Math.floor( frac/avg * 100 );
		global_hash['throttle'].refresh( avg )
	}

	function processNormal(obj) {
		if( obj.type == 'status' ) {
			$p = $('<p>').append(obj.data)
			obj.data = $p;
			popup(obj);
		}
	}

	function processError(obj) {
		if( obj.type != 'error' ) return;
		var $list = $('<ol>', {
			class : "msg-list"
		});
		obj.data.list.forEach(function(data){
			$list.append( "<li>"+data+"</li>" );
		});
		$p = $('<p>')
		$p.append("<strong>"+obj.what+"</strong>")
		$p.append($list)
		obj.data = $p
		popup( obj )
	}

	function msgRouter(s) {
		var obj = {};
		try {
			obj = JSON.parse(s);
		}
		catch(e) {
			console.log( "JSON parse failed >> " + s )
			console.log(e)
			obj.type = "uncategorized"
			obj.data = s;			
		}
		switch( obj.type ) {
		case "status"       : return processNormal(obj);
		case "cmd"          : return;
		case "motors"       : return processMotorGroup(obj.data);
		case "PID"          : return processPID(obj);
		case "potential"    : return;
		case "throttle"     : return;
		case "error"        : return processError(obj);
		case "uncategorized": return popup(obj);
		}
	}

	function popup(obj) {
		var $elem = $("<div>", {
			id:'m'+(msgCount++),
			class:'msg-box msg-'+obj.type
		})
		$elem.append( obj.data );
		$('.msg-target').prepend( $elem )
		if( !global_hash[obj.type] || !global_hash['all'] ) {
			$elem.height( $elem.children(0).height() + 3 );
			$elem.hide();
		} else {
			$elem.animate({
				height : $elem.children(0).height() + 3
			},1000);
		}

		if( msgCount > 100 ) {
			$('#m'+(msgCount-100)).remove();
		}
	}

	socket.on('msg',function(obj){
		//console.log( "recieved" + obj )
		msgRouter(obj);
	});
	$('#START').click(function(){
		socket.emit("start","");
	});
	$('#ABORT').click(function(){
		socket.emit('cmd',"DOWN");
	});
	$('#RESET').click(function(){
		socket.emit('cmd',"RESET");
	});
	$('#ARM').click(function(){
		socket.emit('cmd',"ARM");
	});
	$('.msg-ctrl').click(function(){
		var id = this.id;
		global_hash[id] = !global_hash[id];
		if( id == 'all' ) {
			if($(this).hasClass('msg-off')) {
				$('.msg-box').show();
				$('.msg-ctrl').removeClass('msg-off')
			} else {
				$('.msg-box').hide();
				$('.msg-ctrl').addClass('msg-off')
			}
		} else {
			$(this).toggleClass('msg-off')
			$('.msg-'+id).toggle()
		}
	});
	$('.msg-clear').click(function(){
		$('.msg-target').empty()
	});
	//throttle.refresh(100)
});