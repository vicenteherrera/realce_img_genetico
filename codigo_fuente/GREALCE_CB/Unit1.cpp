//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include <dir.h>
#include <stdio.h>
#include <string.h>
#include "Unit1.h"
#include "Unit2.h"
#include "GuiAux.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
char dir[512]="C:\\pid\\";
char exedir[512]="C:\\pid\\";
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner){
  int i;

  //Obtenemos el directorio del ejecutable
  GetCurrentDirectory(sizeof(exedir),exedir);
  //Añadimos '\' al final
  i=strlen(exedir);
  exedir[i]='\\';exedir[i+1]='\0';
  //Copiamos el dir a la ruta de imagen
  strcpy(dir,exedir);
  //Edit1->Text=AnsiString(exedir);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender) {
  int i;
  if (OpenPictureDialog1->Execute()) // Si se ha elegido un archivo
      try {
        // Intentar cargarlo
        Image1->Picture->LoadFromFile(OpenPictureDialog1->FileName);
        Caption = "Visualizar Gráficos - (" + OpenPictureDialog1->FileName + ")";
        Edit1->Text=OpenPictureDialog1->FileName;

        //Establecemos la ruta de la imagen
        strcpy(dir,OpenPictureDialog1->FileName.c_str());
        i=strlen(dir);
        while (dir[i]!='\\' && i>=0)
          i--;
        dir[i]='\\';dir[i+1]='\0';
      } catch(...) {
             ShowMessage("No es posible cargar el archivo");
      }

   WideString url=WideString("file:")+WideString(dir)+WideString("vista.htm");
   write_blank_html(dir);

   CppWebBrowser1->Navigate(url);
}
//---------------------------------------------------------------------------
void DisableOptionsControls(void) {
   Form1->Edit2->Enabled=false;
   Form1->Edit3->Enabled=false;
   Form1->Edit4->Enabled=false;
   Form1->Edit5->Enabled=false;
   Form1->Edit6->Enabled=false;
   Form1->Edit7->Enabled=false;
   Form1->Edit8->Enabled=false;
   Form1->Edit9->Enabled=false;
   Form1->Edit10->Enabled=false;
   Form1->Button1->Enabled=false;
   Form1->Edit1->Enabled=false;
   Form1->Button2->Enabled=false;
   Form1->Button8->Enabled=true;
   Form1->ComboBox1->Enabled=false;
   Form1->ComboBox2->Enabled=false;
   Form1->ComboBox3->Enabled=false;
   Form1->ComboBox4->Enabled=false;
   Form1->ComboBox5->Enabled=false;
   Form1->ComboBox6->Enabled=false;
   Form1->Edit13->Enabled=false;
   Form1->Edit14->Enabled=false;

   Form1->Edit15->Enabled=false;
   Form1->Edit16->Enabled=false;
   Form1->Edit17->Enabled=false;
   Form1->Edit19->Enabled=false;
   Form1->Edit11->Enabled=false;


}
void EnableOptionsControls(void) {
   Form1->Edit2->Enabled=true;
   Form1->Edit3->Enabled=true;
   Form1->Edit4->Enabled=true;
   Form1->Edit5->Enabled=true;
   Form1->Edit6->Enabled=true;
   Form1->Edit7->Enabled=true;
   Form1->Edit8->Enabled=true;
   Form1->Edit9->Enabled=true;
   Form1->Edit10->Enabled=true;
   Form1->Button1->Enabled=true;
   Form1->Edit1->Enabled=true;
   Form1->Button2->Enabled=true;
   Form1->Button8->Enabled=false;
   Form1->ComboBox1->Enabled=true;
   Form1->ComboBox2->Enabled=true;
   Form1->ComboBox3->Enabled=true;
   Form1->ComboBox4->Enabled=true;
   Form1->ComboBox5->Enabled=true;
   Form1->ComboBox6->Enabled=true;
   Form1->Edit13->Enabled=true;
   Form1->Edit14->Enabled=true;
   if (Form1->ComboBox3->ItemIndex==2) Form1->Edit15->Enabled=true;
   if (Form1->ComboBox5->ItemIndex==1) {
     Form1->Edit16->Enabled=true;
     Form1->Edit17->Enabled=true;
   }
   if (Form1->ComboBox6->ItemIndex==1) Form1->Edit19->Enabled=true;
   if (Form1->ComboBox2->ItemIndex==2) Form1->Edit11->Enabled=true;
}
char *GetExeDir(void) {
  return exedir;
}
char *GetDir(void) {
  return dir;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender) {
   WideString url=WideString("file:")+WideString(dir)+WideString("vista.htm");
   DisableOptionsControls();
   ExecuteGRealce();
   CppWebBrowser1->Navigate(url);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender) {
        WideString url=WideString("file:")+WideString(dir)+WideString("vista.htm");
        TerminateGrealce();
        EnableOptionsControls();
        write_blank_html(dir,1);
        CppWebBrowser1->Navigate(url);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender) {
        WideString url=WideString("file:")+WideString(dir)+WideString("vista.htm");
        CppWebBrowser1->Navigate(url);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender) {
  AnsiString url=AnsiString("file:")+AnsiString(dir)+AnsiString("vista.htm");
  ::ShellExecute(NULL, NULL, url.c_str(), NULL,NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender) {
  Form2->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender) {
  TerminateGrealce();
  CppWebBrowser1->Refresh();
  EnableOptionsControls();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
  CppWebBrowser1->Width=521-(797-(Form1->Width));
  CppWebBrowser1->Height=553-(615-(Form1->Height));
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ComboBox3Change(TObject *Sender)
{
  if (ComboBox3->ItemIndex!=2) Edit15->Enabled=false;
  else Edit15->Enabled=true;}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox5Change(TObject *Sender)
{
  if (ComboBox5->ItemIndex!=1) {
    Edit16->Enabled=false;
    Edit17->Enabled=false;
  }
  else {
    Edit16->Enabled=true;
    Edit17->Enabled=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox6Change(TObject *Sender)
{
  if (ComboBox6->ItemIndex==1) Edit19->Enabled=true;
  else Edit19->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{
  if (ComboBox2->ItemIndex==2) Edit11->Enabled=true;
  else Edit11->Enabled=false;
}
//---------------------------------------------------------------------------

