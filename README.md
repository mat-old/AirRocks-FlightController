AirRocks-FlightController
=========================

ARFC is a quad-copter flight controller which uses concepts from SCA and iterative learning to achieve awesome drone actions.

___________________
Makefile
------------------- 
`make`  - optomized build, only assembles new sources, no headers  
`make new` - clear old objects and starts build from scratch   
`make build` - optomized build, no linking  
___________________
Progress 
-------------------
__Subsystems__  96%   
_Defined as hardware and the basic software needed to use it. (Hardware & firmware exists )_
  1. PID - 100%
2. ~~Waiting on IMU data Provider~~   
  2. SPI - 100%    
4. Research "failsafe landing procedure"
5. Research "detect SPI failure"
  3. SCA - 80%    
4. Invest in better power source   
5. Must re-write GrovePi interface in C/C++
5. ~~Needs bettery wiring / not enough physical space in SCA module space~~  
  4. DHCP interface - 100%    
5. Waiting on aditional Rpi to test P2P posibilities  
  6. Sensor Providers - 100% 
6. ~~Research ability and speed of retrieving all sensor readings(data faster then SPI)~~  

__Implemented__  57.5%  
_Defined as a design feature that achieves its goal._  
  1. Proportional integral derivative controller __(PID)__ - 80%  
2. needs tuning  
  2. SPI - 100%    
3. Works great, coms are fast and reliable.   
  3. Spatial contextual awareness (SCA) - 0%  
4. ~Must wait for PID Subsystem~  
  5. User interface & (DGRAM) - 50%
6. Waiting for Android developement. 
  7. Sensor Providers - 100%
7. ~~IMU done, USSA waiting on PIDctrl~~
8. USSA - requires a proper heading reference so that unecessary sensors don't fire, (if i'm flying north don't look south)
