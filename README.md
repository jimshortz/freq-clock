# Frequency Counter Clock

This is a simple PulseAudio application that will emit an audio
frequency corresponding to the current time.  Its intended use is to
feed a frequency counter to make an obscure clock display.

In the normal mode of operation, it will output hours and minutes
according to the 12 hour clock.  This means that midnight or noon will be
1200Hz, 1PM or 1AM will be 100Hz, and so on.  The range of frequencies
emitted will be between 100-1259Hz.

The -10 flag may be passed to increase the frequency by 10x.  In this
mode, frequencies will range between 1000 (1AM/1PM) to 12590 Hz (12:59
AM/PM).

The -s flag may be passed to include seconds.  In this mode, each
second will add .01Hz to the signal.  It is doubtful this will be
observable.

Lastly, the -d flag will print the frequency/time information to the
console, along with the latency as reported by PulseAudio.

# Prerequisites
The pulse audio libraries must be present.  These can be installed
with the
```
sudo apt install libpulse-dev
```
command under Debian.  `gcc` and `make` are also required of course.

# Building and Running
Just type `make` and run `./audioclock`.
LocalWords:  PulseAudio sudo libpulse dev
