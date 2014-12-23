var PidCtrls = []
  , socket   = io() 
  , lastBeat = new Date()
  , signal = 0
  , sigInterval = null;

function slideUpdate(id,val) {
	var s = id+':'+val
	socket.emit('update', {data:s});
}

function updateSignal() {
	var num = Math.floor(signal*100)
	  , count = Math.floor(num/20)
	  , color = count<3?'RED':count==5?'#00ff40':'WHITE'
	  , incr = 32/5;

	count = count > 5 ? 5 : count;

	$('.signal').empty();
	for (var i = 1; i < count+1; i++) {
		var $elem = $('<div>', {
			class      : 'bar'
		  , style      : 'background:'+color+';left:'+(i*7)+'px; height:'+ (i*20) +'%;'
		});
		$('.signal').append( $elem )
	}  

	if(signal <= 0.5) {
		clearInterval(sigInterval);
		$('.signal').empty();
		//alert('SIGNAL LOST')
	}
}

socket.on('heartbeat',function(td){
	var d = new Date();
	signal = (d.getTime() - lastBeat.getTime()) / td;
	lastBeat = d;
	updateSignal() 
});

socket.on('handshake', function() {
	sigInterval	= window.setInterval(function(){
		var d = new Date();
		if( d - lastBeat > 2000 ) {
			signal = signal - (signal*0.5)
			updateSignal()
		}
	}, 2000);
	window.setTimeout(function(){

		$('.main').removeClass('suppressed');
	}, 1000);

})

	window.setTimeout(function(){
		PidCtrls.forEach(function(p){
			$('#'+p).children(0).attr('data-content', 0.0)
			$('.data-'+p).text('0.0')
		});
	}, 200)


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

	function setActiveGroupItem( sender ) {
		$(sender).addClass('active').siblings().removeClass('active')
	}

	socket.on('msg',function(obj){
		//console.log( "recieved" + obj )
		msgRouter(obj);
	});

	$('.menu-box li').click(function(){
		var id = this.id
		setActiveGroupItem( this )

		$('.menu-box#'+id).siblings().not('#selector').fadeOut(200,function(){
			window.setTimeout(function(){
				$('.menu-box#'+id).fadeIn()
			},250)
		})
		if( id.indexOf('toggle') >= 0 ) {
			var group = $(this).attr('group')
			console.log( group )
		}
	})

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