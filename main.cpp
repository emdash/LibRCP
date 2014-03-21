#include <stdio.h>
#include "rcpparse.h"


#define RPM_CHANNEL FREQ_0
#define CONFIG						\
  (ANALOG_0_ENABLED					\
   |ANALOG_1_ENABLED					\
   |ANALOG_2_ENABLED					\
   |FREQ_0_ENABLED					\
   |ACCEL_0_ENABLED					\
   |ACCEL_1_ENABLED					\
   |ACCEL_2_ENABLED 					\
   |ACCEL_3_ENABLED					\
   |GPS_LAT_ENABLED					\
   |GPS_LON_ENABLED					\
   |GPS_SPEED_ENABLED					\
   |GPS_TIME_ENABLED					\
   |GPS_SATELITE_ENABLED)


class RPMPrinter : public RCPParser 
{
public:

  RPMPrinter(ChannelConfig c) : RCPParser(c) {};

private:
  
  void processSample(float s, ChannelId n)
  {
    if (n == RPM_CHANNEL) {
      printf("%f %lu\n", s, int(n));
    }
  };
};


int main (int argc, char **argv)
{
  RPMPrinter p(CONFIG);
  int c;

  while ((c = fgetc(stdin)) != -1) {
    p.handleChar(c);
  }

  return 0;
}
