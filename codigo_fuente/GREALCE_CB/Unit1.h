//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
char *GetExeDir(void);
char *GetDir(void);
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button2;
        TButton *Button3;
        TCppWebBrowser *CppWebBrowser1;
        TButton *Button4;
        TButton *Button5;
  TGroupBox *GroupBox1;
  TLabel *Label3;
  TEdit *Edit3;
  TEdit *Edit4;
  TLabel *Label4;
  TLabel *Label19;
  TGroupBox *GroupBox2;
  TGroupBox *GroupBox3;
  TLabel *Label6;
  TEdit *Edit8;
  TEdit *Edit5;
  TLabel *Label7;
  TLabel *Label8;
  TEdit *Edit6;
  TEdit *Edit7;
  TLabel *Label9;
  TLabel *Label5;
  TGroupBox *GroupBox4;
  TEdit *Edit10;
  TEdit *Edit9;
  TLabel *Label12;
  TLabel *Label11;
  TLabel *Label10;
  TPanel *Panel1;
  TLabel *Label2;
  TEdit *Edit2;
  TButton *Button7;
  TGroupBox *GroupBox5;
  TEdit *Edit1;
  TOpenPictureDialog *OpenPictureDialog1;
  TImage *Image1;
  TButton *Button1;
  TLabel *Label1;
  TButton *Button8;
  TComboBox *ComboBox1;
  TComboBox *ComboBox2;
  TGroupBox *GroupBox6;
  TComboBox *ComboBox3;
  TEdit *Edit13;
  TEdit *Edit14;
  TLabel *Label13;
  TLabel *Label14;
  TComboBox *ComboBox4;
  TComboBox *ComboBox5;
  TEdit *Edit15;
  TEdit *Edit16;
  TLabel *Label15;
  TLabel *Label16;
  TEdit *Edit17;
  TLabel *Label17;
  TLabel *Label20;
  TEdit *Edit19;
  TComboBox *ComboBox6;
  TEdit *Edit11;
  TLabel *Label18;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
  void __fastcall Button6Click(TObject *Sender);
  void __fastcall Button8Click(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall ComboBox3Change(TObject *Sender);
  void __fastcall ComboBox5Change(TObject *Sender);
  void __fastcall ComboBox6Change(TObject *Sender);
  void __fastcall ComboBox2Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
