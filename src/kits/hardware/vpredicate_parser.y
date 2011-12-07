%{
#include <stdlib.h>
#include <stdio.h>
#include "vpredicate_parser.h"
#include "vpredicateparse.h"

#define YYLTYPE_IS_TRIVIAL 0
#define YYENABLE_NLS 0
#define YYLEX_PARAM scanner
#define YYPARSE_PARAM scanner
typedef void* yyscan_t;
void VibeHardwareerror(const char *s);
int VibeHardwarelex( YYSTYPE *yylval, yyscan_t scanner );
int VibeHardwarelex_init( yyscan_t *scanner );
int VibeHardwarelex_destroy( yyscan_t *scanner );
void VPredicateParse_initLexer( const char *s, yyscan_t scanner );
void VPredicateParse_mainParse( const char *_code );

%}

%union
{
     char valb;
     int vali;
     double vald;
     char *name;
     void *ptr;
}

%token EQ
%token MASK

%token AND
%token OR
%token IS

%token <valb> VAL_BOOL
%token <name> VAL_STRING
%token <name> VAL_ID
%token <vali> VAL_NUM
%token <vald> VAL_FLOAT

%type <ptr> predicate
%type <ptr> predicate_atom
%type <ptr> predicate_or
%type <ptr> predicate_and
%type <ptr> string_list
%type <ptr> string_list_rec
%type <ptr> value

%destructor { VPredicateParse_destroy( $$ ); } predicate
%destructor { VPredicateParse_destroy( $$ ); } predicate_atom
%destructor { VPredicateParse_destroy( $$ ); } predicate_or
%destructor { VPredicateParse_destroy( $$ ); } predicate_and

%pure-parser

%%

predicate: predicate_atom { VPredicateParse_setResult( $<ptr>1 ); $$ = $<ptr>1; }
         | '[' predicate_or ']' { VPredicateParse_setResult( $<ptr>2 ); $$ = $<ptr>2; }
         | '[' predicate_and ']' { VPredicateParse_setResult( $<ptr>2 ); $$ = $<ptr>2; }

predicate_atom: VAL_ID '.' VAL_ID EQ value { $$ = VPredicateParse_newAtom( $<name>1, $<name>3, $<ptr>5 ); }
              | VAL_ID '.' VAL_ID MASK value { $$ = VPredicateParse_newMaskAtom( $<name>1, $<name>3, $<ptr>5 ); }
              | IS VAL_ID { $$ = VPredicateParse_newIsAtom( $<name>2 ); }

predicate_or: predicate OR predicate { $$ = VPredicateParse_newOr( $<ptr>1, $<ptr>3 ); }

predicate_and: predicate AND predicate { $$ = VPredicateParse_newAnd( $<ptr>1, $<ptr>3 ); }

value: VAL_STRING { $$ = VPredicateParse_newStringValue( $<name>1 ); }
     | VAL_BOOL { $$ = VPredicateParse_newBoolValue( $<valb>1 ); }
     | VAL_NUM { $$ = VPredicateParse_newNumValue( $<vali>1 ); }
     | VAL_FLOAT { $$ = VPredicateParse_newDoubleValue( $<vald>1 ); }
     | string_list { $$ = $<ptr>1; }

string_list: '{' string_list_rec '}' { $$ = $<ptr>1; }

string_list_rec: /* empty */ { $$ = VPredicateParse_newEmptyStringListValue(); }
               | VAL_STRING { $$ = VPredicateParse_newStringListValue( $<ptr>1 ); }
               | VAL_STRING ',' string_list_rec { $$ = VPredicateParse_appendStringListValue( $<name>1, $<ptr>3 ); }

%%

void VibeHardwareerror ( const char *s )  /* Called by VibeHardwareparse on error */
{
    VPredicateParse_errorDetected(s);
}

void VPredicateParse_mainParse( const char *_code )
{
    yyscan_t scanner;
    VibeHardwarelex_init( &scanner );
    VPredicateParse_initLexer( _code, scanner );
    VibeHardwareparse( scanner );
    VibeHardwarelex_destroy( scanner );
}

