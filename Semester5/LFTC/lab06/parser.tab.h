/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VAR = 258,
     LOAD = 259,
     FILTER = 260,
     MAP = 261,
     SELECT = 262,
     SAVE = 263,
     FOR = 264,
     WHILE = 265,
     IN = 266,
     DO = 267,
     END = 268,
     TO = 269,
     IDENTIFIER = 270,
     STRING = 271,
     NUMBER = 272,
     PLUS = 273,
     MINUS = 274,
     MULTIPLY = 275,
     DIVIDE = 276,
     ASSIGN = 277,
     SEMICOLON = 278,
     COMMA = 279,
     LPAREN = 280,
     RPAREN = 281,
     EQ = 282,
     NEQ = 283,
     GT = 284,
     LT = 285,
     GTE = 286,
     LTE = 287,
     UMINUS = 288
   };
#endif
/* Tokens.  */
#define VAR 258
#define LOAD 259
#define FILTER 260
#define MAP 261
#define SELECT 262
#define SAVE 263
#define FOR 264
#define WHILE 265
#define IN 266
#define DO 267
#define END 268
#define TO 269
#define IDENTIFIER 270
#define STRING 271
#define NUMBER 272
#define PLUS 273
#define MINUS 274
#define MULTIPLY 275
#define DIVIDE 276
#define ASSIGN 277
#define SEMICOLON 278
#define COMMA 279
#define LPAREN 280
#define RPAREN 281
#define EQ 282
#define NEQ 283
#define GT 284
#define LT 285
#define GTE 286
#define LTE 287
#define UMINUS 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

