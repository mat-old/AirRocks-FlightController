Lesther Reynoso, Mathew Utter     
AirRocks-FlightController     
Overview.md   

____________________________  


Introduction  
--------------------
> AirRocks-FlightController, herin known as ARF, is a multirotor flight controller suite that uses machine learning to achieve autonomous aerial maneuvers.   

> ARF offers an alternative approach to pre-programed flight patterns. Many drones use GPS coordinates to set way points in a flight plan. ARF seeks to leverage range-based stimuli combined with iterative machine learning to complete flight patterns in an efficient way. Essentially ARF learns to fly better.  

__Problem__   
> The platform and control systems of drones are extremely appealing paradigms. The hardware platform may be configured for various flight styles. The style of flight is meant to accommodate a drone's purpose such as acrobatic, transport, visual surveillance, and signal surveillance. Flight control systems may be developed in such a way that their influence on the ability of the platform can transcend the designed purpose. Flight controllers provide the highest influence on the quality of drone flight becuase all drone's need a pilot and software is the superior pilot.   

> This introduces an argument of effectiveness between human pilots and software. Human effectiveness is measured in the skill of the operator with a remote control. However in software the effectiveness is measurable by how well it can mimic the skill of a human pilot. .  

> For a human to adapt to a drone’s weight he or she must observe it's behavior and build a skill at operating the drone's controls. Likewise, the effect of the drone’s weight can be compensated for by collecting common potentials of past flights, creating a model using that data, comparing that model with a composite model of all past flights to generate another model, applying that model to the rotor response, and correlating the application of that model back into the composite model of past flights. Through this, learning algorithms can be applied to create maneuvers that adapt to the environment and mimic human skill.

__Solution__  
> Machine learning is used to improve speed and stability of a drone. Correlating data collected during flight will be used to create a better composite model. The model is used to alter future maneuvers and even change the flight plan if an obstruction is detected. 
The goal is to develop control software that mimics the abilities of skilled drone pilots. 

__Similar Systems__
> A R&D group, the  [Flying Machine Arena](http://flyingmachinearena.org/) specializes in advanced drone avionics. A paper published by Markus Hehn and Raddaello D'Andrea [Link HERE](http://www.idsc.ethz.ch/people/staff/hehn-m/hehn_dandrea_periodic_learning.pdf) describes very much what our project seeks  to solve. 


Summary 
--------------  
ARF seeks to solve several issues facing classic drone operation by applying machine learning concepts to improve execution of flight patterns. Both hardware and software platforms are being built to demonstrate the effect on flight. 



