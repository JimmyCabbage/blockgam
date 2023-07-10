#ifndef TEBRIS_G_TICKTIMER_H
#define TEBRIS_G_TICKTIMER_H

#include <stdint.h>

typedef struct ticktimer_s ticktimer_t;

#define TICK_RATE (64)

ticktimer_t* G_CreateTimer();

void G_DestroyTimer(ticktimer_t* timer);

uint64_t G_GetTimerTicks(ticktimer_t* timer);

#endif //TEBRIS_G_TICKTIMER_H
