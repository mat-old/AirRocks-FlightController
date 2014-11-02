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
	function msg(s) {
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
		case "status" :
			popup(obj);
			break;
		case "cmd" :
		case "motors" :
			console.log( obj )
			break;
		case "PID" :
			console.log( obj )
			break;
		case "potential" :
			console.log( obj )
			break;
		case "throttle" :
			console.log( obj )
			break;
		case "error" :
			popup(obj);
		case "uncategorized":
			popup(obj);
		}
	}

	function popup(obj) {
		var $elem = $("<div>", {
			id:'m'+(msgCount++),
			class:'msg-box msg-'+obj.type,
			html: "<p>"+ obj.data +"</p>"
		})
		$('.msg-target').prepend( $elem )
		$elem.animate({
			height : $elem.children(0).height()
		},1000);
	}

	socket.on('msg',function(obj){
		console.log( "recieved" + obj )
		msg(obj);
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

	//throttle.refresh(100)
});