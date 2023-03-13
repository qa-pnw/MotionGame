#ifndef ultrasonicClass_h
#define ultrasonicClass_h
#include "Arduino.h"


#define CM 1
#define INC 0

class Ultrasonic
{
  public:
    Ultrasonic(int TP, int EP);
  Ultrasonic(int TP, int EP, long TO);
    long Timing();
    long Ranging(int sys);

  private:
    int Trig_pin;
    int Echo_pin;
  long Time_out;
    long duration,distance_cm,distance_inc;
};

#endif

