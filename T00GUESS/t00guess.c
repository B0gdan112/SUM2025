/* Sangadzhiev Bogdan */

#include <stdio.h>
#include <windows.h>
#include <conio.h>

void main( void )
{
  int a = 0, b = 100, m = 50;
  char Buf[100];

  sprintf(Buf, "Хочешь, угадаю число?");
  if (MessageBox(NULL, Buf, "Question", MB_YESNO | MB_ICONQUESTION) == IDYES)
  {
    while (a != b)
    {
      m = (a + b) / 2;
      sprintf(Buf, "Число больше %i?", m);
      if (MessageBox(NULL, Buf, "Question", MB_YESNO | MB_ICONQUESTION) == IDYES)
        if (b - a == 1)
        {
          m = b;
          a = b;
        }
        else
          a = m;
      else
        if (b - a == 1)
        {
          m = a;
          b = a;
        }
        else
          b = m;
    }
    sprintf(Buf, "Ваше число %i?", m);
    MessageBox(NULL, Buf, "Question", MB_YESNO | MB_ICONQUESTION);
  }
  else
    MessageBox(NULL, "Ладно T_T", "Information", MB_OK | MB_ICONINFORMATION);
  _getch();
}