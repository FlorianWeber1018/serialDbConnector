#ifndef control__h
#define control__h
#include <climits>
struct config_PID
{
  float kp        = 2000;
    float up_max  = INT_MAX;
    float up_min  = INT_MIN;
  float ki        = 0;
    float ui_max  = INT_MAX;
    float ui_min  = INT_MIN;
  float kd        = 0;
    float ud_max  = INT_MAX;
    float ud_min  = INT_MIN;
};

class PID
{
public:
  float getOutput(float input, float w);
  config_PID config;
protected:
  float up=0;
  float ui=0;
  float ud=0;
  bool firstRun = true;

  float pre_e = 0;
private:
};

struct config_ServoPWM
{
  int input_max  = 30000;
  int input_min  = -30000;
  float incPWM_max = 150;
  float incPWM_min = 30;
  float decPWM_max = 150;
  float decPWM_min = 30;
};

class ServoPWM
{
public:
  void getOutput(int& PWMinc, int& PWMdec, int input);
  config_ServoPWM config;
protected:

private:

};

#endif
