AirRocks-FlightController
=========================

ARFC is a quad-copter flight controller which uses concepts from SCA and iterative learning to achieve awesome drone actions.

___________________
Progress 
-------------------
__Subsystems__  
_Defined as hardware and the basic software needed to use it. (Hardware & firmware exists )_
  1. PID - 100% _Ready to start_    
2. ~~Waiting on IMU data Provider~~   
  2. SPI - 90%    _Ready to start_ 
4. Research "failsafe landing procedure"
5. Research "detect SPI failure"
  3. SCA - 90%    _Ready to start_
4. Invest in better power source  
5. ~~Needs bettery wiring / not enough physical space in SCA module space~~  
  4. DHCP interface - 0%    
5. Waiting on aditional Rpi to test P2P posibilities  
  6. Sensor Providers - 100% _Ready to start_
6. ~~Research ability and speed of retrieving all sensor readings(data faster then SPI)~~  

__Protocol__  
_Defined as a design feature that achieves its goal._  
  1. Proportional integral derivative controller __(PID)__ - 1%  
2. Requires research  
  2. SPI - 100%    
3. Works great, coms are fast and reliable.   
  3. Spatial contextual awareness (SCA) - 0%  
4. Must wait for PID Subsystem  
  5. User interface & (DHCP) - 0%
6. Waiting for Android developement to start. 
  7. Sensor Providers - 50%
7. IMU done, USSA waiting on PIDctrl
8. USSA - requires a proper heading reference so that unecessary sensors don't fire, (if i'm flying north don't look south)
