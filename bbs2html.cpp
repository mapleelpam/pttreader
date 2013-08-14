#include "bbs2html.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <QtCore>
#include <QTextCodec>

#define MAX_WORDS 1000


BBS2Html::BBS2Html()
{
}


int ansi_on, ansi_blink;

char *INDEX_TITLE = "[sob-man main]";
char *INDEX_BACKGROUND;
char *INDEX_HEAD_ICON;
char INDEX_COLOR[]="bgcolor=#ddddaa text=#000088 vlink=#8040ff link=#8800ff";
char INDEX_END[]="</BODY></HTML>";

char *ART_TITLE = "[sob-man]";
char *ART_BACKGROUND;
char *ART_HEAD_ICON;
char ART_COLOR[]="bgcolor=#000000 text=#ddffdd vlink=#ffff00 link=#ffff11";
char ART_END[]="</BODY></HTML>";


void tagcolor( char **tag, int attr )
{
   switch( attr ) /* === filter no-used ansi control === */
   {
   case 0: case 5:
   case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
      break;

   default:
      return;
   }


   if( attr==5 )
   {
      if( ansi_blink==0 )
      { ansi_blink=1; strcpy( *tag, "<blink>" ); *tag+=7; }
      return;
   }

   if( ansi_blink ) { strcpy( *tag, "</blink>" ); *tag+=8; }

   if( ansi_on )
   {
      strcpy( *tag, "</FONT>" ); *tag+=7;
      ansi_on=0;
   }


   switch( attr )
   {
   case 0:  ansi_blink=0; return;

   case 30: strcpy( *tag, "<FONT color=gray>" );   *tag+=17;break;
   case 31: strcpy( *tag, "<FONT color=red>" );    *tag+=16;break;
   case 32: strcpy( *tag, "<FONT color=green>" );  *tag+=18;break;
   case 33: strcpy( *tag, "<FONT color=yellow>" ); *tag+=19;break;
   case 34: strcpy( *tag, "<FONT color=blue>" );   *tag+=17;break;
   case 35: strcpy( *tag, "<FONT color=fuchsia>" );*tag+=20;break;
   case 36: strcpy( *tag, "<FONT color=aqua>" );   *tag+=17;break;
   case 37: strcpy( *tag, "<FONT color=white>" );  *tag+=18;break;

   default:
      if( ansi_blink ) { strcpy( *tag, "<blink>" ); *tag+=7; }
      return;
   }

   if( ansi_blink ) { strcpy( *tag, "<blink>" ); *tag+=7; }
   ansi_on=1;
}

int getansi( char **ap, int *attr, char *cmd )
{
   char cattr[100], *cap;

   cap = cattr; *cap=0;
   if( **ap == 0 ) return EOF;

   while( **ap>='0' && **ap<='9' )
   {
      *cap=**ap; cap++; (*ap)++;
   }
   if( cap==cattr ) return 0;

   *cap=0;
   sscanf( cattr, "%d", attr );
   if( **ap == 0 ) return 1;

   *cmd=**ap; (*ap)++;
   if( **ap == 0 ) return 2;
   return 3;
}

void tag8859_1( char **tag, char c )
{
   switch( c )
   {
   case '\"': strcpy( *tag, "&quot" );   *tag+=5;break;
   case '<':  strcpy( *tag, "&lt" );     *tag+=3;break;
   case '>':  strcpy( *tag, "&gt" );     *tag+=3;break;
   case '&':  strcpy( *tag, "&amp" );    *tag+=4;break;

   default:
      **tag=c; (*tag)++;
   }
}

void ansi2tag( char *l )
{
   char tag[MAX_WORDS], esc[3];
   char *ap, *tp, cmd;
   int  attr, num, nextansi;

   esc[2]=0; nextansi=0;
   ap=l; tp=tag;
   while( *ap!=0 )
   {
      esc[0]=ap[0];esc[1]=ap[1];
      if( !strcmp(esc,"[") || nextansi )
      {
         if( nextansi ) nextansi=0; else ap+=2;
         num=getansi( &ap, &attr, &cmd );
         if( num==EOF )
         {
            /* end-of-string */
            break;
         }
         else if( num==0 )
         {
            /* ANSI+noint? eat the word */
            ap++;
         }
         else if( num==1 )
         {
            /* ANSI+int+EOL go next line */
            break;
         }
         else if( num==2 )
         {
            /* ANSI+attr+cmd+EOL, set color and go next line */
            if( cmd=='m') tagcolor( &tp, attr );
            break;
         }
         else if( num==3 )
         {
            /* ANSI+attr+cmd+str, set color.. */
            tagcolor( &tp, attr );
            if( cmd==';') nextansi=1;
         }
      }
      else /* Not ANSI cntrol */
      {
         tag8859_1( &tp, *ap ); ap++;
      }
   }
   *tp = 0;
   strcpy( l, tag );
}

void cut_cr( char *l )
{
   char *i;

   for( i=l; *i!=0; i++ );
   if( *(i-1) != 0 ) *(i-1)=0;
}


void add_href( char *l )
{
   char tag[MAX_WORDS], url[MAX_WORDS];
   char *ap, *tp, *u;
   int found;

   ap=l; tp=tag;

   while( *ap!=0 )
   {
      found=0;

      strncpy( url, ap, 7 ); url[7]=0;
      if( !strcmp(url,"http://") || !strcmp(url,"HTTP://") ) found=1;

      strncpy( url, ap, 6 ); url[6]=0;
      if( !strcmp(url,"ftp://") || !strcmp(url,"FTP://") ) found=1;

      strncpy( url, ap, 7 ); url[7]=0;
      if( !strcmp(url,"file://") || !strcmp(url,"FILE://") ) found=1;

      strncpy( url, ap, 9 ); url[9]=0;
      if( !strcmp(url,"gopher://") || !strcmp(url,"GOPHER://") ) found=1;

      strncpy( url, ap, 9 ); url[9]=0;
      if( !strcmp(url,"telnet://") || !strcmp(url,"TELNET://") ) found=1;

      strncpy( url, ap, 7 ); url[7]=0;
      if( !strcmp(url,"mailto:") || !strcmp(url,"MAILTO:") ) found=1;

      if( found )
      {
         for( u=url; *ap!=0; *u++=*ap++ )
         {
            if( *ap==' ' ) break;
            if( *ap=='\"' ) break;
            if( *ap=='&' ) break;
            if( (unsigned char)*ap>=127 ) break;
         }
         *u=0;

         sprintf( tp, "<A HREF=\"%s\">%s</A>", url, url );
         tp+=(strlen(tp));
      }
      else /* Not URL */
      {
         *tp++=*ap++;
      }
   }

   *tp = 0;
   strcpy( l, tag );
}

int BBS2Html::ann2htmlFile( const char *bbsfile, const char *htmlfile, char *tit )
{
   char l1[MAX_WORDS], title[MAX_WORDS];
   FILE *fi, *fo;
   struct stat st;
   time_t htmltime;

   if( (fi=fopen( bbsfile,"r")) == NULL )
   {
      printf( "ann2html: No input file: %s\n", bbsfile );
      return 1;
   }
   lstat( bbsfile, &st );

   if( fo=fopen( htmlfile,"r") )
   {
      /* === check for update === */
      if( fgets( l1, MAX_WORDS, fo) != NULL )
      {
         if( sscanf(l1,"<!-- BBS2HTML[%lu]",&htmltime)==1 )
            if( htmltime == st.st_mtime )
            {
               fclose( fi ); fclose( fo );
               return 2;
            }
      }
      fclose(fo);
   }

   printf("file %s -> %s\n", bbsfile, htmlfile);

   if( (fo=fopen( htmlfile,"w")) == NULL )
   {
      printf( "ann2html: Can't write html file: %s\n", htmlfile );
      fclose( fi );
      return 1;
   }

   ansi_on = 0;
   ansi_blink = 0;
   strcpy( title, tit );

   ansi2tag( title );
/*
woju
*/
   ART_TITLE = title;

   /* ========== html headers ============= */
   fprintf( fo,
"<!-- BBS2HTML[%lu] Areicle by wcf@CCCA.NCTU.edu.tw  -->\n"
"<HTML>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" />"
"<HEAD>"
"<TITLE>%s</TITLE>"
"</HEAD>"
"\n"
"<BODY background=\"%s\" %s"
">\n"
"<center><font color=\"white\">"
"\n<table><tr><td><pre><hr>"
           , st.st_mtime, title, ART_BACKGROUND,
             ART_COLOR );


   /* ========== text body ============= */
   while( fgets( l1, MAX_WORDS, fi) != NULL )
   {
      cut_cr( l1 );
      ansi2tag( l1 );
      add_href( l1 );
      if( !strcmp( l1, "--" ) )
         fprintf( fo, "<hr>" );
      else
         fprintf( fo, "%s\n", l1 );
   }

   /* ========== end html ============= */
   if( ansi_blink ) fprintf( fo, "</blink>" );
   if( ansi_on ) fprintf( fo, "</FONT>" );
   fprintf( fo, "</pre></td></tr></table></font></center>%s", ART_END );

   fclose( fi ); fclose( fo );
   return 0;
}

int BBS2Html::ann2html( QString& out, const char *bbsfile, char *tit )
{
   char l1[MAX_WORDS], title[MAX_WORDS];
   FILE *fi;
//   /, *fo;
   struct stat st;
//   time_t htmltime;
   QString lalala;
   QTextCodec* big5codec = QTextCodec::codecForName("Big5");

   if( (fi=fopen( bbsfile,"r")) == NULL )
   {
      printf( "ann2html: No input file: %s\n", bbsfile );
      return 1;
   }


   ansi_on = 0;
   ansi_blink = 0;
   strcpy( title, tit );

   ansi2tag( title );
/*
woju
*/
   ART_TITLE = title;

   /* ========== html headers ============= */
   out += lalala.sprintf(
"<!-- BBS2HTML[%lu] Areicle by wcf@CCCA.NCTU.edu.tw  -->\n"
"<HTML>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" />"
"<HEAD>"
"<TITLE>%s</TITLE>"
"</HEAD>"
"\n"
"<BODY background=\"%s\" %s"
">\n"
"<center><font color=\"white\">"
"\n<table><tr><td><pre><hr>"
           , st.st_mtime, title, ART_BACKGROUND,
             ART_COLOR );


   /* ========== text body ============= */
   while( fgets( l1, MAX_WORDS, fi) != NULL )
   {
      cut_cr( l1 );
      ansi2tag( l1 );
      add_href( l1 );

      if( !strcmp( l1, "--" ) )
         out += "<hr>";
      else
          out += big5codec->toUnicode(l1) + "\n";
//                 lalala.sprintf( "%s\n", l1 );
   }

   /* ========== end html ============= */
   if( ansi_blink ) out += QLatin1String("</blink>" );
   if( ansi_on ) out += QLatin1String("</FONT>" );
   out += lalala.sprintf( "</pre></td></tr></table></font></center>%s", ART_END );
//   qDebug() << out ;
   fclose( fi );
//   fclose( fo );
   return 0;
}
