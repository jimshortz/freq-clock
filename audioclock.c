/************************************************************************
 * Audio frequency generator.
 *
 * This program generates square waves of specific frequencies.
 * Its intended use is to allow a frequency counter to be used as a
 * a clock display.  The current version is not synced to actual time
 ************************************************************************/
#include <math.h>
#include <pulse/simple.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define SAMP 48000		/* Sample rate */

int debug = 0;
double multiplier = 1;
int use_seconds = 0;

struct timezone utc = {0,0};

pa_simple* pa_handle = NULL;	/* PulseAudio handle */
int latency;
pa_sample_spec ss;

int16_t buf[SAMP];		/* Buffer holding 1 second of audio */

/* Generate 1 second of sine tone */
void sinwav(double freq) {
    int i;
    double f;

    if (debug) {
	printf("freq=%0.2f\n", freq);
    }
    
    f = 2.0*M_PI*freq/(double)SAMP;
    for (i=0;i<SAMP;i++) {
	buf[i] = cos(f*(double)i) * 32767;
    }
}

/* Convert a 24 hour to 12 */
int hour_to_12(int hour) {
    if (hour > 12) {
	return hour - 12;
    } else if (hour == 0) {
	return 12;
    } else {
	return hour;
    }
}

void parse_args(int argc, char** argv) {
    argv++;
    while (argc > 1) {
	if (!strncmp(*argv, "-10", 3)) {
	    multiplier = 10;
	} else if (!strncmp(*argv, "-s", 2)) {
	    use_seconds = 1;
	} else if (!strncmp(*argv, "-d", 2)) {
	    debug = 1;
	} else {
	    fprintf(stderr, "Unknown argument %s\n", *argv);
	    fprintf(stderr, "Usage: audioclock [-10] [-s]\n");
	    fprintf(stderr, "  -10  Multiply frequency by 10\n");
	    fprintf(stderr, "  -s   Include seconds\n");
	    fprintf(stderr, "  -d   Show debug information\n");
	    exit(2);
	}
	argv++;
	argc--;	
    }
}

void setup_audio() {
    int err_code;
    ss.format = PA_SAMPLE_S16NE;
    ss.channels = 1;
    ss.rate = SAMP;

    pa_handle = pa_simple_new(NULL,
			      "AudioClock",
			      PA_STREAM_PLAYBACK,
			      NULL,
			      "Clock",
			      &ss,
			      NULL,
			      NULL,
			      &err_code);
    if (pa_handle == NULL) {
	fprintf(stderr, "Error %d opening pulse audio\n", err_code);
	exit(1);
    }

    /* Detect latency */
    latency = pa_simple_get_latency(pa_handle, &err_code);
    if (debug) {
	printf("Latency=%d\n", latency);
    }   
 }

int main(int argc, char** argv) {
    double freq;
    int err_code = 0;
    int sleep_time;
    struct timeval now;
    struct tm* tm;

    parse_args(argc, argv);
    setup_audio();

    for (;;) {
	/* Sleep until beginnig of next second */
	gettimeofday(&now, &utc);
	sleep_time = 1000000 - now.tv_usec - latency;
	if (sleep_time > 0) {
	    usleep(sleep_time);
	}

	/* Figure out current time */
	now.tv_sec++;
	tm = localtime(&now.tv_sec);
	freq = hour_to_12(tm->tm_hour) * 100 + tm->tm_min;
	if (use_seconds) {
	    freq += tm->tm_sec / 100.0;
	}
	freq *= multiplier;

	/* Play 1 second of tone */
	sinwav(freq);
	if (pa_simple_write(pa_handle, buf, sizeof(buf), &err_code) < 0) {
	    fprintf(stderr, "Error %d writing samples\n", err_code);
	    break;
	}
    }

    pa_simple_free(pa_handle);
    return 0;
}

	
	
	
	
