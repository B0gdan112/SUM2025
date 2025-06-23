/* FILE NAME: timer.c
 * PROGRAMMER: BS7
 * DATE: 09.06.2022
 * PURPOSE: timer implementation module
 */

#include "anim.h"

typedef unsigned long long UINT64;

static UINT64
    StartTime,    /* Start program time */      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

VOID BS7_TimerInit( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = BS7_Anim.OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  BS7_Anim.IsPause = FALSE;
  BS7_Anim.FPS = 30.0;
  PauseTime = 0; 
}

VOID BS7_TimerResponse( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceCounter(&t);
  /* Global time */
  BS7_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  BS7_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - BS7_Anim.OldTime) / TimePerSec;
  /* Time with pause */
  if (BS7_Anim.IsPause)
  {
    BS7_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - BS7_Anim.OldTime;
  }
  else
  {
    BS7_Anim.DeltaTime = BS7_Anim.GlobalDeltaTime;
    BS7_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
 
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    BS7_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  BS7_Anim.OldTime = t.QuadPart; 
}