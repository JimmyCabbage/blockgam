#include "g_ticktimer.h"

#include "SDL.h"

struct ticktimer_s
{
    uint64_t startTime;
};

ticktimer_t* G_CreateTimer()
{
    SDL_InitSubSystem(SDL_INIT_TIMER);
    
    ticktimer_t* timer = malloc(sizeof(ticktimer_t));
    
    timer->startTime = SDL_GetTicks64();
    
    return timer;
}

void G_DestroyTimer(ticktimer_t* timer)
{
    free(timer);
    
    SDL_QuitSubSystem(SDL_INIT_TIMER);
}

uint64_t G_GetTimerTicks(ticktimer_t* timer)
{
    const uint64_t ticks = SDL_GetTicks64() - timer->startTime;
    return (ticks * TICK_RATE) / 1000;
}
