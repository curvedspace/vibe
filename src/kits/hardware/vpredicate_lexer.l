%{
#include "vpredicate_parser.h"
#include "vpredicateparse.h"
#include <string.h>
#include <stdlib.h>
#define YY_NO_UNPUT

int VibeHardwarewrap( yyscan_t _scanner );
void VPredicateParse_initLexer( const char *_code, yyscan_t _scanner );
char *VPredicateParse_putSymbol( char *_name );
char *VPredicateParse_putString( char *_str );
void VPredicateParse_initFlex( const char *_code, yyscan_t _scanner );

%}

%option nomain
%option never-interactive
%option noalways-interactive
%option nostack
%option reentrant
%option bison-bridge

DIGIT [0-9]

%%

"==" { return EQ; }
"&" { return MASK; }

[aA][nN][dD] { return AND; }
[oO][rR] { return OR; }
[iI][sS] { return IS; }

[tT][rR][uU][eE] { yylval->valb = 1; return VAL_BOOL; }
[fF][aA][lL][sS][eE] { yylval->valb = 0; return VAL_BOOL; }

"'"[^']*"'" { yylval->name = VPredicateParse_putString( yytext ); return VAL_STRING; }

"-"{DIGIT}+ { yylval->vali = atoi( yytext ); return VAL_NUM; }
{DIGIT}+ { yylval->vali = atoi( yytext ); return VAL_NUM; }

{DIGIT}*"\."{DIGIT}+ { yylval->vald = atof( yytext ); return VAL_FLOAT; }

[a-zA-Z][a-zA-Z0-9\-]* { yylval->name = VPredicateParse_putSymbol( yytext ); return VAL_ID; }

"{"|"}"|"["|"]"|","|"\." { yylval->name = 0; return (int)(*yytext); }

[ \t\n]+ /* eat up whitespace */

. { VPredicateLexer_unknownToken(yytext); }

%%

char *VPredicateParse_putSymbol( char *_name )
{
    char *p = (char*)malloc( strlen( _name ) + 1 );
    if (p != NULL)
    {
        strcpy( p, _name );
    }
    return p;
}

char *VPredicateParse_putString( char *_str )
{
    int l = strlen( _str );
    char *p = (char*)malloc( l );
    char *s = _str + 1;
    char *d = p;

    if (p == NULL)
        return NULL;

    while ( s != _str + l - 1 )
    {
        if ( *s != '\\' )
            *d++ = *s++;
        else
        {
            s++;
            if ( s != _str + l - 1 )
            {
                if ( *s == '\\' )
                    *d++ = '\\';
                else if ( *s == 'n' )
                    *d++ = '\n';
                else if ( *s == 'r' )
                    *d++ = '\r';
                else if ( *s == 't' )
                    *d++ = '\t';
                s++;
            }
         }
    }
    *d = 0;
    return p;
}

void VPredicateParse_initLexer( const char *_code, yyscan_t _scanner )
{
    VibeHardware_switch_to_buffer( VibeHardware_scan_string( _code, _scanner ), _scanner );
}

int VibeHardwarewrap( yyscan_t _scanner )
{
    struct yyguts_t *yyg = (struct yyguts_t*)_scanner;
    VibeHardware_delete_buffer( YY_CURRENT_BUFFER, _scanner );
    return 1;
}

