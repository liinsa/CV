/*** PL0 COMPILER WITH CODE GENERATION ***/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
const  AL    =  10;  /* LENGTH OF IDENTIFIERS */
const  NORW  =  22;  /* # OF RESERVED WORDS */
const  TXMAX = 100;  /* LENGTH OF IDENTIFIER TABLE */
const  NMAX  =  14;  /* MAX NUMBER OF DEGITS IN NUMBERS*/
const  AMAX  =2047;  /* MAXIMUM ADDRESS */
const  LEVMAX=   3;  /* MAX DEPTH OF BLOCK NESTING */
const  CXMAX = 200;  /* SIZE OF CODE ARRAY */

typedef enum  { NUL, IDENT, NUMBER, PLUS, MINUS, TIMES,
	            SLASH, ODDSYM, EQL, NEQ, LSS, LEQ, GTR, GEQ,
	            LPAREN, RPAREN, COMMA, SEMICOLON, PERIOD,
	            BECOMES, BEGINSYM, ENDSYM, IFSYM, THENSYM,ELSESYM,
	            WHILESYM, WRITESYM, READSYM, DOSYM, CALLSYM,
	            CONSTSYM, VARSYM, PROCSYM, PROGSYM,
                    FORSYM,TOSYM,DOWNTOSYM,RETURNSYM,
                    PBECOMES,MBECOMES,PLUSSELF,MINUSSELF,
                    CHARSYM,CHNUMBER,FLOATSYM,ARRAY
        } SYMBOL;
char *SYMOUT[] = {"NUL", "IDENT", "NUMBER", "PLUS", "MINUS", "TIMES",
	    "SLASH", "ODDSYM", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ",
	    "LPAREN", "RPAREN", "COMMA", "SEMICOLON", "PERIOD",
	    "BECOMES", "BEGINSYM", "ENDSYM", "IFSYM", "THENSYM","ELSESYM",
	    "WHILESYM", "WRITESYM", "READSYM", "DOSYM", "CALLSYM",
	    "CONSTSYM", "VARSYM", "PROCSYM", "PROGSYM",
            "FORSYM", "TOSYM", "DOWNTOSYM", "RETURNSYM",
            "PBECOMES", "MBECOMES", "PLUSSELF", "MINUSSELF","CHARSYM"
            ,"CHNUMBER","FLOATSYM","ARRAY"
             };
typedef  int *SYMSET; // SET OF SYMBOL;
typedef  char ALFA[11];
typedef  enum { CONSTANT, VARIABLE, PROCEDUR, CHARBLE, FLOATBLE} OBJECTS ;
typedef  enum { LIT, OPR, LOD, STO, CAL, INI, JMP, JPC } FCT;
typedef struct {
	 FCT F;     /*FUNCTION CODE*/
	 int L; 	/*0..LEVMAX  LEVEL*/
	 float A;     /*0..AMAX    DISPLACEMENT ADDR*/
} INSTRUCTION;
	  /* LIT O A -- LOAD CONSTANT A             */
	  /* OPR 0 A -- EXECUTE OPR A               */
	  /* LOD L A -- LOAD VARIABLE L,A           */
	  /* STO L A -- STORE VARIABLE L,A          */
	  /* CAL L A -- CALL PROCEDURE A AT LEVEL L */
	  /* INI 0 A -- INCREMET T-REGISTER BY A    */
	  /* JMP 0 A -- JUMP TO A                   */
	  /* JPC 0 A -- JUMP CONDITIONAL TO A       */
char   CH;  /*LAST CHAR READ*/
SYMBOL SYM; /*LAST SYMBOL READ*/
ALFA   ID;  /*LAST IDENTIFIER READ*/
float  NUM; /*LAST NUMBER READ*/
char   CHA; /*LAST CHAR READ*/
int    CC;  /*CHARACTER COUNT*/
int    LL;  /*LINE LENGTH*/
int    CX;  /*CODE ALLOCATION INDEX*/
int    arraynumber;
char   LINE[81];
INSTRUCTION  CODE[CXMAX];
ALFA    KWORD[NORW+1];
SYMBOL  WSYM[NORW+1];
SYMBOL  SSYM['^'+1];
ALFA    MNEMONIC[9];
SYMSET  DECLBEGSYS, STATBEGSYS, FACBEGSYS;

struct {
  ALFA NAME;
  OBJECTS KIND;
  union {
    float VAL;   /*CONSTANT*/
    struct { int LEVEL,ADR,SIZE,arraysize;bool arrayflag; } vp;  /*VARIABLE,PROCEDUR:*/
  };
} TABLE[TXMAX];

FILE *FIN,*FOUT;
int ERR;

void EXPRESSION(SYMSET FSYS, int LEV, int &TX);
void TERM(SYMSET FSYS, int LEV, int &TX);
//---------------------------------------------------------------------------
int SymIn(SYMBOL SYM, SYMSET S1) {
  return S1[SYM];
}
//---------------------------------------------------------------------------
SYMSET SymSetUnion(SYMSET S1, SYMSET S2) {
  SYMSET S=(SYMSET)malloc(sizeof(int)*45);
  for (int i=0; i<45; i++)
	if (S1[i] || S2[i]) S[i]=1;
	else S[i]=0;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetAdd(SYMBOL SY, SYMSET S) {
  SYMSET S1;
  S1=(SYMSET)malloc(sizeof(int)*45);
  for (int i=0; i<45; i++) S1[i]=S[i];
  S1[SY]=1;
  return S1;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;  S[b]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;  S[b]=1; S[c]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;  S[b]=1; S[c]=1; S[d]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d,SYMBOL e) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;  S[b]=1; S[c]=1; S[d]=1; S[e]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNew(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d,SYMBOL e, SYMBOL f) {
  SYMSET S; int i,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  S[a]=1;  S[b]=1; S[c]=1; S[d]=1; S[e]=1; S[f]=1;
  return S;
}
//---------------------------------------------------------------------------
SYMSET SymSetNULL() {
  SYMSET S; int i,n,k;
  S=(SYMSET)malloc(sizeof(int)*45);
  for (i=0; i<45; i++) S[i]=0;
  return S;
}
//---------------------------------------------------------------------------
void Error(int n) {
  String s = "***"+AnsiString::StringOfChar(' ', CC-1)+"^";
  Form1->printls(s.c_str(),n);   fprintf(FOUT,"%s%d\n", s.c_str(), n);
  ERR++;
} /*Error*/
//---------------------------------------------------------------------------
void GetCh() {
  if (CC==LL) {
    if (feof(FIN)) {
	  Form1->printfs("PROGRAM INCOMPLETE");
	  fprintf(FOUT,"PROGRAM INCOMPLETE\n");
	  fclose(FOUT);
	  exit(0);
	}
	LL=0; CC=0;
	CH=' ';
	while (!feof(FIN) && CH!=10)
      { CH=fgetc(FIN);  LINE[LL++]=CH; }
	LINE[LL-1]=' ';  LINE[LL]=0;
    String s=IntToStr(CX);
    while(s.Length()<3) s=" "+s;
    s=s+" "+LINE;
	Form1->printfs(s.c_str());
    fprintf(FOUT,"%s\n",s);
  }
  CH=LINE[CC++];
} /*GetCh()*/
//---------------------------------------------------------------------------
void GetSym() {
  int i,J,K;   ALFA  A;
  float pointnumber;
  while (CH<=' ') GetCh();
  if (CH>='A' && CH<='Z') { /*ID OR RESERVED WORD*/
    K=0;
	do {
	  if (K<AL) A[K++]=CH;
	  GetCh();
	}while((CH>='A' && CH<='Z')||(CH>='0' && CH<='9'));
	A[K]='\0';
	strcpy(ID,A); i=1; J=NORW;
	do {
	  K=(i+J) / 2;
	  if (strcmp(ID,KWORD[K])<=0) J=K-1;
	  if (strcmp(ID,KWORD[K])>=0) i=K+1;
	}while(i<=J);
	if (i-1 > J) SYM=WSYM[K];
	else SYM=IDENT;
  }
  else
    if (CH>='0' && CH<='9') { /*NUMBER*/
      K=0; NUM=0; SYM=NUMBER; pointnumber=0.1;
      do {
          NUM=10*NUM+(CH-'0');
          K++; GetCh();
      }while(CH>='0' && CH<='9');
      if(CH=='.')
      {
        GetCh();
        do {
          NUM=NUM+(CH-'0')*pointnumber;
          pointnumber*=0.1;
          GetCh();
        }while(CH>='0' && CH<='9');
      }
      if(CH=='.') Error(35);
	  if (K>NMAX) Error(30);
    }
    else
      if (CH==':') {
	    GetCh();
		if (CH=='=') { SYM=BECOMES; GetCh(); }
		else SYM=NUL;
      }
	  else /* THE FOLLOWING TWO CHECK WERE ADDED
	         BECAUSE ASCII DOES NOT HAVE A SINGLE CHARACTER FOR <= OR >= */
	    if (CH=='<') {
		  GetCh();
		  if (CH=='=') { SYM=LEQ; GetCh(); }
                  else if(CH=='>') { SYM=NEQ; GetCh(); }      /*修改不等号”*/
		  else SYM=LSS;
		}
		else
		  if (CH=='>') {
		    GetCh();
			if (CH=='=') { SYM=GEQ; GetCh(); }
			else SYM=GTR;
          }
     else
       if (CH=='+'){
          GetCh();
          if (CH=='=') {SYM=PBECOMES;GetCh();}             /*增加+=，++符号*/
          else if(CH=='+') {SYM=PLUSSELF;GetCh();}
          else {SYM=SSYM['+'];}
       }
     else
       if (CH=='-'){
          GetCh();
          if (CH=='=') {SYM=MBECOMES;GetCh();}             /*增加-=，--符号*/
          else if(CH=='-') {SYM=MINUSSELF;GetCh();}
          else {SYM=SSYM['-'];}
       }
     else
       if (CH=='\'')
       {
        GetCh();
        CHA=CH;
        GetCh();
        if(CH=='\'') {SYM=CHNUMBER;GetCh();}
        else Error(35);
       }
     else
       if(CH=='[')
       {      arraynumber=0;
        GetCh();
        do {
          arraynumber=10*arraynumber+(CH-'0');
          GetCh();
        }while(CH>='0' && CH<='9');
        if(CH==']') {SYM=ARRAY;GetCh();}
        else Error(35);
       }
     else { SYM=SSYM[CH]; GetCh(); }
} /*GetSym()*/
//---------------------------------------------------------------------------
void GEN(FCT X, int Y, float Z) {
  if (CX>CXMAX) {
    Form1->printfs("PROGRAM TOO LONG");
	fprintf(FOUT,"PROGRAM TOO LONG\n");
	fclose(FOUT);
    exit(0);
  }
  CODE[CX].F=X; CODE[CX].L=Y; CODE[CX].A=Z;
  CX++;
} /*GEN*/
//---------------------------------------------------------------------------
void TEST(SYMSET S1, SYMSET S2, int N) {
  if (!SymIn(SYM,S1)) {
    Error(N);
	while (!SymIn(SYM,SymSetUnion(S1,S2))) GetSym();
  }
} /*TEST*/
//---------------------------------------------------------------------------
void ENTER(OBJECTS K, int LEV, int &TX, int &DX) { /*ENTER OBJECT INTO TABLE*/
  TX++;
  strcpy(TABLE[TX].NAME,ID); TABLE[TX].KIND=K;
  switch (K) {
	case CONSTANT:
	       if (NUM>AMAX) { Error(31); NUM=0; }
	       TABLE[TX].VAL=NUM;
	       break;
        case VARIABLE:
	       TABLE[TX].vp.LEVEL=LEV; TABLE[TX].vp.ADR=DX; DX++;
	       break;
        case CHARBLE:
	       TABLE[TX].vp.LEVEL=LEV; TABLE[TX].vp.ADR=DX; DX++;
	       break;
        case FLOATBLE:
	       TABLE[TX].vp.LEVEL=LEV; TABLE[TX].vp.ADR=DX; DX++;
	       break;
	case PROCEDUR:
	       TABLE[TX].vp.LEVEL=LEV;
	       break;
  }
} /*ENTER*/
//---------------------------------------------------------------------------
int POSITION(ALFA ID, int TX) { /*FIND IDENTIFIER IN TABLE*/
  int i=TX;
  strcpy(TABLE[0].NAME,ID);
  while (strcmp(TABLE[i].NAME,ID)!=0) i--;
  return i;
} /*POSITION*/
//---------------------------------------------------------------------------
int array1(int i)
{
        int j=0;
        if(TABLE[i].vp.arrayflag==1)
                {
                        GetSym();
                        if(SYM==ARRAY)
                        {
                                if(arraynumber>=TABLE[i].vp.arraysize) Error(35);
                                else j=arraynumber;
                        }
                        else Error(35);
                }
        return j;
}
//---------------------------------------------------------------------------
void ConstDeclaration(int LEV,int &TX,int &DX) {
  if (SYM==IDENT) {
    GetSym();
    if (SYM==EQL||SYM==BECOMES) {
	  if (SYM==BECOMES) Error(1);
	  GetSym();
	  if (SYM==NUMBER) { ENTER(CONSTANT,LEV,TX,DX); GetSym(); }
	  else Error(2);
    }
    else Error(3);
  }
  else Error(4);
} /*ConstDeclaration()*/
//---------------------------------------------------------------------------
void VarDeclaration(int LEV,int &TX,int &DX) {
  if (SYM==IDENT)
  {     GetSym();
        if(SYM==ARRAY)
        {       ENTER(VARIABLE,LEV,TX,DX);
                DX=DX+(arraynumber-1);
                TABLE[TX].vp.arraysize=arraynumber;
                TABLE[TX].vp.arrayflag=1;
                GetSym();
        }
        else {ENTER(VARIABLE,LEV,TX,DX);TABLE[TX].vp.arrayflag=0;}
  }
  else Error(4);
} /*VarDeclaration()*/
//---------------------------------------------------------------------------
void FloatDeclaration(int LEV,int &TX,int &DX)
{
  if (SYM==IDENT)
  {     GetSym();
        if(SYM==ARRAY)
        {       ENTER(FLOATBLE,LEV,TX,DX);
                DX=DX+(arraynumber-1);
                TABLE[TX].vp.arraysize=arraynumber;
                TABLE[TX].vp.arrayflag=1;
                GetSym();
        }
        else {ENTER(FLOATBLE,LEV,TX,DX);TABLE[TX].vp.arrayflag=0;}
  }
  else Error(4);
} /*FloatDeclaration()*/
//---------------------------------------------------------------------------
void CharDeclaration(int LEV,int &TX,int &DX)
{
        if(SYM==IDENT)
        {     GetSym();
                if(SYM==ARRAY)
                {       ENTER(CHARBLE,LEV,TX,DX);
                        DX=DX+(arraynumber-1);
                        TABLE[TX].vp.arraysize=arraynumber;
                        TABLE[TX].vp.arrayflag=1;
                        GetSym();
                }
                else {ENTER(CHARBLE,LEV,TX,DX);TABLE[TX].vp.arrayflag=0;}
        }
        else Error(4);
} /*CharDeclaration()*/
//---------------------------------------------------------------------------
void ListCode(int CX0) {  /*LIST CODE GENERATED FOR THIS Block*/
  if (Form1->ListSwitch->ItemIndex==0)
    for (int i=CX0; i<CX; i++) {
      String s=IntToStr(i);
      while(s.Length()<3)s=" "+s;
      s=s+" "+MNEMONIC[CODE[i].F]+" "+IntToStr(CODE[i].L)+" "+IntToStr((int)CODE[i].A);
	  Form1->printfs(s.c_str());
	  fprintf(FOUT,"%3d%5s%4d%4d\n",i,MNEMONIC[CODE[i].F],CODE[i].L,CODE[i].A);
    }
} /*ListCode()*/;
//---------------------------------------------------------------------------
void FACTOR(SYMSET FSYS, int LEV, int &TX) {
  int i,j=0;
  if (SYM==PLUSSELF)
  {
                GetSym();
                if(SYM==IDENT)
                {
                        i=POSITION(ID,TX);
                        j=array1(i);
		        if (i==0) Error(11);
		        else
		        if (TABLE[i].KIND!=VARIABLE&&TABLE[i].KIND!=CHARBLE&&TABLE[i].KIND!=FLOATBLE) {Error(12); i=0;}
                        else
                        {
                        if(i!=0)    {GEN(LIT,0,1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                    GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                    GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                        }
                        GetSym();
                }
                else Error(1);/*++后应接变量*/
  }
  else if (SYM==MINUSSELF)
  {
                GetSym();
                if(SYM==IDENT)
                {
                        i=POSITION(ID,TX);
                        j=array1(i);
		        if (i==0) Error(11);
		        else
		        if (TABLE[i].KIND!=VARIABLE&&TABLE[i].KIND!=CHARBLE&&TABLE[i].KIND!=FLOATBLE) {Error(12); i=0;}
                        else
                        {
                        if(i!=0)   {GEN(LIT,0,-1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                   GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                   GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                        }
                        GetSym();
                }
                else Error(1);/*++后应接变量*/
  }
  else
  {
  TEST(FACBEGSYS,FSYS,24);
  while (SymIn(SYM,FACBEGSYS)) {
	if (SYM==IDENT) {
	  i=POSITION(ID,TX);
          j=array1(i);
	  if (i==0) {Error(11); GetSym();}
	  else
		switch (TABLE[i].KIND) {
		  case CONSTANT: GEN(LIT,0,TABLE[i].VAL); GetSym();break;
                  case VARIABLE:
                  case FLOATBLE:
		  case CHARBLE:  GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j); GetSym();
                                 if (SYM==BECOMES)                       /*变量后面增加赋值表达式，自加自减操作*/
                                 {
                                        GetSym();
                                        EXPRESSION(FSYS,LEV,TX);
                                        if (i!=0) GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                                 else if (SYM==PBECOMES)
                                 {
                                        GetSym();
                                        EXPRESSION(FSYS,LEV,TX);
                                        if (i!=0) {GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                                        GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                 }
                                 else if (SYM==MBECOMES)
                                 {
                                        GetSym();
                                        EXPRESSION(FSYS,LEV,TX);
                                        if (i!=0) {GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        GEN(OPR,0,3); GEN(OPR,0,1); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                                        GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                 }
                                 else if(SYM==PLUSSELF)
                                 {
                                        GetSym();
                                        if(i!=0)  {GEN(LIT,0,1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        }
                                 }
                                 else if(SYM==MINUSSELF)
                                 {
                                        GetSym();
                                        if(i!=0)  {GEN(LIT,0,-1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                                        }
                                 }
                                 break;
		  case PROCEDUR: Error(21); GetSym();break;
		}

	}
	else
	  if (SYM==NUMBER) {
		if (NUM>AMAX) { Error(31); NUM=0; }
		GEN(LIT,0,NUM);
                GetSym();
	  }
        else                      /*字符*/
          if(SYM==CHNUMBER)
          {
                GEN(LIT,0,CHA); GetSym();
          }
        else
          if (SYM==LPAREN) {
                GetSym();
                EXPRESSION(SymSetAdd(RPAREN,FSYS),LEV,TX);
                if (SYM==RPAREN) GetSym();
                else Error(22);
          }

  TEST(FSYS,FACBEGSYS,23);
  }     }
}/*FACTOR*/
//---------------------------------------------------------------------------
void TERM(SYMSET FSYS, int LEV, int &TX) {  /*TERM*/
  SYMBOL MULOP;int i;
  FACTOR(SymSetUnion(FSYS,SymSetNew(TIMES,SLASH)), LEV,TX);
  while (SYM==TIMES || SYM==SLASH) {
	MULOP=SYM;  GetSym();
	FACTOR(SymSetUnion(FSYS,SymSetNew(TIMES,SLASH)),LEV,TX);
	if (MULOP==TIMES) GEN(OPR,0,4);
	else GEN(OPR,0,5);
  }
} /*TERM*/;
//---------------------------------------------------------------------------
void EXPRESSION(SYMSET FSYS, int LEV, int &TX) {
  SYMBOL ADDOP;
  if (SYM==PLUS || SYM==MINUS) {
    ADDOP=SYM; GetSym();
    TERM(SymSetUnion(FSYS,SymSetNew(PLUS,MINUS)),LEV,TX);
    if (ADDOP==MINUS) GEN(OPR,0,1);
  }

  else TERM(SymSetUnion(FSYS,SymSetNew(PLUS,MINUS)),LEV,TX);
  while (SYM==PLUS || SYM==MINUS) {
    ADDOP=SYM; GetSym();
    TERM(SymSetUnion(FSYS,SymSetNew(PLUS,MINUS)),LEV,TX);
    if (ADDOP==PLUS) GEN(OPR,0,2);
    else GEN(OPR,0,3);
  }
} /*EXPRESSION*/
//---------------------------------------------------------------------------
void CONDITION(SYMSET FSYS,int LEV,int &TX) {
  SYMBOL RELOP;
  if (SYM==ODDSYM) { GetSym(); EXPRESSION(FSYS,LEV,TX); GEN(OPR,0,6); }
  else {
	EXPRESSION(SymSetUnion(SymSetNew(EQL,NEQ,LSS,LEQ,GTR,GEQ),FSYS),LEV,TX);
	if (!SymIn(SYM,SymSetNew(EQL,NEQ,LSS,LEQ,GTR,GEQ))) Error(20);
	else {
	  RELOP=SYM; GetSym(); EXPRESSION(FSYS,LEV,TX);
	  switch (RELOP) {
	    case EQL: GEN(OPR,0,8);  break;
	    case NEQ: GEN(OPR,0,9);  break;
	    case LSS: GEN(OPR,0,10); break;
	    case GEQ: GEN(OPR,0,11); break;
	    case GTR: GEN(OPR,0,12); break;
	    case LEQ: GEN(OPR,0,13); break;
	  }
	}
  }
} /*CONDITION*/
//---------------------------------------------------------------------------
void STATEMENT(SYMSET FSYS,int LEV,int &TX) {   /*STATEMENT*/
  int i,CX1,CX2,j=0;
  switch (SYM) {
	case IDENT:
		i=POSITION(ID,TX);
                j=array1(i);
		if (i==0) Error(11);
		else
		  if (TABLE[i].KIND!=VARIABLE&&TABLE[i].KIND!=CHARBLE&&TABLE[i].KIND!=FLOATBLE) { /*ASSIGNMENT TO NON-VARIABLE*/
			Error(12); i=0;
		  }
                GetSym();
		if (SYM==BECOMES)
                {
                        GetSym();
                        EXPRESSION(FSYS,LEV,TX);
		                if (i!=0) GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                else if (SYM==PEQ)           /*+=*/
                {
                        GetSym();
                        EXPRESSION(FSYS,LEV,TX);
                        if (i!=0) {GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                }
                else if (SYM==MEQ)            /*-=*/
                {
                        GetSym();
                        EXPRESSION(FSYS,LEV,TX);
                        if (i!=0) {GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,3); GEN(OPR,0,1); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                }
                else if (SYM==PLUSSELF)           /*++*/
                {
                        GetSym();
                        if(i!=0) {GEN(LIT,0,1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                }
                else if (SYM==MINUSSELF)          /*--*/
                {
                        GetSym();
                        if(i!=0) {GEN(LIT,0,-1); GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,2); GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                }
                else Error(13);
		break;
	case READSYM:
		GetSym();
		if (SYM!=LPAREN) Error(34);
		else
		  do {
			GetSym();
			if (SYM==IDENT) i=POSITION(ID,TX);
			else i=0;
			if (i==0) Error(35);
			else {
			  GEN(OPR,0,16);
			  GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR);
			}
			GetSym();
		  }while(SYM==COMMA);
		if (SYM!=RPAREN) {
		  Error(33);
		  while (!SymIn(SYM,FSYS)) GetSym();
		}
		else GetSym();
		break; /* READSYM */
	case WRITESYM:
		GetSym();
		if (SYM==LPAREN) {
		  do {
			GetSym();
                        if(SYM==IDENT)
                        {
                                i=POSITION(ID,TX);
			        EXPRESSION(SymSetUnion(SymSetNew(RPAREN,COMMA),FSYS),LEV,TX);
			        if (TABLE[i].KIND==CHARBLE) GEN(OPR,0,17);
                                else GEN(OPR,0,14);
                        }
                        else if(SYM==CHNUMBER)
                                {
                                        EXPRESSION(SymSetUnion(SymSetNew(RPAREN,COMMA),FSYS),LEV,TX);
                                        GEN(OPR,0,17);
                                }
                        else
                        {
                                EXPRESSION(SymSetUnion(SymSetNew(RPAREN,COMMA),FSYS),LEV,TX);
                                GEN(OPR,0,14);
                        }

		  }while(SYM==COMMA);
		  if (SYM!=RPAREN) Error(33);
		  else GetSym();
		}
		GEN(OPR,0,15);
		break; /*WRITESYM*/
	case CALLSYM:
		GetSym();
		if (SYM!=IDENT) Error(14);
		else {
		  i=POSITION(ID,TX);
		  if (i==0) Error(11);
		  else
			if (TABLE[i].KIND==PROCEDUR)
			  GEN(CAL,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR);
			else Error(15);
		  GetSym();
		}
		break;
	case IFSYM:
		GetSym();
		CONDITION(SymSetUnion(SymSetNew(THENSYM),FSYS),LEV,TX);
		if (SYM==THENSYM) GetSym();
		else Error(16);
		CX1=CX;  GEN(JPC,0,0);
		STATEMENT(SymSetUnion(SymSetNew(ELSESYM),FSYS),LEV,TX);
                CX2=CX;  GEN(JMP,0,0);
                CODE[CX1].A=CX;
                if(SYM==ELSESYM)              /*ELSE*/
                {  GetSym();  STATEMENT(FSYS,LEV,TX);  }
                CODE[CX2].A=CX;
		break;
	case BEGINSYM:
		GetSym();
		STATEMENT(SymSetUnion(SymSetNew(SEMICOLON,ENDSYM),FSYS),LEV,TX);
		while (SymIn(SYM, SymSetAdd(SEMICOLON,STATBEGSYS))) {
		  if (SYM==SEMICOLON) GetSym();
		  else Error(10);
		  STATEMENT(SymSetUnion(SymSetNew(SEMICOLON,ENDSYM),FSYS),LEV,TX);
		}
		if (SYM==ENDSYM) GetSym();
		else Error(17);
		break;
	case WHILESYM:
		CX1=CX; GetSym(); CONDITION(SymSetAdd(DOSYM,FSYS),LEV,TX);
		CX2=CX; GEN(JPC,0,0);
		if (SYM==DOSYM) GetSym();
		else Error(18);
		STATEMENT(FSYS,LEV,TX);
		GEN(JMP,0,CX1);
		CODE[CX2].A=CX;
		break;
        case FORSYM:                        /*FOR*/
                GetSym();
                if(SYM==IDENT)
                {
                        i=POSITION(ID,TX);
                        j=array1(i);
		        if (i==0) Error(11);
		        else
		        if (TABLE[i].KIND!=VARIABLE) { /*ASSIGNMENT TO NON-VARIABLE*/
			Error(12); i=0;
		        }
                        GetSym();
		        if (SYM==BECOMES) {GetSym();
                        EXPRESSION(SymSetUnion(SymSetNew(TOSYM,DOWNTOSYM),FSYS),LEV,TX);
		        if (i!=0) GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);}
                        CX1=CX;  GEN(JMP,0,0);
                }
                else Error(25);
                if(SYM==TOSYM)
                {
                        GetSym();
                        GEN(LIT,0,1);  GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,2);  GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        CODE[CX1].A=CX;
                        EXPRESSION(SymSetUnion(SymSetNew(DOSYM),FSYS),LEV,TX);
                        GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,12);
                        CX2=CX;  GEN(JPC,0,0);
                }
                else if(SYM==DOWNTOSYM)
                {
                        GetSym();
                        GEN(LIT,0,1);  GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,3);  GEN(OPR,0,1);  GEN(STO,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        CODE[CX1].A=CX;
                        EXPRESSION(SymSetUnion(SymSetNew(DOSYM),FSYS),LEV,TX);
                        GEN(LOD,LEV-TABLE[i].vp.LEVEL,TABLE[i].vp.ADR+j);
                        GEN(OPR,0,10);
                        CX2=CX;  GEN(JPC,0,0);
                }
                else Error(26);
                if(SYM==DOSYM)
                {
                        GetSym();
                        STATEMENT(FSYS,LEV,TX);
                        GEN(JMP,0,CX1+1);
                        CODE[CX2].A=CX;
                }
                else Error(27);
                break;
        case PLUSSELF:                      /*++*/
                EXPRESSION(FSYS,LEV,TX); break;
        case MINUSSELF:                     /*--*/
                EXPRESSION(FSYS,LEV,TX); break;
  }
  TEST(FSYS,SymSetNULL(),19);
} /*STATEMENT*/
//---------------------------------------------------------------------------
void Block(int LEV, int TX, SYMSET FSYS) {
  int DX=3;    /*DATA ALLOCATION INDEX*/
  int TX0=TX;  /*INITIAL TABLE INDEX*/
  int CX0=CX;  /*INITIAL CODE INDEX*/
  TABLE[TX].vp.ADR=CX; GEN(JMP,0,0);
  if (LEV>LEVMAX) Error(32);
  do {
    if (SYM==CONSTSYM) {
      GetSym();
      do {
        ConstDeclaration(LEV,TX,DX);
        while (SYM==COMMA) {
          GetSym();  ConstDeclaration(LEV,TX,DX);
        }
        if (SYM==SEMICOLON) GetSym();
        else Error(5);
      }while(SYM==IDENT);
    }
    if (SYM==VARSYM) {
      GetSym();
      do {
        VarDeclaration(LEV,TX,DX);
        while (SYM==COMMA) { GetSym(); VarDeclaration(LEV,TX,DX); }
	    if (SYM==SEMICOLON) GetSym();
	    else Error(5);
      }while(SYM==IDENT);
    }
    if(SYM==CHARSYM)
    {
        GetSym();
        do
        {
                CharDeclaration(LEV,TX,DX);
                while(SYM==COMMA)
                {
                        GetSym();CharDeclaration(LEV,TX,DX);
                }
                if(SYM==SEMICOLON) GetSym();
                else Error(5);
        }while(SYM==IDENT);
    }
    if(SYM==FLOATSYM)
    {
        GetSym();
        do
        {
                FloatDeclaration(LEV,TX,DX);
                while(SYM==COMMA)
                {
                        GetSym();FloatDeclaration(LEV,TX,DX);
                }
                if(SYM==SEMICOLON) GetSym();
                else Error(5);
        }while(SYM==IDENT);
    }
    while ( SYM==PROCSYM) {
      GetSym();
	  if (SYM==IDENT) { ENTER(PROCEDUR,LEV,TX,DX); GetSym(); }
	  else Error(4);
	  if (SYM==SEMICOLON) GetSym();
	  else Error(5);
	  Block(LEV+1,TX,SymSetAdd(SEMICOLON,FSYS));
	  if (SYM==SEMICOLON) {
	    GetSym();
	    TEST(SymSetUnion(SymSetNew(IDENT,PROCSYM),STATBEGSYS),FSYS,6);
	  }
	  else Error(5);
    }
    TEST(SymSetAdd(IDENT,STATBEGSYS), DECLBEGSYS,7);
  }while(SymIn(SYM,DECLBEGSYS));
  CODE[TABLE[TX0].vp.ADR].A=CX;
  TABLE[TX0].vp.ADR=CX;   /*START ADDR OF CODE*/
  TABLE[TX0].vp.SIZE=DX;  /*SIZE OF DATA SEGMENT*/
  GEN(INI,0,DX);
  STATEMENT(SymSetUnion(SymSetNew(SEMICOLON,ENDSYM),FSYS),LEV,TX);
  GEN(OPR,0,0);  /*RETURN*/
  TEST(FSYS,SymSetNULL(),8);
  ListCode(CX0);
} /*Block*/
//---------------------------------------------------------------------------
int BASE(int L,int B,float S[]) {
  int B1=B; /*FIND BASE L LEVELS DOWN*/
  while (L>0) { B1=S[B1]; L=L-1; }
  return B1;
} /*BASE*/
//---------------------------------------------------------------------------
void Interpret() {
  const STACKSIZE = 500;
  int P,B,T; 		/*PROGRAM BASE TOPSTACK REGISTERS*/
  INSTRUCTION I;
  float S[STACKSIZE];  	/*DATASTORE*/
  Form1->printfs("~~~ RUN PL0 ~~~");
  fprintf(FOUT,"~~~ RUN PL0 ~~~\n");
  T=0; B=1; P=0;
  S[1]=0; S[2]=0; S[3]=0;
  do {
    I=CODE[P]; P=P+1;
    switch (I.F) {
      case LIT: T++; S[T]=I.A;break;
	  case OPR:
	    switch ((int)I.A) { /*OPERATOR*/
	      case 0: /*RETURN*/ T=B-1; P=S[T+3]; B=S[T+2]; break;
	      case 1: S[T]=-S[T];  break;
	      case 2: T--; S[T]=S[T]+S[T+1];   break;
	      case 3: T--; S[T]=S[T]-S[T+1];   break;
	      case 4: T--; S[T]=S[T]*S[T+1];   break;
	      case 5: T--; S[T]=S[T] / S[T+1]; break;
	      case 6: S[T]=((int)S[T]%2!=0);        break;
	      case 8: T--; S[T]=S[T]==S[T+1];  break;
	      case 9: T--; S[T]=S[T]!=S[T+1];  break;
	      case 10: T--; S[T]=S[T]<S[T+1];   break;
	      case 11: T--; S[T]=S[T]>=S[T+1];  break;
	      case 12: T--; S[T]=S[T]>S[T+1];   break;
	      case 13: T--; S[T]=S[T]<=S[T+1];  break;
	      case 14: Form1->printrs("",S[T]); fprintf(FOUT,"%f\n",S[T]); T--;
                   break;
	      case 15: /*Form1->printfs(""); fprintf(FOUT,"\n"); */ break;
	      case 16: T++;  S[T]=InputBox("输入","请键盘输入：", 0).ToInt();
                   Form1->printls("? ",S[T]); fprintf(FOUT,"? %d\n",S[T]);
                   break;
              case 17: Form1->printcs(S[T]);fprintf(FOUT,"%c\n",S[T]); T--;
                   break;
	    }
	    break;
      case LOD: T++; S[T]=S[BASE(I.L,B,S)+(int)I.A]; break;
      case STO: S[BASE(I.L,B,S)+(int)I.A]=S[T]; T--;break;
      case CAL: /*GENERAT NEW Block MARK*/
	      S[T+1]=BASE(I.L,B,S); S[T+2]=B; S[T+3]=P;
	      B=T+1; P=I.A; break;
      case INI: T=T+I.A;  break;
      case JMP: P=I.A; break;
      case JPC: if (S[T]==0) P=I.A;  T--;  break;

    } /*switch*/
  }while(P!=0);
  Form1->printfs("~~~ END PL0 ~~~");
  fprintf(FOUT,"~~~ END PL0 ~~~\n");
} /*Interpret*/
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonRunClick(TObject *Sender) {
  for (CH=' '; CH<='^'; CH++) SSYM[CH]=NUL;
  strcpy(KWORD[ 1],"BEGIN");    strcpy(KWORD[ 2],"CALL");
  strcpy(KWORD[ 3],"CHAR");     strcpy(KWORD[ 4],"CONST");
  strcpy(KWORD[ 5],"DO");       strcpy(KWORD[ 6],"DOWNTO");
  strcpy(KWORD[ 7],"ELSE");     strcpy(KWORD[ 8],"END");
  strcpy(KWORD[ 9],"FLOAT");    strcpy(KWORD[10],"FOR");
  strcpy(KWORD[11],"IF");       strcpy(KWORD[12],"ODD");
  strcpy(KWORD[13],"PROCEDURE");strcpy(KWORD[14],"PROGRAM");
  strcpy(KWORD[15],"READ");     strcpy(KWORD[16],"RETURN");
  strcpy(KWORD[17],"THEN");     strcpy(KWORD[18],"TO");
  strcpy(KWORD[19],"VAR");      strcpy(KWORD[20],"WHILE");
  strcpy(KWORD[21],"WRITE");
  WSYM[ 1]=BEGINSYM;   WSYM[ 2]=CALLSYM;
  WSYM[ 3]=CHARSYM;    WSYM[ 4]=CONSTSYM;
  WSYM[ 5]=DOSYM;      WSYM[ 6]=DOWNTOSYM;
  WSYM[ 7]=ELSESYM;    WSYM[ 8]=ENDSYM;
  WSYM[ 9]=FLOATSYM;   WSYM[10]=FORSYM;
  WSYM[11]=IFSYM;      WSYM[12]=ODDSYM;
  WSYM[13]=PROCSYM;    WSYM[14]=PROGSYM;
  WSYM[15]=READSYM;    WSYM[16]=RETURNSYM;
  WSYM[17]=THENSYM;    WSYM[18]=TOSYM;
  WSYM[19]=VARSYM;     WSYM[20]=WHILESYM;
  WSYM[21]=WRITESYM;

  SSYM['+']=PLUS;      SSYM['-']=MINUS;
  SSYM['*']=TIMES;     SSYM['/']=SLASH;
  SSYM['(']=LPAREN;    SSYM[')']=RPAREN;
  SSYM['=']=EQL;       SSYM[',']=COMMA;
  SSYM['.']=PERIOD;
  SSYM[';']=SEMICOLON;
  strcpy(MNEMONIC[LIT],"LIT");   strcpy(MNEMONIC[OPR],"OPR");
  strcpy(MNEMONIC[LOD],"LOD");   strcpy(MNEMONIC[STO],"STO");
  strcpy(MNEMONIC[CAL],"CAL");   strcpy(MNEMONIC[INI],"INI");
  strcpy(MNEMONIC[JMP],"JMP");   strcpy(MNEMONIC[JPC],"JPC");

  DECLBEGSYS=(int*)malloc(sizeof(int)*45);
  STATBEGSYS=(int*)malloc(sizeof(int)*45);
  FACBEGSYS =(int*)malloc(sizeof(int)*45);
  for(int j=0; j<45; j++) {
	DECLBEGSYS[j]=0;  STATBEGSYS[j]=0;  FACBEGSYS[j] =0;
  }
  DECLBEGSYS[CONSTSYM]=1;
  DECLBEGSYS[VARSYM]=1;
  DECLBEGSYS[CHARSYM]=1;
  DECLBEGSYS[FLOATSYM]=1;
  DECLBEGSYS[PROCSYM]=1;
  STATBEGSYS[BEGINSYM]=1;
  STATBEGSYS[CALLSYM]=1;
  STATBEGSYS[FORSYM]=1;
  STATBEGSYS[IFSYM]=1;
  STATBEGSYS[WHILESYM]=1;
  STATBEGSYS[WRITESYM]=1;
  STATBEGSYS[PLUSSELF]=1;
  STATBEGSYS[MINUSSELF]=1;
  FACBEGSYS[IDENT] =1;
  FACBEGSYS[NUMBER]=1;
  FACBEGSYS[CHNUMBER]=1;
  FACBEGSYS[LPAREN]=1;

  if ((FIN=fopen((Form1->EditName->Text+".PL0").c_str(),"r"))!=0) {
	FOUT=fopen((Form1->EditName->Text+".COD").c_str(),"w");
    Form1->printfs("=== COMPILE PL0 ===");
    fprintf(FOUT,"=== COMPILE PL0 ===\n");
	ERR=0;
	CC=0; CX=0; LL=0; CH=' '; GetSym();
	if (SYM!=PROGSYM) Error(0);
	else {
	  GetSym();
	  if (SYM!=IDENT) Error(0);
	  else {
		GetSym();
		if (SYM!=SEMICOLON) Error(5);
		else GetSym();
	  }
	}
	Block(0,0,SymSetAdd(PERIOD,SymSetUnion(DECLBEGSYS,STATBEGSYS)));
	if (SYM!=PERIOD) Error(9);
	if (ERR==0) Interpret();
	else {
	  Form1->printfs("ERROR IN PL/0 PROGRAM");
	  fprintf(FOUT,"ERROR IN PL/0 PROGRAM");
	}
	fprintf(FOUT,"\n"); fclose(FOUT);
  }
}
//---------------------------------------------------------------------------

