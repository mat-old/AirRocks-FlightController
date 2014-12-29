__List of acceptable interface messages__  
__NOTE:__ default properties are as follows...     
```javascript  
{ action:'std::string', name:'std::string', value:0.0 }  
```  
AC_handshake _name = clients IP addresss, value = PORT_  
```json  
{ "action":"handshake", "name":"0.0.0.0", "value":"5000" }  
```  
  
AC_mode_select  _Selects which mode the drone will run with_   
 ```json
 {"action":"Mode-select", "value":0} = NO_MODE
 {"action":"Mode-select", "value":1} = TEST_MODE
 {"action":"Mode-select", "value":2} = TUNE_MODE
 {"action":"Mode-select", "value":3} = UAV_MODE
 ```

 
AC_Throttle* _States for controlling the throttle's safety and output_   
```json 
 {"action":"Throttle-arm"}   = AC_throttle_arm   
 {"action":"Throttle-start"} = AC_throttle_start    
 {"action":"Throttle-stop"}  = AC_throttle_stop     
```
Each enables the motos used to tune each respective PID, only one may be active at a time    
```json
 {"action":"Pitch-activate"} = AC_pitch_activate  
 {"action":"Roll-activate"}  = AC_roll_active  
 {"action":"Yaw-activate"}   = AC_yaw_active  
```

RESET _A class of functions that reloads the config.json file last saved settings_     
```json 
 {"action":"Pitch-reset"} = AC_pitch_reset  
 {"action":"Roll-reset"}  = AC_roll_reset     
 {"action":"Yaw-reset"}   = AC_yaw_reset  
```

SAVE _A class of functions that save each setting to the config.json file_    
```json 
 {"action":"Pitch-save"} = AC_pitch_save   
 {"action":"Roll-save"}  = AC_roll_save  
 {"action":"Yaw-save"}   = AC_yaw_save  
```


AC_set  _The AC_set action is class of setter functions for use in 'Tuning mode'_   
```json
 {"action":"set", "name":"Throttle-Torque", "value":0.1} = AC_throttle_torque   
 {"action":"set", "name":"Pitch-P","value":0.1} = AC_pitch_p  
 {"action":"set", "name":"Pitch-I","value":0.1} = AC_pitch_i  
 {"action":"set", "name":"Pitch-D","value":0.1} = AC_pitch_d  
 {"action":"set", "name":"Roll-P","value":0.1} = AC_roll_p  
 {"action":"set", "name":"Roll-I","value":0.1} = AC_roll_i  
 {"action":"set", "name":"Roll-D","value":0.1} = AC_roll_d  
 {"action":"set", "name":"Yaw-P","value":0.1} = AC_yaw_p  
 {"action":"set", "name":"Yaw-I","value":0.1} = AC_yaw_i  
 {"action":"set", "name":"Yaw-D","value":0.1} = AC_yaw_d  
```

 AC_err _any malformed message_  
 _none_ -> _does nothing_   
 
 
__I2C config__   `i2cdetect -y 1`    
 ```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  
    -- 04 -- -- -- -- -- -- -- -- -- -- --   
10: -- -- -- -- -- -- -- -- -- -- -- UU -- 1d -- --    
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
60: -- -- -- -- -- -- -- -- -- -- -- 6b -- -- -- --  
70: -- -- -- -- -- -- -- --  
```
