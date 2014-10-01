Lesther Reynoso, Mathew Utter     
AirRocks-FlightController     
Overview.md   

____________________________  


Introduction  
--------------------
> AirRocks-FlightController, herin known as ARF, is a multirotor flight controller suite that uses machine learning to autonomous aerial maneuvers.   

> ARF offers an alternative means to pre-programed flight patterns. Many drones use GPS coordinates to set way points in a flight plan. ARF seeks to implement range-based stimuli combined with iterative machine learning to achieve flight patterns in an increasingly efficient way. Essentially ARF learns to fly better with each flight.  

__Problem__   
> The platform and control systems for drones are extremely appealing paradigms. The hardware platform may be configured for various styles of flight serving purposes such as acrobatic, cargo transport, visual surveillance, and signal surveillance. Flight control systems may be developed in such a way that there influence on the ability of the platform can transcend the designed purpose. Flight controllers provide the highest influence on the quality of the drone flight as hardware is not capable of flying itself without a driver.   

> This introduces an argument of effectiveness between human pilots and software. The human effectiveness is solely reliant on the skill applied through a remote control. However in software the effectiveness is measurable by how heuristic and also calculated the flight can be. Learning algorithms allow for maneuvers to be altered accommodating the imperfect physical environment around the drone. The drone’s weight can be felt by a human and the effect of the drone’s weight can be calculated by on bored equipment.       

__Solution__  
> Machine learning is used to improve flight patterns speed and stability. Correlating data collected during flight plans will be used to create a better environmental model. The model is used to alter maneuvering and even change the flight plan is an obstruction is detected. 
The goal is to develop control software that mimics the abilities of skilled drone pilots. 

__Similar Systems__
> A R&D group, the  [Flying Machine Arena](http://flyingmachinearena.org/) specializes in advanced drone avionics. A paper published by Markus Hehn and Raddaello D'Andrea [Link HERE](http://www.idsc.ethz.ch/people/staff/hehn-m/hehn_dandrea_periodic_learning.pdf) describes very much what our project seeks  to solve. 


Summary 
--------------  
ARF seeks to solve several issues facing classic drone operation by applying machine learning concepts to improve execution of flight patterns. Both hardware and software platforms are being built to demonstrate the effect on flight. 



