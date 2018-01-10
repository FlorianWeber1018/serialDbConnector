#include "control.h"

float PID::getOutput(float x, float w)
{
  float e = w - x;

  if(firstRun){
    firstRun=false;
  }else{

    up = e * config.kp;

    ui += e * config.ki;

    ud = (e - pre_e) * config.kd;
  }


  moveToBorders(up, config.up_min, config.up_max);
  moveToBorders(ui, config.ui_min, config.ui_max);
  moveToBorders(ud, config.ud_min, config.ud_max);

  float y = up + ui + ud;

  pre_e = e;

  return y;
}

int ServoPWM::getOutput(int& PWMinc, int& PWMdec, int input)
{
  if(input > config.input_max){
    input = input_max;
  }else if(input < config.input_min){
    input = config.input_min;
  }

  float inc_m = config.incPWM_max / config.input_max;
  float dec_m = config.decPWM_max / config.input_min;

  float pwminc = inc_m * static_cast<float>(input);
  float pwmdec = dec_m * static_cast<float>(input);

  if(pwminc < incPWM_min){
    PWMinc = 0;
  }else if(pwminc >= config.incPWM_max){
    PWMinc = static_cast<int>(config.incPWM_max);
  }else{
    PWMinc = static_cast<int>(pwminc);
  }
}
