#include "unit1.h"
//#include "GuiAux.cpp"
#include <stdio.h>

static HANDLE hProcessGrealce=NULL;

int write_blank_html(char *dir, int RefreshSecs=0) {
	FILE *f;
  char dirfil[512];

  //mkdir("prueba");
  strcpy(dirfil,dir);
  strcat(dirfil,"vista.htm");
	f=fopen(dirfil,"w+");
	if (f==NULL) {
		return -1;
	}
	fprintf(f,"<html><head>\n");
  if (RefreshSecs!=0)
	  fprintf(f,"<meta http-equiv=\"refresh\" content=\"%i;URL=vista.htm;target=_top\">\n",RefreshSecs);
	fprintf(f,"<title>Realce de imagenes grises mediante algoritmo genético</title>\n");
	fprintf(f,"</head>\n<body>\n");
	fprintf(f,"<b><u>Realce de imagenes grises mediante algoritmo genético</u></b><br>\n");
	fprintf(f,"\n</body>\n</html>\n");
	fclose(f);
	return 0;
}

BOOL MyCreateSubProcess(char *exefilename, char *cmdline, int priority) {
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    char *cmd;

    cmd=(char *)malloc(sizeof(char)*strlen(exefilename)+strlen(cmdline)+5);
    strcpy(cmd,exefilename);
    strcat(cmd," ");
    strcat(cmd,cmdline);

    /* Set up members of STARTUPINFO structure. */
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.lpReserved = NULL;
    siStartInfo.lpReserved2 = NULL;
    siStartInfo.cbReserved2 = 0;
    siStartInfo.lpDesktop = NULL;
    siStartInfo.dwFlags = STARTF_USESHOWWINDOW; //0 si no usa showwindow
    siStartInfo.wShowWindow=SW_MINIMIZE; //para q sea creada minimizada
    /* Elegir prioridad: 'idle' o 'normal' */
    DWORD  dwCreationFlags;
    if (priority==0) dwCreationFlags=IDLE_PRIORITY_CLASS;
    else dwCreationFlags=NORMAL_PRIORITY_CLASS;
    /* Create the child process. */
    CreateProcess(
        exefilename,
        cmd,       /* command line                       */
        NULL,          /* process security attributes        */
        NULL,          /* primary thread security attributes */
        TRUE,          /* handles are inherited              */
        dwCreationFlags,  /* creation flags                     */
        NULL,          /* use parent's environment           */
        NULL,          /* use parent's current directory     */

        &siStartInfo,  /* STARTUPINFO pointer                */
        &piProcInfo);  /* receives PROCESS_INFORMATION       */

    hProcessGrealce=piProcInfo.hProcess;
    free(cmd);
    return 0;
}

int TerminateGrealce(void) {
  if (hProcessGrealce!=NULL)
    if (TerminateProcess(hProcessGrealce,-1)==TRUE) {
      Form1->Button8->Enabled=true;
      Form1->Button2->Enabled=true;
    }
  return 0;
}

int ExecuteGRealce(void) {
        int i;
        char exedirfil[512];
        char t[512]="";

        strcat(t,Form1->Edit1->Text.c_str()); //nombre de archivo
        strcat(t," ");
        strcat(t,Form1->Edit2->Text.c_str()); //Tamaño de poblacion
        strcat(t," ");
        strcat(t,Form1->Edit3->Text.c_str()); //Maximas iteraciones
        strcat(t," ");
        strcat(t,Form1->Edit4->Text.c_str());  //error tolerado
        strcat(t," ");
        strcat(t,AnsiString(Form1->ComboBox3->ItemIndex).c_str()); //distorsion a original
        strcat(t," ");
        strcat(t,AnsiString(Form1->ComboBox5->ItemIndex).c_str()); //ruido a imagen original
        strcat(t," ");
        strcat(t,AnsiString(Form1->ComboBox1->ItemIndex).c_str()); //tipo de cromosomas
        strcat(t," ");
        strcat(t,AnsiString(Form1->ComboBox2->ItemIndex).c_str()); //distorsion a cromosomas
        strcat(t," ");
        strcat(t,AnsiString(Form1->ComboBox6->ItemIndex).c_str()); //funcion de evaluación
        strcat(t," ");
        strcat(t,Form1->Edit8->Text.c_str()); //crossprob[0]
        strcat(t," ");
        strcat(t,Form1->Edit5->Text.c_str()); //crossprob[1]
        strcat(t," ");
        strcat(t,Form1->Edit6->Text.c_str()); //crossprob[2]
        strcat(t," ");
        strcat(t,Form1->Edit7->Text.c_str()); //crossprob[3]
        strcat(t," ");
        strcat(t,Form1->Edit13->Text.c_str());//cte cruce uniforme
        strcat(t," ");
        strcat(t,Form1->Edit9->Text.c_str()); //mutprob[0]
        strcat(t," ");
        strcat(t,Form1->Edit10->Text.c_str()); //mutprob[1]
        strcat(t," ");
        strcat(t,Form1->Edit14->Text.c_str());//cte mut no-uniforme
        strcat(t," ");
        strcat(t,Form1->Edit15->Text.c_str());//cte A desenfoque imagen
        strcat(t," ");
        strcat(t,Form1->Edit16->Text.c_str());//cte sigma ruido
        strcat(t," ");
        strcat(t,Form1->Edit17->Text.c_str());//cte mu ruido
        strcat(t," ");
        strcat(t,Form1->Edit11->Text.c_str());//cte A desenfoque cromosomas
        strcat(t," ");
        strcat(t,Form1->Edit19->Text.c_str());//cte lambda Laplaciana

        strcpy(exedirfil,GetExeDir());
        strcat(exedirfil,"grealce.exe");

        //Form1->Label19->Caption=AnsiString(t);


    write_blank_html(GetDir(),1);
    if (hProcessGrealce!=NULL) TerminateGrealce(); //por si ya hay uno, lo cerramos
      MyCreateSubProcess(exedirfil,t,Form1->ComboBox4->ItemIndex);
    return 0;
}



