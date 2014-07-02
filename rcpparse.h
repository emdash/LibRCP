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

#ifndef RCPPARSE_H
#define RCPPARSE_H

#include <stdint.h>

#define MAX_SAMPLES 50
#define NUMERIC(c) (('0' <= c) && (c <= '9'))

// It's worse than I thought -- even with the bitmap, channel IDs
// depend on the current config. Both the sender and receiver configs
// have to be in sync with respect to which channels are enabled.

#define CHANNELS(CHANNEL)			\
  CHANNEL(ANALOG_0)				\
  CHANNEL(ANALOG_1)				\
  CHANNEL(ANALOG_2)				\
  CHANNEL(ANALOG_3)				\
  CHANNEL(ANALOG_4)				\
  CHANNEL(ANALOG_5)				\
  CHANNEL(ANALOG_6)				\
  CHANNEL(ANALOG_7)				\
  CHANNEL(FREQ_0)				\
  CHANNEL(FREQ_1)				\
  CHANNEL(FREQ_2)				\
  CHANNEL(ACCEL_0)				\
  CHANNEL(ACCEL_1)				\
  CHANNEL(ACCEL_2)				\
  CHANNEL(ACCEL_3)				\
  CHANNEL(PWM_0)				\
  CHANNEL(PWM_1)				\
  CHANNEL(PWM_2)				\
  CHANNEL(GPS_LAT)				\
  CHANNEL(GPS_LON)				\
  CHANNEL(GPS_SPEED)				\
  CHANNEL(GPS_TIME)				\
  CHANNEL(GPS_SATELITE)				\
  CHANNEL(TRACK_LAP_COUNT)			\
  CHANNEL(TRACK_LAP_TIME)			\
  CHANNEL(TRACK_SPLIT_TIME)			\
  CHANNEL(TRACK_DISTANCE)			\
  CHANNEL(TRACK_PREDICTED_TIME)


// Define an enum value for each possible channel

#define CHANNEL_ENUM(name)			\
  name,
typedef enum {
  CHANNELS(CHANNEL_ENUM)
  CHANNELS_END
} ChannelId;

#define ENABLE(name) name |
#define EVERYTHING_ENABLED (CHANNELS(ENABLE) 0)


// Number of all possible channels, not just enabled ones
#define MAX_CHANNELS (int(CHANNELS_END) - 1)

// Define a bit mask for each possible channel, and a data-type to
// represent the a channel configuration

#define CHANNEL_MASKS(name)			\
  const ChannelConfig name##_ENABLED = (1 << (uint32_t) name);
typedef uint32_t ChannelConfig;
CHANNELS(CHANNEL_MASKS)


// This class encapsulates the sate machine for the RCP JSON
// protocol. To use it, derive a subclass that overrides
// processSample(). Then, feed characters into it with
// handleChar(). processSample() will be called for each sample
// received.

class RCPParser {

private:

  enum {
    IGNORE_EVERYTHING,
    SAMPLE_FIRST_CHAR,
    SAMPLE_NTH_CHAR,
    SAMPLE_HAVE_DECIMAL,
  } state = IGNORE_EVERYTHING;

  ChannelConfig  config;
  float          samples[MAX_CHANNELS];
  uint32_t       cursample = 0;
  bool           negate = false;
  float          value = 0;
  float          multiplicand = 0;

  void reset();
  void resetValue();
  void pushSample(float v);
  void processSamples(float samples[],
		      unsigned int n);

  RCPParser();
  RCPParser(RCPParser &);

public:

  RCPParser(ChannelConfig c);
  void handleChar(char c);

protected:

  virtual void processSample(float s,
			     ChannelId i) = 0;

};

#endif
