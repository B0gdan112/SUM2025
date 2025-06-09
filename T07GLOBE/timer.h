/* FILE NAME: timer.h
 * PROGRAMMER: BS7
 * DATE: 09.06.2022
 * PURPOSE: timer implementation module
 */


#include <windows.h>

extern double
  GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
  Time, DeltaTime,             /* Time with pause and interframe interval */
  FPS;                         /* Frames per second value */
extern BOOL IsPause;

VOID GLB_TimerInit( VOID );
VOID GLB_TimerResponse( VOID );