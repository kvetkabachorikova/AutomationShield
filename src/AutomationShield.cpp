/* 
 AutomationShield.cpp
  Arduino library for teaching control engineering.
  Authors: Tibor Konkoly, Gabor Penzinger, [...], and Gergely Takacs to be continued
  2017-2018.
  Released into the public domain.
*/
#include "AutomationShield.h"
#include "Arduino.h"

float AutomationShieldClass::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) // same as Arudino map() but with floating point numbers
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // linear mapping, same as Arduino map()
}

float AutomationShieldClass::constrain(float x, float min_x, float max_x){
 if (x>=max_x){x=max_x;} // if x larger than maximum, it's maximum
 if (x<=min_x){x=min_x;} // if x smaller than minimum, it's minimum
 return x; // return x
}

void AutomationShieldClass::error(char *str) // Error handler function
{
  #if ECHO_TO_SERIAL                    // If Serial Echo is turned on in the DEFINE (by advanced user)
  Serial.print("Error: ");
  Serial.println(str);
  #endif
  digitalWrite(ERRORPIN, HIGH);           // Enable generic error pin, should be 13 w/ the onboard LED
  while (1);                              // Stop all activity in busy cycle, so user definitely knows what's wrong.
}

float AutomationShieldClass::pid(float err, float input,float Kp,float Ki,float Kd,float outMin, float outMax ,int direct)
 {     

    if (direct == 1) // reverse acting system (FloatShield) 
    { 
        Kp = (0 - Kp); 
        Ki = (0 - Ki); 
        Kd = (0 - Kd); 
    } 
  
   integral = integral + (err)*Ts;
  if (integral > outMax)
  {
    integral = outMax;
  }
  else if( integral < outMin)
  {
    integral = outMin;
  }

   derivative = (input - lastinput)/Ts; //removes Derivative kick
   output = Kp *err + Ki*integral - Kd*derivative;

  if (output > outMax)
  {
    output = outMax;
  }
  else if( output < outMin)
  {
    output = outMin;
  }

 lastinput = input;
  return output;
 }
    
float AutomationShieldClass::pid1(float err,float Kp,float Ti,float Td,float outMin, float outMax)
 {
    
  error[0] = err;
  r_p = Kp;
  r_i = Kp/Ti;
  r_d = Kp*Td;

  q0 = r_p+r_i*Ts+r_d/Ts;
  q1 = -r_p - (2*r_d)/Ts;
  q2 = r_d/Ts;

  out[1] = out[0]+ q0*error[0] + q1*error[1] +q2*error[2];

  out[0] = out[1];
  if (out[1] > outMax)
  {
    out[1] = outMax;
  }
  else if( out[1] < outMin)
  {
    out[1] = outMin;
  }

  
  error[2] = error[1];
  error[1]= error[0];

  return out[1];
 }

AutomationShieldClass AutomationShield; // Construct instance (define)


void OptoClass::begin(void){                  // begin function initializes the pins used by the hardware. Optoshield uses three pins, pin A1 is used by the LDR, 
                                            //pin A0 is connected to the runner of the potentiometer and digital pin 3 is for setting the intensity of the leds' light                                            
  pinMode(OPTO_YPIN, INPUT);
  pinMode(OPTO_UPIN, OUTPUT);
  pinMode(OPTO_RPIN, INPUT); 
}


void OptoClass::actuatorWrite(int value){
 // Do it in percents @TiborKonkoly :)
  if(value <= 255){                                                 // nested if statement, if the condition is true check the following
      if(value > 0){                                                  // if the second condition is also true, write the value of the sensor
        analogWrite(OPTO_UPIN,value);
      }
    }
    else {AutomationShield.error("Input must be between 0-100.");} // if any of the statements is true, you receive a report 
}

int OptoClass::sensorRead(){
  int _valueRead = analogRead(OPTO_YPIN);
  return _valueRead;
}

int OptoClass::referenceRead(){
  int _valueRead = analogRead(OPTO_RPIN);
  return _valueRead;
}
            
OptoClass OptoShield; // Construct instance (define)                  

