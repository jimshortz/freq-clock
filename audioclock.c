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

#define SAMP 48000		/* Sample rate */
#define HI 16383		/* Amplitude of high portion of wave */
#define LO -16384		/* Amplitude of low portion of wave */

int16_t buf[SAMP];		/* Buffer holding 1 second of audio */

/* Generate a 1 second tone of freq and output it to stdout */
void play(int freq) {
    int i,j, period, rem, err,p;
    int cnt=0; 			/* Actual frequency count */
    
    period = SAMP / 2 /freq;	/* Number of samples per half-cycle */
    rem = (SAMP / 2) % freq;	/* Number of partial samples lost in the divide */
    err = 0;			/* Accumulated error */
    
    for (i=0 ; i<SAMP; ) {
	p = period;		/* Period for this iteration */
	if (err*2 >= freq) {
	    /* Add an extra sample if we have accumulated enough error */
	    p += 1;
	    err -= freq;
	}
	cnt++;
	/* Do the high part */
	for (j=0;j<p;j++) {
	    buf[i++] = HI;
	}
	/* And the low part */
	for (j=0;j<p;j++) {
	    buf[i++] = LO;
	}
	
	err += rem;	/* Accumulate the error */
    }

    /* Write diagnostic data to console */
    fprintf(stderr, "freq=%d, period=%d, rem=%d ", freq, period, rem);
    fprintf(stderr, "cnt=%d\n", cnt);

    /* Emit sample data to stdout */
    fwrite(buf, sizeof(buf), 1, stdout);
}

int main(int argc, char**argv) {
    int h,m;

    /* Simulate an HHMM clock going from 0400 to 2359 */
    for (h=1;h<24;h++) {
	for (m=0;m<60;m++) {
	    play(h*100 + m);
	}
    }
}
