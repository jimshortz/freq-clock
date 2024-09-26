/************************************************************************
 * Audio frequency generator.
 *
 * This program generates square waves of specific frequencies.
 * Its intended use is to allow a frequency counter to be used as a
 * a clock display.  The current version is not synced to actual time
 ************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define SAMP 48000		/* Sample rate */
int16_t buf[SAMP];		/* Buffer holding 1 second of audio */

struct timezone utc = {0,0};

/* Play 1 second of sine tone */
void sinwav(double freq) {
    int i;
    double f;

    fprintf(stderr,"freq=%0.2f\n", freq);
    
    f = 2.0*M_PI*freq/(double)SAMP;
    for (i=0;i<SAMP;i++) {
	buf[i] = cos(f*(double)i) * 32767;
    }

    fwrite(buf, sizeof(buf), 1, stdout);
}

int hour_to_12(int hour) {
    if (hour > 12) {
	return hour - 12;
    } else if (hour == 0) {
	return 12;
    } else {
	return hour;
    }
}

int main(int argc, char** argv) {
    int sleep_time;
    double freq;
    struct timeval now;
    struct tm* tm;

    for (;;) {
	/* Sleep until next second */
	gettimeofday(&now, &utc);
	sleep_time = (1000000 - now.tv_usec);
	usleep(sleep_time);

	/* Emit 1 second of tone */
	now.tv_sec++;
	tm = localtime(&now.tv_sec);
	freq = hour_to_12(tm->tm_hour) * 100 + tm->tm_min + tm->tm_sec/100.0;
	sinwav(freq);
    }
}

	
	
	
	
