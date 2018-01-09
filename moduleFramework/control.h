#ifndef control__h
#define control__h
struct config_PID
{
  float kp        = 0;
    float up_max  = 0;
    float up_min  = 0;
  float ki        = 0;
    float ui_max  = 0;
    float ui_min  = 0;
  float kd        = 0;
    float ud_max  = 0;
    float ud_min  = 0;
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



#endif
