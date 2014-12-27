__The follow is a list of acceptable interface messages__  

__AC_mode_select__  _Selects which mode the drone will run with_   
 `{"action":"Mode-select", "value":0}` -> NO_MODE  
 `{"action":"Mode-select", "value":1}` -> TEST_MODE  
 `{"action":"Mode-select", "value":2}` -> TUNE_MODE  
 `{"action":"Mode-select", "value":3}` -> UAV_MODE  
 
__AC_thrittke_*__ _States for controlling the throttle's safety and output_   
 `{"action":"Throttle-arm"}`    -> __AC_throttle_arm__   
 `{"action":"Throttle-start"}`  -> __AC_throttle_start__    
 `{"action":"Throttle-stop"}`   -> __AC_throttle_stop__     

__Each enables the motos used to tune each respective PID, only one may be active at a time__    
 `{"action":"Pitch-activate"}` -> __AC_pitch_activate__  
 `{"action":"Roll-activate"}`  -> __AC_roll_active__  
 `{"action":"Yaw-activate"}`   -> __AC_yaw_active__  

__RESET__ _A class of functions that reloads the config.json file last saved settings_     
 `{"action":"Pitch-reset"}` -> __AC_pitch_reset__  
 `{"action":"Roll-reset"}`  -> __AC_roll_reset__     
 `{"action":"Yaw-reset"}`   -> __AC_yaw_reset__  

__SAVE__ _A class of functions that save each setting to the config.json file_    
 `{"action":"Pitch-save"}` -> __AC_pitch_save__   
 `{"action":"Roll-save"}`  -> __AC_roll_save__  
 `{"action":"Yaw-save"}`   -> __AC_yaw_save__  


__AC_set__  _ The AC_set action is class of setter functions for use in 'Tuning mode'_   
 `{ "action":"set", "name":"Throttle-Torque", "value":0.1}`  -> __AC_throttle_torque__   
 `{"action":"set", "name":"Pitch-P","value":0.1}` -> __AC_pitch_p__  
 `{"action":"set", "name":"Pitch-I","value":0.1}` -> __AC_pitch_i__  
 `{"action":"set", "name":"Pitch-D","value":0.1}` -> __AC_pitch_d__  
 `{"action":"set", "name":"Roll-P","value":0.1}`  -> __AC_roll_p__  
 `{"action":"set", "name":"Roll-I","value":0.1}`  -> __AC_roll_i__  
 `{"action":"set", "name":"Roll-D","value":0.1}`  -> __AC_roll_d__  
 `{"action":"set", "name":"Yaw-P","value":0.1}`   -> __AC_yaw_p__  
 `{"action":"set", "name":"Yaw-I","value":0.1}`   -> __AC_yaw_i__  
 `{"action":"set", "name":"Yaw-D","value":0.1}`   -> __AC_yaw_d__  

 __AC_err__ _any malformed message_
 _none_ -> _does nothing_ 
