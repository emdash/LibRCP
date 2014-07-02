#include "rcpparse.h"


static unsigned int
population(ChannelConfig config)
{
  unsigned int ret = 0;

  while (config) {
    if (config & 1) {
      ret++;
    }

    config >>= 1;
  }

  return ret;
}


RCPParser::RCPParser(ChannelConfig c)
{
  config = c;
}


void
RCPParser::reset()
{
  state = IGNORE_EVERYTHING;
  cursample = 0; 
};


void
RCPParser::resetValue()
{
  value = 0;
  negate = false;
  multiplicand = .1;
}


void
RCPParser::pushSample(float v)
{
  samples[cursample++] = negate ? -v : v;
  resetValue();
}


void 
RCPParser::handleChar(char c)
{
  switch (state) {
  case IGNORE_EVERYTHING:
    if (c == '[') {
      state = SAMPLE_FIRST_CHAR;
      resetValue();
    }
    break;

  case SAMPLE_FIRST_CHAR:
    if (c == '-') {
      negate = true;
      state = SAMPLE_NTH_CHAR;
      break;
    }

    /* fallthrough ! */

  case SAMPLE_NTH_CHAR:
    if (NUMERIC(c)) {
      value *= 10;
      value += (c - '0');
    } else if (c == '.') {
      state = SAMPLE_HAVE_DECIMAL;
    } else if (c == ',') {
      pushSample(value);
      state = SAMPLE_FIRST_CHAR;
    } else if (c == ']') {
      pushSample(value);
      processSamples(samples, cursample);
      reset();
    } else {
      reset();
    }
    break;

  case SAMPLE_HAVE_DECIMAL:
    if (NUMERIC(c)) {
      value += (c - '0') * multiplicand;
      multiplicand /= 10;
    } else if (c == ',') {
      pushSample(value);
      state = SAMPLE_FIRST_CHAR;
    } else if (c == ']') {
      pushSample(value);
      processSamples(samples, cursample);
      reset();
    } else {
      reset();
    }
    break;

  };
}


void
RCPParser::processSamples(float samples[],
			  unsigned int n)
{
  ChannelConfig configBitmap = config;
  // XXX: this will break if the number of samples exceeds the integer
  // precision of a 32-bit float (23-ish bits).
  uint32_t sampleRecordBitmap = (uint32_t) samples[n - 1];
  unsigned int channel = 0;
  unsigned int i = 0;

  while (configBitmap) {
    if (configBitmap & 1) {
      if (sampleRecordBitmap & 1) {
	processSample(samples[i++], ChannelId(channel));
      }
      sampleRecordBitmap >>= 1;
    }

    configBitmap >>= 1;
    channel += 1;
  }
}
