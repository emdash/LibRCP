#! /usr/bin/python

import json
import math
import sys
import termios
import time


INTERVAL = 0.1
LOWER_RPM = 3000
UPPER_RPM = 6200
RANGE_RPM = UPPER_RPM - LOWER_RPM
X = RANGE_RPM / 2
B = UPPER_RPM - X

def initserial(path):
    tty = file(path, "r+", 0)
    attrs = termios.tcgetattr(tty)
    attrs[0] &= ~(
        termios.IGNBRK |
        termios.BRKINT |
        termios.PARMRK |
        termios.ISTRIP |
        termios.INLCR  |
        termios.IGNCR  |
        termios.ICRNL  |
        termios.IXON   |
        termios.IXOFF)
    attrs[1] &= ~termios.OPOST
    attrs[2] &= ~(termios.CSIZE  |
                  termios.PARENB |
                  termios.HUPCL)
    attrs[2] |= termios.CS8
    attrs[2] &= ~(
        termios.ECHO   |
        termios.ECHONL |
        termios.ICANON |
        termios.ISIG   |
        termios.IEXTEN)
    attrs[4] = termios.B115200
    attrs[5] = termios.B115200
    attrs[6][termios.VMIN] = 0
    attrs[6][termios.VTIME] = 0
    termios.tcsetattr(tty, termios.TCSANOW, attrs)
    return tty

def mseconds():
    return int(time.time() * 1000)

def sampleRecord(rpm):
    return ('{"s":{"t":%d,"d":'
           '[0.00,0.00,%d,0,0,0,0,0,254]}}' % (mseconds(), rpm))


try:
    path = sys.argv[1]

    if path.startswith("/dev/tty"):
        sp = initserial(path)
    else:
        sp = open(sys.argv[1], "w")

except IndexError:
    sp = sys.stdout

while True:
    rpm = sampleRecord(X * math.sin(time.time() / 2) + B)
    print rpm
    print >> sp, rpm
    sp.flush()
    time.sleep(INTERVAL)
