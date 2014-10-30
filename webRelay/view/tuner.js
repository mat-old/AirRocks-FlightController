var PidCtrls = []
  , socket   = io.connect() 
  , lastBeat = new Date()
  , signal;

function state(s){
	switch(s) {
		case 'start':
			$('html').css('border','1px solid #36c7e1')
		return
		case 'stop':
			$('html').css('border','1px solid #fff')
		return
	}
}

function slideUpdate(id,val) {
	var s = id+':'+val
	socket.emit('update', {data:s});
}

socket.on('heartbeat',function(td){
	var d = new Date();
	signal = (d.getTime() - lastBeat.getTime()) / td;
	lastBeat = d;
	var num = Math.floor(signal*100);
	$('.signal').text( num+'%')
	if( num < 90 ) 
		$('.NSR').css('background',"#b0251a")
	else
		$('.NSR').css('background',"#1ab04a")

});

	window.setTimeout(function(){
		PidCtrls.forEach(function(p){
			var id = '#'+p
			$(id).children(0).attr('data-content', 0.0)
		});
	},1000)


$(document).ready(function(){

	$('#start').click(function(){
		var r = window.confirm("Arm ARFC and start PID tuning? (Old settings will be lost)");
		if(r)
			socket.emit('start',{});
		state('start')
	});
	$('#shutdown').click(function(){
		socket.emit('shutdown',{});
		alert("Kill signal was sent.")
		state('stop')
	});

})