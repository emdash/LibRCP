// The MIT License (MIT)
//
//   Copyright (c) 2014 Brandon Lewis
//
//   Permission is hereby granted, free of charge, to any person
//   obtaining a copy of this software and associated documentation
//   files (the "Software"), to deal in the Software without
//   restriction, including without limitation the rights to use,
//   copy, modify, merge, publish, distribute, sublicense, and/or sell
//   copies of the Software, and to permit persons to whom the
//   Software is furnished to do so, subject to the following
//   conditions:
//
//   The above copyright notice and this permission notice shall be
//   included in all copies or substantial portions of the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//   OTHER DEALINGS IN THE SOFTWARE.

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
