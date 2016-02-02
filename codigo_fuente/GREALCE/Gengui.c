#include "gengui.h"
#include <stdio.h>

#ifdef CBuilder
  #include "UnitGenetic.h"
  //#include <vcl.h>
#endif

#ifdef CVIP_tools
  //#include <stdlib.h>
#endif

static FILE *logfile=NULL;

#ifdef CVIP_tools
void log_mes(char *string)  {
  if (logfile==NULL) {
    logfile=fopen("logfile.txt","w+");
  }
  fprintf(logfile,string);
  printf("%s",string);
}
#endif

#ifdef CBuilder
void log_mes(char *string) {
  if (logfile==NULL) {
    logfile=fopen("logfile.txt","w+");
  }
  fprintf(logfile,string);
  FormGenetic->Memo1->Lines->Add(AnsiString(string));
}
#endif