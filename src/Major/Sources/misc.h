#ifndef MISC_H
#define MISC_H

void start_count (void);

float end_count (void);

float average (int num_samples, float *array);

float sum (int num_samples, float *array);

float clock_to_time (unsigned long clock_cycles);

#endif
