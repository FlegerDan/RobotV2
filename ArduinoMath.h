#ifndef _ARDUINO_MATH_H_
#define _ARDUINO_MATH_H_

//those methods do exactly the same things as the corespondent Arduino methods do
//see the Arduino documentation for more info.
class ArduinoMath{
  public:
    static int constrain(int, int, int);
    static int min(int, int);
    static int max(int, int);
    static int map(int, int, int, int, int);
    static int abs(int);
  private:
    ArduinoMath(){};
};

#endif
