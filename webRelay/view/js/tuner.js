var PidCtrls = []
  , socket   = io() 
  , lastBeat = new Date()
  , testTime = null
  , signal = 0
  , sigInterval = null
  , msgCount = 0
  , nextChar = function(c) {
	    return String.fromCharCode(c.charCodeAt(0) + 1);
	}
  , processPID = function(p) {
		/* p.input is what the IMU reported p.output is the response */
		global_hash[ p.name ].refresh( p.input );
	}
  , processMotorGroup = function(d) {
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
  , processNormal = function(obj) {
  		obj.data = obj.data || 'Unknown message.'
		displayMsg(obj.data,'error');
	}
  , msgRouter = function(s) {
  		try {
			var obj = {};
			try {
				if( typeof s === 'string' )
					obj = JSON.parse(s);
				else
					obj = s;			
			}
			catch(e) {
				obj = s;			
			}
			switch( obj.type ) {
			case "status"       : return displayMsg(obj.data,'status');
			case "cmd"          : return;
			case "motors"       : return processMotorGroup(obj.data);
			case "PID"          : return processPID(obj);
			case "potential"    : return;
			case "throttle"     : return;
			case "error"        : return processNormal(obj,'error');
			case "uncategorized": return displayMsg(obj);
			}
		} catch(e) {
			console.log('malformed message')
		}
	}
  , displayMsg = function(text,type) {
  		var t = 'msg'+(type || 'status');
		$('.msg-target').append( $("<li>", { id:'m'+(msgCount++),text:text,class:t }) );
		if( msgCount > 100 ) {
			$('#m'+(msgCount-100)).remove();
		}
	}
  , setActiveGroupItem = function( sender ) {
		$(sender).addClass('active').siblings().removeClass('active')
	}
  , setActiveItem = function( sender, set ) {
  		if( set == true )
			$(sender).addClass('active')
  		else
  			$(sender).removeClass('active')
	}
  , slideUpdate = function(id,val) {
		socket.emit('update', {  name:id, value:val, action:'set'});
	}
  , updateSignal = function() {
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
			var $bang = $('<div>',{class:'bang',text:'!'});
			$('.signal').append( $bang );
			msgRouter( {type:'error',data:'Error. Connection lost.'} );
		}
	};

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
		msgRouter( {type:'status', data:'Connected.'} )
		socket.emit('ready', '');
	}, 1000);

});

window.setTimeout(function(){
	PidCtrls.forEach(function(p){
		$('#'+p).children(0).attr('data-content', 0.0)
		$('.data-'+p).text('0')
	});
}, 200)


$(document).ready(function(){
	socket.on('res',function(obj){
		msgRouter(obj);
	});
	$('.menu-box li,.navbar-nav li').click(function(){
		var id = this.id
		setActiveGroupItem( this )
		$('.menu-box#'+id+',section#'+id).siblings().not('.msg-container').fadeOut(100,function(){
			window.setTimeout(function(){
				$('.menu-box#'+id+',section#'+id).fadeIn()
			},140)
		});
		if( id.indexOf('toggle') >= 0 ) {
			var group = $(this).attr('group')
			//console.log( 'Group = '+group )
			//$('section#'+id).toggle();
		}
		//console.log( 'Item = '+id )
	});

	$('.btn-tuner').click(function(){
		socket.emit("update", { action:this.id } );
	});

	$('.signal').click(function(){
		var dt = new Date();
		testTime = dt.getTime();
		socket.emit('signal-test', testTime);
	});
	socket.on('signal-test-res', function(dt){
		var dt   = new Date()
		  , diff = (dt.getTime() - testTime)/1000; 
		msgRouter({type:'status',data: 'UAV response time '+diff+'s.' });
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
			$('.msg'+id).toggle()
		}
	});
	$('.msg-clear').click(function(){
		$('.msg-target').empty()
	});
	$('.control').click(function(){
		socket.emit('control', this.id)
	});
	$('section#help-menu, section#controls-menu').hide();
	$('.carousel-indicators li').click(function(){
		setActiveGroupItem( this )
	});
});