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
