/* ******************************************************************** **
** @@ Antiword
** @  Copyrt : 
** @  Author : 
** @  Update :
** @  Update :
** @  Notes  : Replace M$Word typographica with normal one
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "..\shared\file_find.h"

/* ******************************************************************** **
** @@                   internal #defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif 

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Proceed()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Proceed(const char* const pszFileName)
{
   char     pszBackName[_MAX_PATH];
   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];

   _splitpath(pszFileName,pszDrive,pszDir,pszFName,pszExt);
   _makepath( pszBackName,pszDrive,pszDir,pszFName,"BAK");

   CopyFile(pszFileName,pszBackName,FALSE);

   FILE*    pIn = fopen(pszBackName,"rt");

   if (!pIn)
   {
      perror("\a\nOpen Input File Error !\n");
      return;
   }

   FILE*    pOut = fopen(pszFileName,"wb");

   if (!pOut)
   {
      perror("\a\nOpen Output File Error !\n");
      fclose(pIn);
      pIn = NULL;
      return;
   }

   int   iChr = 0;

   while ((iChr = fgetc(pIn)) != EOF)
   {
      switch (iChr)         
      {  
         case 0x85:
         {
            fprintf(pOut,"...");
            break;
         }
         case 0x96:
         case 0x97:
         {
            fputc(0x2D,pOut);
            break;
         }
         case 0xA0:
         {
            fputc(0x20,pOut);
            break;
         }
         case 0xA9:
         {
            fprintf(pOut,"(c)");
            break;
         }
         case 0xAB:
         case 0xBB:
         {
            fputc(0x22,pOut);
            break;
         }
         default:
         {
            fputc(iChr,pOut);
            break;
         }
      }
   }

   fclose(pIn);
   pIn = NULL;

   fclose(pOut);
   pOut = NULL;
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void ShowHelp()
{
   const char  pszCopyright[] = "-*-   Antiword 1.0   *   Copyright (c) Gazlan, 2014   -*-";
   const char  pszDescript [] = "Replace M$Word typographica with normal one";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: antiword.com wildcards\n\n");
   printf("%s\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int main(int argc,char** argv)
{
   if ((argc < 2) || (argc > 4))
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2 && ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h"))))
   {
      ShowHelp();
      return 0;
   }

   FindFile   FF;

   FF.SetMask(argv[1]);

   while (FF.Fetch())
   {
      if ((FF._w32fd.dwFileAttributes | FILE_ATTRIBUTE_NORMAL) && !(FF._w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         Proceed(FF._w32fd.cFileName);
      }
   }

   return 0;
}

/* ******************************************************************** **
** @@                   The End
** ******************************************************************** */
