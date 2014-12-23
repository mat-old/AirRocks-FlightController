/* basically constant variables, all lower case */

/*op maker*/
var op = function(name,id,active) {
	this.name = name;
	this.id   = id;
	this.active = active || false;
	return {
		name   : this.name
	  , id     : this.id
	  , active : this.active
	};	
};
/*menu maker*/
var mm = function(name,id,group,opts,active) {
	this.name = name
	this.id   = id
	this.opts = opts
	this.group = group
	this.active = active || false;
	return {
		name : this.name
	  , id   : this.id
	  , opts : this.opts
	  , group: this.group
	  ,active: this.active
	};
};
/* gauges */
var gm = function(id,dclass,container,subtext,min,max) {
	this.id        = id;
	this.class     = dclass;
	this.container = container;
	this.subtext   = subtext;
	this.min       = min;
	this.max       = max;
	return {
		id        : this.id
	  , class     : this.class
	  , container : this.container
	  , subtext   : this.subtext
	  , min       : this.min
	  , max       : this.max
	};
};
/* WxH to css */
var gc = function(width,height) {
	this.width  = width;
	this.height = height;
	this.s = function() {
		return 'width:'+this.width+'px; height:'+this.height+'px;'
	}
	//return { width:this.width, height:this.height };
	return this.s();
};
/* scroll bar */
var sb = function(name, min, max, div) {
	this.name = name;
	this.min  = min;
	this.max  = max;
	this.div  = div
	return {
		name : this.name
	  , min  : this.min
	  , max  : this.max
	  , div  : this.div
	};
};
/* setting */
var sg = function( name, controls, scrollBars ) {
	this.name       = name;
	this.controls   = controls;
	this.scrollBars = scrollBars;
	return {
		name       : this.name
	  , controls   : this.controls
	  , scrollBars : this.scrollBars
	};
};

var  PIDSETTINGS =  [sb('P',0,100,100),sb('I',0,100,100),sb('D',0,100,100)];

var Global = {
    maxpeers : 1
  , ioperiod : 2000 /* 2 sec */
  , favoritePort : process.env.PORT
  , backupPort   : 8888
  , viewpath : './view/'
  , uavpath  : '/home/arfc/arfc/flightController/' 
  , uavexec  : 'ARFCapp.o'
  , uavconf  : 'config.json'
  , uavoptions : {
	  	'test' : '-test',
	  	'tune' : '-tune',
	  	'uav'  : '-uav'
  	}
  , resource : {
		tuner : {
			Tuners : [
				sg('Throttle', ['arm','start','stop'], [sb('Torque',0,100,1)]),
				sg('Pitch', ['activate','reset','save'], PIDSETTINGS),
				sg('Roll', ['activate','reset','save'], PIDSETTINGS),
				sg('Yaw', ['activate','reset','save'], PIDSETTINGS)
			],
			"controls": ['arm','start','stop','save','end'],
			"PIDS"    : ["Roll","Pitch","Yaw"],
			"ITEMS"   : ["P","I","D"],
			GAUGES  : {
				group1 : [
					/* 380,301 */
					gm('throttle',gc(188,150), '', 'Average', 0, 100)
				],
				group2 : [
					gm('Motor_A',gc(93,75),'gauge-small','+Roll',0,100),
					gm('Motor_B',gc(93,75),'gauge-small','+Pitch',0,100),
					gm('Motor_C',gc(93,75),'gauge-small','-Roll',0,100),
					gm('Motor_D',gc(93,75),'gauge-small','-Pitch',0,100)
				],
				group3 : [
					gm('pitch',gc(188,150),'gauge-small','Y\'',-1,1),
					gm('roll',gc(188,150),'gauge-small','X\'',-1,1),
					gm('yaw',gc(188,150),'gauge-small','Z\'',-1,1)
				]
			},
			SIDEBARS : [
				mm('Main','selector', 'main',[
					op('Tuner','tuner-menu',true)
				  , op('Help','help-menu')
				], true), 
				mm('Tuner','tuner-menu', 'tuner',[
					op('All','toggle-all',true)
				  , op('Levels','toggle-levels')
				  , op('Alerts','toggle-alerts')
				  , op('Tuners','toggle-tuners')
				], true),
				mm('Help', 'help-menu', 'help',[
					op('About','toggle-about')
				  , op('Info','toggle-info')
				], false)
			]
		}
	}
  , route    : {
		""  : "landingPad"
	}
  , responseType : {
  		'test_results' : 'xhr'
  }
};

exports.g = Global;