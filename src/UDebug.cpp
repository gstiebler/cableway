//---------------------------------------------------------------------------
#pragma hdrstop
#include "UDebug.h"

#ifdef _DEBUG_BUILDER
void GravaArquivoDouble(void *pont, int tam, int shift)
{
  int orig=(int)pont;
  AnsiString path=ExtractFilePath(Application->ExeName)+"\\";
  TStringList *lista=new TStringList;
  ((byte *)pont)+=shift;
  for (int n=0; n<(int)(tam/sizeof(double)-1); n++)
  {
    lista->Add(FloatToStr(*(double *)(pont))+" "+IntToStr(int(pont)-orig));
    ((double *)pont)++;
  }
  lista->SaveToFile(path+"double"+IntToStr(shift)+".txt");
  delete lista;
}
//---------------------------------------------------------------------------

void GravaArquivoInt(void *pont, int tam, int shift)
{                       
  int orig=(int)pont;
  AnsiString path=ExtractFilePath(Application->ExeName)+"\\";
  TStringList *lista=new TStringList;
  ((byte *)pont)+=shift;
  for (int n=0; n<(int)(tam/sizeof(int)-1); n++)
  {
    lista->Add(IntToStr(*(int *)(pont))+" "+IntToStr(int(pont)-orig));
    ((int *)pont)++;
  }
  lista->SaveToFile(path+"int"+IntToStr(shift)+".txt");
  delete lista;
}
//---------------------------------------------------------------------------

void GravaArquivoByte(void *pont, int tam)
{
  AnsiString path=ExtractFilePath(Application->ExeName)+"byte.txt";
  FILE *arq=fopen(path.c_str(), "w");
  fwrite(pont, 1, tam, arq);
  fclose(arq);
}
//---------------------------------------------------------------------------

void TVerticesGerais::DEBUG_Grava()
{
  string path=ExtractFilePath(Application->ExeName)+"\\";
  TStringList *lista=new TStringList;
  TVerticeGeral *VerticeGeral;
  for (int n=0; n<Tamanho(); n++)
  {
    VerticeGeral=getItem(n);
    lista->Add(IntToStr(n)+";"+VerticeGeral->texto.c_str()+";"+IntToStr(VerticeGeral->iDesenho)+
            ";"+FloatToStr(VerticeGeral->pos.x)+";"+
            FloatToStr(VerticeGeral->pos.y));
  }                                   
  lista->SaveToFile(path+"vertices.csv");
  delete lista;
}
#endif
//---------------------------------------------------------------------------

