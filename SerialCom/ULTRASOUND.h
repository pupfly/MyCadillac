#ifndef _ULTRASOUND_
#define _ULTRASOUND_
extern void measure_distanceF(unsigned int *);
extern void measure_distanceF2str(unsigned char *);
extern void send_trigF();
extern void measure_distanceB(unsigned int *);
extern void measure_distanceB2str(unsigned char *);
extern void send_trigB();
#endif