#ifndef clock__h
#define clock__h

class Clock
{
public:
  Clock(std::crono::milliseconds T);
  void run(int ticks);
  void runOnce();
  void stop();

protected:

private:

}

#endif
