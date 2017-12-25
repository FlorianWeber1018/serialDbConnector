#ifndef __module_tempDiff_h
#define __module_tempDiff_h
#include "mysqlcon.h"
#include <string>
namespace Module{
  struct Input
  {
    std::string DeviceID;
    std::string PortType;
    std::string Port;
    std::string Pin;
  }
  namespace TempDiff{
    struct config
    {
      unsigned int ID;

    }
    class TempDiff
    {
    public:
      moduleTempDiff();
      ~moduleTempDiff();
      void tick();
    protected:
      config m_config;
      Input m_I0;
      Input m_I1;
      bool m_sigOut;
    };
  }
}







#endif
