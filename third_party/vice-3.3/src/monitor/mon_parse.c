/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "mon_parse.y"

/* -*- C -*-
 *
 * mon_parse.y - Parser for the VICE built-in monitor.
 *
 * Written by
 *  Daniel Sladic <sladic@eecg.toronto.edu>
 *  Andreas Boose <viceteam@t-online.de>
 *  Thomas Giesel <skoe@directbox.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#if !defined(MACOS_COMPILE) && !(defined(__OS2__) && defined(IDE_COMPILE))
#ifdef __GNUC__
#undef alloca
#ifndef ANDROID_COMPILE
#define        alloca(n)       __builtin_alloca (n)
#endif
#else
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#else  /* Not HAVE_ALLOCA_H.  */
#if !defined(_AIX) && !defined(WINCE)
#ifndef _MSC_VER
extern char *alloca();
#else
#define alloca(n)   _alloca(n)
#endif  /* MSVC */
#endif /* Not AIX and not WINCE.  */
#endif /* HAVE_ALLOCA_H.  */
#endif /* GCC.  */
#endif /* MACOS OS2 */

/* SunOS 4.x specific stuff */
#if defined(sun) || defined(__sun)
#  if !defined(__SVR4) && !defined(__svr4__)
#    ifdef __sparc__
#      define YYFREE
#    endif
#  endif
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "asm.h"
#include "console.h"
#include "lib.h"
#include "machine.h"
#include "mon_breakpoint.h"
#include "mon_command.h"
#include "mon_disassemble.h"
#include "mon_drive.h"
#include "mon_file.h"
#include "mon_memmap.h"
#include "mon_memory.h"
#include "mon_register.h"
#include "mon_util.h"
#include "montypes.h"
#include "resources.h"
#include "types.h"
#include "uimon.h"

#ifdef AMIGA_MORPHOS
#undef REG_PC
#endif

#define join_ints(x,y) (LO16_TO_HI16(x)|y)
#define separate_int1(x) (HI16_TO_LO16(x))
#define separate_int2(x) (LO16(x))

static int yyerror(char *s);
static int temp;
static int resolve_datatype(unsigned guess_type, const char *num);
static int resolve_range(enum t_memspace memspace, MON_ADDR range[2],
                         const char *num);

#ifdef __IBMC__
static void __yy_memcpy (char *to, char *from, int count);
#endif

/* Defined in the lexer */
extern int new_cmd, opt_asm;
extern void free_buffer(void);
extern void make_buffer(char *str);
extern int yylex(void);
extern int cur_len, last_len;

#define ERR_ILLEGAL_INPUT 1     /* Generic error as returned by yacc.  */
#define ERR_RANGE_BAD_START 2
#define ERR_RANGE_BAD_END 3
#define ERR_BAD_CMD 4
#define ERR_EXPECT_CHECKNUM 5
#define ERR_EXPECT_END_CMD 6
#define ERR_MISSING_CLOSE_PAREN 7
#define ERR_INCOMPLETE_COMPARE_OP 8
#define ERR_EXPECT_FILENAME 9
#define ERR_ADDR_TOO_BIG 10
#define ERR_IMM_TOO_BIG 11
#define ERR_EXPECT_STRING 12
#define ERR_UNDEFINED_LABEL 13
#define ERR_EXPECT_DEVICE_NUM 14
#define ERR_EXPECT_ADDRESS 15
#define ERR_INVALID_REGISTER 16

#define BAD_ADDR (new_addr(e_invalid_space, 0))
#define CHECK_ADDR(x) ((x) == addr_mask(x))

#define YYDEBUG 1


#line 205 "mon_parse.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_MON_PARSE_H_INCLUDED
# define YY_YY_MON_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    H_NUMBER = 258,                /* H_NUMBER  */
    D_NUMBER = 259,                /* D_NUMBER  */
    O_NUMBER = 260,                /* O_NUMBER  */
    B_NUMBER = 261,                /* B_NUMBER  */
    CONVERT_OP = 262,              /* CONVERT_OP  */
    B_DATA = 263,                  /* B_DATA  */
    H_RANGE_GUESS = 264,           /* H_RANGE_GUESS  */
    D_NUMBER_GUESS = 265,          /* D_NUMBER_GUESS  */
    O_NUMBER_GUESS = 266,          /* O_NUMBER_GUESS  */
    B_NUMBER_GUESS = 267,          /* B_NUMBER_GUESS  */
    BAD_CMD = 268,                 /* BAD_CMD  */
    MEM_OP = 269,                  /* MEM_OP  */
    IF = 270,                      /* IF  */
    MEM_COMP = 271,                /* MEM_COMP  */
    MEM_DISK8 = 272,               /* MEM_DISK8  */
    MEM_DISK9 = 273,               /* MEM_DISK9  */
    MEM_DISK10 = 274,              /* MEM_DISK10  */
    MEM_DISK11 = 275,              /* MEM_DISK11  */
    EQUALS = 276,                  /* EQUALS  */
    TRAIL = 277,                   /* TRAIL  */
    CMD_SEP = 278,                 /* CMD_SEP  */
    LABEL_ASGN_COMMENT = 279,      /* LABEL_ASGN_COMMENT  */
    CMD_SIDEFX = 280,              /* CMD_SIDEFX  */
    CMD_RETURN = 281,              /* CMD_RETURN  */
    CMD_BLOCK_READ = 282,          /* CMD_BLOCK_READ  */
    CMD_BLOCK_WRITE = 283,         /* CMD_BLOCK_WRITE  */
    CMD_UP = 284,                  /* CMD_UP  */
    CMD_DOWN = 285,                /* CMD_DOWN  */
    CMD_LOAD = 286,                /* CMD_LOAD  */
    CMD_SAVE = 287,                /* CMD_SAVE  */
    CMD_VERIFY = 288,              /* CMD_VERIFY  */
    CMD_IGNORE = 289,              /* CMD_IGNORE  */
    CMD_HUNT = 290,                /* CMD_HUNT  */
    CMD_FILL = 291,                /* CMD_FILL  */
    CMD_MOVE = 292,                /* CMD_MOVE  */
    CMD_GOTO = 293,                /* CMD_GOTO  */
    CMD_REGISTERS = 294,           /* CMD_REGISTERS  */
    CMD_READSPACE = 295,           /* CMD_READSPACE  */
    CMD_WRITESPACE = 296,          /* CMD_WRITESPACE  */
    CMD_RADIX = 297,               /* CMD_RADIX  */
    CMD_MEM_DISPLAY = 298,         /* CMD_MEM_DISPLAY  */
    CMD_BREAK = 299,               /* CMD_BREAK  */
    CMD_TRACE = 300,               /* CMD_TRACE  */
    CMD_IO = 301,                  /* CMD_IO  */
    CMD_BRMON = 302,               /* CMD_BRMON  */
    CMD_COMPARE = 303,             /* CMD_COMPARE  */
    CMD_DUMP = 304,                /* CMD_DUMP  */
    CMD_UNDUMP = 305,              /* CMD_UNDUMP  */
    CMD_EXIT = 306,                /* CMD_EXIT  */
    CMD_DELETE = 307,              /* CMD_DELETE  */
    CMD_CONDITION = 308,           /* CMD_CONDITION  */
    CMD_COMMAND = 309,             /* CMD_COMMAND  */
    CMD_ASSEMBLE = 310,            /* CMD_ASSEMBLE  */
    CMD_DISASSEMBLE = 311,         /* CMD_DISASSEMBLE  */
    CMD_NEXT = 312,                /* CMD_NEXT  */
    CMD_STEP = 313,                /* CMD_STEP  */
    CMD_PRINT = 314,               /* CMD_PRINT  */
    CMD_DEVICE = 315,              /* CMD_DEVICE  */
    CMD_HELP = 316,                /* CMD_HELP  */
    CMD_WATCH = 317,               /* CMD_WATCH  */
    CMD_DISK = 318,                /* CMD_DISK  */
    CMD_QUIT = 319,                /* CMD_QUIT  */
    CMD_CHDIR = 320,               /* CMD_CHDIR  */
    CMD_BANK = 321,                /* CMD_BANK  */
    CMD_LOAD_LABELS = 322,         /* CMD_LOAD_LABELS  */
    CMD_SAVE_LABELS = 323,         /* CMD_SAVE_LABELS  */
    CMD_ADD_LABEL = 324,           /* CMD_ADD_LABEL  */
    CMD_DEL_LABEL = 325,           /* CMD_DEL_LABEL  */
    CMD_SHOW_LABELS = 326,         /* CMD_SHOW_LABELS  */
    CMD_CLEAR_LABELS = 327,        /* CMD_CLEAR_LABELS  */
    CMD_RECORD = 328,              /* CMD_RECORD  */
    CMD_MON_STOP = 329,            /* CMD_MON_STOP  */
    CMD_PLAYBACK = 330,            /* CMD_PLAYBACK  */
    CMD_CHAR_DISPLAY = 331,        /* CMD_CHAR_DISPLAY  */
    CMD_SPRITE_DISPLAY = 332,      /* CMD_SPRITE_DISPLAY  */
    CMD_TEXT_DISPLAY = 333,        /* CMD_TEXT_DISPLAY  */
    CMD_SCREENCODE_DISPLAY = 334,  /* CMD_SCREENCODE_DISPLAY  */
    CMD_ENTER_DATA = 335,          /* CMD_ENTER_DATA  */
    CMD_ENTER_BIN_DATA = 336,      /* CMD_ENTER_BIN_DATA  */
    CMD_KEYBUF = 337,              /* CMD_KEYBUF  */
    CMD_BLOAD = 338,               /* CMD_BLOAD  */
    CMD_BSAVE = 339,               /* CMD_BSAVE  */
    CMD_SCREEN = 340,              /* CMD_SCREEN  */
    CMD_UNTIL = 341,               /* CMD_UNTIL  */
    CMD_CPU = 342,                 /* CMD_CPU  */
    CMD_YYDEBUG = 343,             /* CMD_YYDEBUG  */
    CMD_BACKTRACE = 344,           /* CMD_BACKTRACE  */
    CMD_SCREENSHOT = 345,          /* CMD_SCREENSHOT  */
    CMD_PWD = 346,                 /* CMD_PWD  */
    CMD_DIR = 347,                 /* CMD_DIR  */
    CMD_RESOURCE_GET = 348,        /* CMD_RESOURCE_GET  */
    CMD_RESOURCE_SET = 349,        /* CMD_RESOURCE_SET  */
    CMD_LOAD_RESOURCES = 350,      /* CMD_LOAD_RESOURCES  */
    CMD_SAVE_RESOURCES = 351,      /* CMD_SAVE_RESOURCES  */
    CMD_ATTACH = 352,              /* CMD_ATTACH  */
    CMD_DETACH = 353,              /* CMD_DETACH  */
    CMD_MON_RESET = 354,           /* CMD_MON_RESET  */
    CMD_TAPECTRL = 355,            /* CMD_TAPECTRL  */
    CMD_CARTFREEZE = 356,          /* CMD_CARTFREEZE  */
    CMD_CPUHISTORY = 357,          /* CMD_CPUHISTORY  */
    CMD_MEMMAPZAP = 358,           /* CMD_MEMMAPZAP  */
    CMD_MEMMAPSHOW = 359,          /* CMD_MEMMAPSHOW  */
    CMD_MEMMAPSAVE = 360,          /* CMD_MEMMAPSAVE  */
    CMD_COMMENT = 361,             /* CMD_COMMENT  */
    CMD_LIST = 362,                /* CMD_LIST  */
    CMD_STOPWATCH = 363,           /* CMD_STOPWATCH  */
    RESET = 364,                   /* RESET  */
    CMD_EXPORT = 365,              /* CMD_EXPORT  */
    CMD_AUTOSTART = 366,           /* CMD_AUTOSTART  */
    CMD_AUTOLOAD = 367,            /* CMD_AUTOLOAD  */
    CMD_MAINCPU_TRACE = 368,       /* CMD_MAINCPU_TRACE  */
    CMD_LABEL_ASGN = 369,          /* CMD_LABEL_ASGN  */
    L_PAREN = 370,                 /* L_PAREN  */
    R_PAREN = 371,                 /* R_PAREN  */
    ARG_IMMEDIATE = 372,           /* ARG_IMMEDIATE  */
    REG_A = 373,                   /* REG_A  */
    REG_X = 374,                   /* REG_X  */
    REG_Y = 375,                   /* REG_Y  */
    COMMA = 376,                   /* COMMA  */
    INST_SEP = 377,                /* INST_SEP  */
    L_BRACKET = 378,               /* L_BRACKET  */
    R_BRACKET = 379,               /* R_BRACKET  */
    LESS_THAN = 380,               /* LESS_THAN  */
    REG_U = 381,                   /* REG_U  */
    REG_S = 382,                   /* REG_S  */
    REG_PC = 383,                  /* REG_PC  */
    REG_PCR = 384,                 /* REG_PCR  */
    REG_B = 385,                   /* REG_B  */
    REG_C = 386,                   /* REG_C  */
    REG_D = 387,                   /* REG_D  */
    REG_E = 388,                   /* REG_E  */
    REG_H = 389,                   /* REG_H  */
    REG_L = 390,                   /* REG_L  */
    REG_AF = 391,                  /* REG_AF  */
    REG_BC = 392,                  /* REG_BC  */
    REG_DE = 393,                  /* REG_DE  */
    REG_HL = 394,                  /* REG_HL  */
    REG_IX = 395,                  /* REG_IX  */
    REG_IY = 396,                  /* REG_IY  */
    REG_SP = 397,                  /* REG_SP  */
    REG_IXH = 398,                 /* REG_IXH  */
    REG_IXL = 399,                 /* REG_IXL  */
    REG_IYH = 400,                 /* REG_IYH  */
    REG_IYL = 401,                 /* REG_IYL  */
    PLUS = 402,                    /* PLUS  */
    MINUS = 403,                   /* MINUS  */
    STRING = 404,                  /* STRING  */
    FILENAME = 405,                /* FILENAME  */
    R_O_L = 406,                   /* R_O_L  */
    OPCODE = 407,                  /* OPCODE  */
    LABEL = 408,                   /* LABEL  */
    BANKNAME = 409,                /* BANKNAME  */
    CPUTYPE = 410,                 /* CPUTYPE  */
    MON_REGISTER = 411,            /* MON_REGISTER  */
    COMPARE_OP = 412,              /* COMPARE_OP  */
    RADIX_TYPE = 413,              /* RADIX_TYPE  */
    INPUT_SPEC = 414,              /* INPUT_SPEC  */
    CMD_CHECKPT_ON = 415,          /* CMD_CHECKPT_ON  */
    CMD_CHECKPT_OFF = 416,         /* CMD_CHECKPT_OFF  */
    TOGGLE = 417,                  /* TOGGLE  */
    MASK = 418                     /* MASK  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define H_NUMBER 258
#define D_NUMBER 259
#define O_NUMBER 260
#define B_NUMBER 261
#define CONVERT_OP 262
#define B_DATA 263
#define H_RANGE_GUESS 264
#define D_NUMBER_GUESS 265
#define O_NUMBER_GUESS 266
#define B_NUMBER_GUESS 267
#define BAD_CMD 268
#define MEM_OP 269
#define IF 270
#define MEM_COMP 271
#define MEM_DISK8 272
#define MEM_DISK9 273
#define MEM_DISK10 274
#define MEM_DISK11 275
#define EQUALS 276
#define TRAIL 277
#define CMD_SEP 278
#define LABEL_ASGN_COMMENT 279
#define CMD_SIDEFX 280
#define CMD_RETURN 281
#define CMD_BLOCK_READ 282
#define CMD_BLOCK_WRITE 283
#define CMD_UP 284
#define CMD_DOWN 285
#define CMD_LOAD 286
#define CMD_SAVE 287
#define CMD_VERIFY 288
#define CMD_IGNORE 289
#define CMD_HUNT 290
#define CMD_FILL 291
#define CMD_MOVE 292
#define CMD_GOTO 293
#define CMD_REGISTERS 294
#define CMD_READSPACE 295
#define CMD_WRITESPACE 296
#define CMD_RADIX 297
#define CMD_MEM_DISPLAY 298
#define CMD_BREAK 299
#define CMD_TRACE 300
#define CMD_IO 301
#define CMD_BRMON 302
#define CMD_COMPARE 303
#define CMD_DUMP 304
#define CMD_UNDUMP 305
#define CMD_EXIT 306
#define CMD_DELETE 307
#define CMD_CONDITION 308
#define CMD_COMMAND 309
#define CMD_ASSEMBLE 310
#define CMD_DISASSEMBLE 311
#define CMD_NEXT 312
#define CMD_STEP 313
#define CMD_PRINT 314
#define CMD_DEVICE 315
#define CMD_HELP 316
#define CMD_WATCH 317
#define CMD_DISK 318
#define CMD_QUIT 319
#define CMD_CHDIR 320
#define CMD_BANK 321
#define CMD_LOAD_LABELS 322
#define CMD_SAVE_LABELS 323
#define CMD_ADD_LABEL 324
#define CMD_DEL_LABEL 325
#define CMD_SHOW_LABELS 326
#define CMD_CLEAR_LABELS 327
#define CMD_RECORD 328
#define CMD_MON_STOP 329
#define CMD_PLAYBACK 330
#define CMD_CHAR_DISPLAY 331
#define CMD_SPRITE_DISPLAY 332
#define CMD_TEXT_DISPLAY 333
#define CMD_SCREENCODE_DISPLAY 334
#define CMD_ENTER_DATA 335
#define CMD_ENTER_BIN_DATA 336
#define CMD_KEYBUF 337
#define CMD_BLOAD 338
#define CMD_BSAVE 339
#define CMD_SCREEN 340
#define CMD_UNTIL 341
#define CMD_CPU 342
#define CMD_YYDEBUG 343
#define CMD_BACKTRACE 344
#define CMD_SCREENSHOT 345
#define CMD_PWD 346
#define CMD_DIR 347
#define CMD_RESOURCE_GET 348
#define CMD_RESOURCE_SET 349
#define CMD_LOAD_RESOURCES 350
#define CMD_SAVE_RESOURCES 351
#define CMD_ATTACH 352
#define CMD_DETACH 353
#define CMD_MON_RESET 354
#define CMD_TAPECTRL 355
#define CMD_CARTFREEZE 356
#define CMD_CPUHISTORY 357
#define CMD_MEMMAPZAP 358
#define CMD_MEMMAPSHOW 359
#define CMD_MEMMAPSAVE 360
#define CMD_COMMENT 361
#define CMD_LIST 362
#define CMD_STOPWATCH 363
#define RESET 364
#define CMD_EXPORT 365
#define CMD_AUTOSTART 366
#define CMD_AUTOLOAD 367
#define CMD_MAINCPU_TRACE 368
#define CMD_LABEL_ASGN 369
#define L_PAREN 370
#define R_PAREN 371
#define ARG_IMMEDIATE 372
#define REG_A 373
#define REG_X 374
#define REG_Y 375
#define COMMA 376
#define INST_SEP 377
#define L_BRACKET 378
#define R_BRACKET 379
#define LESS_THAN 380
#define REG_U 381
#define REG_S 382
#define REG_PC 383
#define REG_PCR 384
#define REG_B 385
#define REG_C 386
#define REG_D 387
#define REG_E 388
#define REG_H 389
#define REG_L 390
#define REG_AF 391
#define REG_BC 392
#define REG_DE 393
#define REG_HL 394
#define REG_IX 395
#define REG_IY 396
#define REG_SP 397
#define REG_IXH 398
#define REG_IXL 399
#define REG_IYH 400
#define REG_IYL 401
#define PLUS 402
#define MINUS 403
#define STRING 404
#define FILENAME 405
#define R_O_L 406
#define OPCODE 407
#define LABEL 408
#define BANKNAME 409
#define CPUTYPE 410
#define MON_REGISTER 411
#define COMPARE_OP 412
#define RADIX_TYPE 413
#define INPUT_SPEC 414
#define CMD_CHECKPT_ON 415
#define CMD_CHECKPT_OFF 416
#define TOGGLE 417
#define MASK 418

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 135 "mon_parse.y"

    MON_ADDR a;
    MON_ADDR range[2];
    int i;
    REG_ID reg;
    CONDITIONAL cond_op;
    cond_node_t *cond_node;
    RADIXTYPE rt;
    ACTION action;
    char *str;
    asm_mode_addr_info_t mode;

#line 597 "mon_parse.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MON_PARSE_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_H_NUMBER = 3,                   /* H_NUMBER  */
  YYSYMBOL_D_NUMBER = 4,                   /* D_NUMBER  */
  YYSYMBOL_O_NUMBER = 5,                   /* O_NUMBER  */
  YYSYMBOL_B_NUMBER = 6,                   /* B_NUMBER  */
  YYSYMBOL_CONVERT_OP = 7,                 /* CONVERT_OP  */
  YYSYMBOL_B_DATA = 8,                     /* B_DATA  */
  YYSYMBOL_H_RANGE_GUESS = 9,              /* H_RANGE_GUESS  */
  YYSYMBOL_D_NUMBER_GUESS = 10,            /* D_NUMBER_GUESS  */
  YYSYMBOL_O_NUMBER_GUESS = 11,            /* O_NUMBER_GUESS  */
  YYSYMBOL_B_NUMBER_GUESS = 12,            /* B_NUMBER_GUESS  */
  YYSYMBOL_BAD_CMD = 13,                   /* BAD_CMD  */
  YYSYMBOL_MEM_OP = 14,                    /* MEM_OP  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_MEM_COMP = 16,                  /* MEM_COMP  */
  YYSYMBOL_MEM_DISK8 = 17,                 /* MEM_DISK8  */
  YYSYMBOL_MEM_DISK9 = 18,                 /* MEM_DISK9  */
  YYSYMBOL_MEM_DISK10 = 19,                /* MEM_DISK10  */
  YYSYMBOL_MEM_DISK11 = 20,                /* MEM_DISK11  */
  YYSYMBOL_EQUALS = 21,                    /* EQUALS  */
  YYSYMBOL_TRAIL = 22,                     /* TRAIL  */
  YYSYMBOL_CMD_SEP = 23,                   /* CMD_SEP  */
  YYSYMBOL_LABEL_ASGN_COMMENT = 24,        /* LABEL_ASGN_COMMENT  */
  YYSYMBOL_CMD_SIDEFX = 25,                /* CMD_SIDEFX  */
  YYSYMBOL_CMD_RETURN = 26,                /* CMD_RETURN  */
  YYSYMBOL_CMD_BLOCK_READ = 27,            /* CMD_BLOCK_READ  */
  YYSYMBOL_CMD_BLOCK_WRITE = 28,           /* CMD_BLOCK_WRITE  */
  YYSYMBOL_CMD_UP = 29,                    /* CMD_UP  */
  YYSYMBOL_CMD_DOWN = 30,                  /* CMD_DOWN  */
  YYSYMBOL_CMD_LOAD = 31,                  /* CMD_LOAD  */
  YYSYMBOL_CMD_SAVE = 32,                  /* CMD_SAVE  */
  YYSYMBOL_CMD_VERIFY = 33,                /* CMD_VERIFY  */
  YYSYMBOL_CMD_IGNORE = 34,                /* CMD_IGNORE  */
  YYSYMBOL_CMD_HUNT = 35,                  /* CMD_HUNT  */
  YYSYMBOL_CMD_FILL = 36,                  /* CMD_FILL  */
  YYSYMBOL_CMD_MOVE = 37,                  /* CMD_MOVE  */
  YYSYMBOL_CMD_GOTO = 38,                  /* CMD_GOTO  */
  YYSYMBOL_CMD_REGISTERS = 39,             /* CMD_REGISTERS  */
  YYSYMBOL_CMD_READSPACE = 40,             /* CMD_READSPACE  */
  YYSYMBOL_CMD_WRITESPACE = 41,            /* CMD_WRITESPACE  */
  YYSYMBOL_CMD_RADIX = 42,                 /* CMD_RADIX  */
  YYSYMBOL_CMD_MEM_DISPLAY = 43,           /* CMD_MEM_DISPLAY  */
  YYSYMBOL_CMD_BREAK = 44,                 /* CMD_BREAK  */
  YYSYMBOL_CMD_TRACE = 45,                 /* CMD_TRACE  */
  YYSYMBOL_CMD_IO = 46,                    /* CMD_IO  */
  YYSYMBOL_CMD_BRMON = 47,                 /* CMD_BRMON  */
  YYSYMBOL_CMD_COMPARE = 48,               /* CMD_COMPARE  */
  YYSYMBOL_CMD_DUMP = 49,                  /* CMD_DUMP  */
  YYSYMBOL_CMD_UNDUMP = 50,                /* CMD_UNDUMP  */
  YYSYMBOL_CMD_EXIT = 51,                  /* CMD_EXIT  */
  YYSYMBOL_CMD_DELETE = 52,                /* CMD_DELETE  */
  YYSYMBOL_CMD_CONDITION = 53,             /* CMD_CONDITION  */
  YYSYMBOL_CMD_COMMAND = 54,               /* CMD_COMMAND  */
  YYSYMBOL_CMD_ASSEMBLE = 55,              /* CMD_ASSEMBLE  */
  YYSYMBOL_CMD_DISASSEMBLE = 56,           /* CMD_DISASSEMBLE  */
  YYSYMBOL_CMD_NEXT = 57,                  /* CMD_NEXT  */
  YYSYMBOL_CMD_STEP = 58,                  /* CMD_STEP  */
  YYSYMBOL_CMD_PRINT = 59,                 /* CMD_PRINT  */
  YYSYMBOL_CMD_DEVICE = 60,                /* CMD_DEVICE  */
  YYSYMBOL_CMD_HELP = 61,                  /* CMD_HELP  */
  YYSYMBOL_CMD_WATCH = 62,                 /* CMD_WATCH  */
  YYSYMBOL_CMD_DISK = 63,                  /* CMD_DISK  */
  YYSYMBOL_CMD_QUIT = 64,                  /* CMD_QUIT  */
  YYSYMBOL_CMD_CHDIR = 65,                 /* CMD_CHDIR  */
  YYSYMBOL_CMD_BANK = 66,                  /* CMD_BANK  */
  YYSYMBOL_CMD_LOAD_LABELS = 67,           /* CMD_LOAD_LABELS  */
  YYSYMBOL_CMD_SAVE_LABELS = 68,           /* CMD_SAVE_LABELS  */
  YYSYMBOL_CMD_ADD_LABEL = 69,             /* CMD_ADD_LABEL  */
  YYSYMBOL_CMD_DEL_LABEL = 70,             /* CMD_DEL_LABEL  */
  YYSYMBOL_CMD_SHOW_LABELS = 71,           /* CMD_SHOW_LABELS  */
  YYSYMBOL_CMD_CLEAR_LABELS = 72,          /* CMD_CLEAR_LABELS  */
  YYSYMBOL_CMD_RECORD = 73,                /* CMD_RECORD  */
  YYSYMBOL_CMD_MON_STOP = 74,              /* CMD_MON_STOP  */
  YYSYMBOL_CMD_PLAYBACK = 75,              /* CMD_PLAYBACK  */
  YYSYMBOL_CMD_CHAR_DISPLAY = 76,          /* CMD_CHAR_DISPLAY  */
  YYSYMBOL_CMD_SPRITE_DISPLAY = 77,        /* CMD_SPRITE_DISPLAY  */
  YYSYMBOL_CMD_TEXT_DISPLAY = 78,          /* CMD_TEXT_DISPLAY  */
  YYSYMBOL_CMD_SCREENCODE_DISPLAY = 79,    /* CMD_SCREENCODE_DISPLAY  */
  YYSYMBOL_CMD_ENTER_DATA = 80,            /* CMD_ENTER_DATA  */
  YYSYMBOL_CMD_ENTER_BIN_DATA = 81,        /* CMD_ENTER_BIN_DATA  */
  YYSYMBOL_CMD_KEYBUF = 82,                /* CMD_KEYBUF  */
  YYSYMBOL_CMD_BLOAD = 83,                 /* CMD_BLOAD  */
  YYSYMBOL_CMD_BSAVE = 84,                 /* CMD_BSAVE  */
  YYSYMBOL_CMD_SCREEN = 85,                /* CMD_SCREEN  */
  YYSYMBOL_CMD_UNTIL = 86,                 /* CMD_UNTIL  */
  YYSYMBOL_CMD_CPU = 87,                   /* CMD_CPU  */
  YYSYMBOL_CMD_YYDEBUG = 88,               /* CMD_YYDEBUG  */
  YYSYMBOL_CMD_BACKTRACE = 89,             /* CMD_BACKTRACE  */
  YYSYMBOL_CMD_SCREENSHOT = 90,            /* CMD_SCREENSHOT  */
  YYSYMBOL_CMD_PWD = 91,                   /* CMD_PWD  */
  YYSYMBOL_CMD_DIR = 92,                   /* CMD_DIR  */
  YYSYMBOL_CMD_RESOURCE_GET = 93,          /* CMD_RESOURCE_GET  */
  YYSYMBOL_CMD_RESOURCE_SET = 94,          /* CMD_RESOURCE_SET  */
  YYSYMBOL_CMD_LOAD_RESOURCES = 95,        /* CMD_LOAD_RESOURCES  */
  YYSYMBOL_CMD_SAVE_RESOURCES = 96,        /* CMD_SAVE_RESOURCES  */
  YYSYMBOL_CMD_ATTACH = 97,                /* CMD_ATTACH  */
  YYSYMBOL_CMD_DETACH = 98,                /* CMD_DETACH  */
  YYSYMBOL_CMD_MON_RESET = 99,             /* CMD_MON_RESET  */
  YYSYMBOL_CMD_TAPECTRL = 100,             /* CMD_TAPECTRL  */
  YYSYMBOL_CMD_CARTFREEZE = 101,           /* CMD_CARTFREEZE  */
  YYSYMBOL_CMD_CPUHISTORY = 102,           /* CMD_CPUHISTORY  */
  YYSYMBOL_CMD_MEMMAPZAP = 103,            /* CMD_MEMMAPZAP  */
  YYSYMBOL_CMD_MEMMAPSHOW = 104,           /* CMD_MEMMAPSHOW  */
  YYSYMBOL_CMD_MEMMAPSAVE = 105,           /* CMD_MEMMAPSAVE  */
  YYSYMBOL_CMD_COMMENT = 106,              /* CMD_COMMENT  */
  YYSYMBOL_CMD_LIST = 107,                 /* CMD_LIST  */
  YYSYMBOL_CMD_STOPWATCH = 108,            /* CMD_STOPWATCH  */
  YYSYMBOL_RESET = 109,                    /* RESET  */
  YYSYMBOL_CMD_EXPORT = 110,               /* CMD_EXPORT  */
  YYSYMBOL_CMD_AUTOSTART = 111,            /* CMD_AUTOSTART  */
  YYSYMBOL_CMD_AUTOLOAD = 112,             /* CMD_AUTOLOAD  */
  YYSYMBOL_CMD_MAINCPU_TRACE = 113,        /* CMD_MAINCPU_TRACE  */
  YYSYMBOL_CMD_LABEL_ASGN = 114,           /* CMD_LABEL_ASGN  */
  YYSYMBOL_L_PAREN = 115,                  /* L_PAREN  */
  YYSYMBOL_R_PAREN = 116,                  /* R_PAREN  */
  YYSYMBOL_ARG_IMMEDIATE = 117,            /* ARG_IMMEDIATE  */
  YYSYMBOL_REG_A = 118,                    /* REG_A  */
  YYSYMBOL_REG_X = 119,                    /* REG_X  */
  YYSYMBOL_REG_Y = 120,                    /* REG_Y  */
  YYSYMBOL_COMMA = 121,                    /* COMMA  */
  YYSYMBOL_INST_SEP = 122,                 /* INST_SEP  */
  YYSYMBOL_L_BRACKET = 123,                /* L_BRACKET  */
  YYSYMBOL_R_BRACKET = 124,                /* R_BRACKET  */
  YYSYMBOL_LESS_THAN = 125,                /* LESS_THAN  */
  YYSYMBOL_REG_U = 126,                    /* REG_U  */
  YYSYMBOL_REG_S = 127,                    /* REG_S  */
  YYSYMBOL_REG_PC = 128,                   /* REG_PC  */
  YYSYMBOL_REG_PCR = 129,                  /* REG_PCR  */
  YYSYMBOL_REG_B = 130,                    /* REG_B  */
  YYSYMBOL_REG_C = 131,                    /* REG_C  */
  YYSYMBOL_REG_D = 132,                    /* REG_D  */
  YYSYMBOL_REG_E = 133,                    /* REG_E  */
  YYSYMBOL_REG_H = 134,                    /* REG_H  */
  YYSYMBOL_REG_L = 135,                    /* REG_L  */
  YYSYMBOL_REG_AF = 136,                   /* REG_AF  */
  YYSYMBOL_REG_BC = 137,                   /* REG_BC  */
  YYSYMBOL_REG_DE = 138,                   /* REG_DE  */
  YYSYMBOL_REG_HL = 139,                   /* REG_HL  */
  YYSYMBOL_REG_IX = 140,                   /* REG_IX  */
  YYSYMBOL_REG_IY = 141,                   /* REG_IY  */
  YYSYMBOL_REG_SP = 142,                   /* REG_SP  */
  YYSYMBOL_REG_IXH = 143,                  /* REG_IXH  */
  YYSYMBOL_REG_IXL = 144,                  /* REG_IXL  */
  YYSYMBOL_REG_IYH = 145,                  /* REG_IYH  */
  YYSYMBOL_REG_IYL = 146,                  /* REG_IYL  */
  YYSYMBOL_PLUS = 147,                     /* PLUS  */
  YYSYMBOL_MINUS = 148,                    /* MINUS  */
  YYSYMBOL_STRING = 149,                   /* STRING  */
  YYSYMBOL_FILENAME = 150,                 /* FILENAME  */
  YYSYMBOL_R_O_L = 151,                    /* R_O_L  */
  YYSYMBOL_OPCODE = 152,                   /* OPCODE  */
  YYSYMBOL_LABEL = 153,                    /* LABEL  */
  YYSYMBOL_BANKNAME = 154,                 /* BANKNAME  */
  YYSYMBOL_CPUTYPE = 155,                  /* CPUTYPE  */
  YYSYMBOL_MON_REGISTER = 156,             /* MON_REGISTER  */
  YYSYMBOL_COMPARE_OP = 157,               /* COMPARE_OP  */
  YYSYMBOL_RADIX_TYPE = 158,               /* RADIX_TYPE  */
  YYSYMBOL_INPUT_SPEC = 159,               /* INPUT_SPEC  */
  YYSYMBOL_CMD_CHECKPT_ON = 160,           /* CMD_CHECKPT_ON  */
  YYSYMBOL_CMD_CHECKPT_OFF = 161,          /* CMD_CHECKPT_OFF  */
  YYSYMBOL_TOGGLE = 162,                   /* TOGGLE  */
  YYSYMBOL_MASK = 163,                     /* MASK  */
  YYSYMBOL_164_ = 164,                     /* '+'  */
  YYSYMBOL_165_ = 165,                     /* '-'  */
  YYSYMBOL_166_ = 166,                     /* '*'  */
  YYSYMBOL_167_ = 167,                     /* '/'  */
  YYSYMBOL_168_ = 168,                     /* '('  */
  YYSYMBOL_169_ = 169,                     /* ')'  */
  YYSYMBOL_170_ = 170,                     /* '@'  */
  YYSYMBOL_171_ = 171,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 172,                 /* $accept  */
  YYSYMBOL_top_level = 173,                /* top_level  */
  YYSYMBOL_command_list = 174,             /* command_list  */
  YYSYMBOL_end_cmd = 175,                  /* end_cmd  */
  YYSYMBOL_command = 176,                  /* command  */
  YYSYMBOL_machine_state_rules = 177,      /* machine_state_rules  */
  YYSYMBOL_register_mod = 178,             /* register_mod  */
  YYSYMBOL_symbol_table_rules = 179,       /* symbol_table_rules  */
  YYSYMBOL_asm_rules = 180,                /* asm_rules  */
  YYSYMBOL_181_1 = 181,                    /* $@1  */
  YYSYMBOL_memory_rules = 182,             /* memory_rules  */
  YYSYMBOL_checkpoint_rules = 183,         /* checkpoint_rules  */
  YYSYMBOL_checkpoint_control_rules = 184, /* checkpoint_control_rules  */
  YYSYMBOL_monitor_state_rules = 185,      /* monitor_state_rules  */
  YYSYMBOL_monitor_misc_rules = 186,       /* monitor_misc_rules  */
  YYSYMBOL_disk_rules = 187,               /* disk_rules  */
  YYSYMBOL_cmd_file_rules = 188,           /* cmd_file_rules  */
  YYSYMBOL_data_entry_rules = 189,         /* data_entry_rules  */
  YYSYMBOL_monitor_debug_rules = 190,      /* monitor_debug_rules  */
  YYSYMBOL_rest_of_line = 191,             /* rest_of_line  */
  YYSYMBOL_opt_rest_of_line = 192,         /* opt_rest_of_line  */
  YYSYMBOL_filename = 193,                 /* filename  */
  YYSYMBOL_device_num = 194,               /* device_num  */
  YYSYMBOL_mem_op = 195,                   /* mem_op  */
  YYSYMBOL_opt_mem_op = 196,               /* opt_mem_op  */
  YYSYMBOL_register = 197,                 /* register  */
  YYSYMBOL_reg_list = 198,                 /* reg_list  */
  YYSYMBOL_reg_asgn = 199,                 /* reg_asgn  */
  YYSYMBOL_checkpt_num = 200,              /* checkpt_num  */
  YYSYMBOL_address_opt_range = 201,        /* address_opt_range  */
  YYSYMBOL_address_range = 202,            /* address_range  */
  YYSYMBOL_opt_address = 203,              /* opt_address  */
  YYSYMBOL_address = 204,                  /* address  */
  YYSYMBOL_opt_sep = 205,                  /* opt_sep  */
  YYSYMBOL_memspace = 206,                 /* memspace  */
  YYSYMBOL_memloc = 207,                   /* memloc  */
  YYSYMBOL_memaddr = 208,                  /* memaddr  */
  YYSYMBOL_expression = 209,               /* expression  */
  YYSYMBOL_opt_if_cond_expr = 210,         /* opt_if_cond_expr  */
  YYSYMBOL_cond_expr = 211,                /* cond_expr  */
  YYSYMBOL_compare_operand = 212,          /* compare_operand  */
  YYSYMBOL_data_list = 213,                /* data_list  */
  YYSYMBOL_data_element = 214,             /* data_element  */
  YYSYMBOL_hunt_list = 215,                /* hunt_list  */
  YYSYMBOL_hunt_element = 216,             /* hunt_element  */
  YYSYMBOL_value = 217,                    /* value  */
  YYSYMBOL_d_number = 218,                 /* d_number  */
  YYSYMBOL_guess_default = 219,            /* guess_default  */
  YYSYMBOL_number = 220,                   /* number  */
  YYSYMBOL_assembly_instr_list = 221,      /* assembly_instr_list  */
  YYSYMBOL_assembly_instruction = 222,     /* assembly_instruction  */
  YYSYMBOL_post_assemble = 223,            /* post_assemble  */
  YYSYMBOL_asm_operand_mode = 224,         /* asm_operand_mode  */
  YYSYMBOL_index_reg = 225,                /* index_reg  */
  YYSYMBOL_index_ureg = 226                /* index_ureg  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  312
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1713

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  172
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  314
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  628

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   418


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     168,   169,   166,   164,     2,   165,     2,   167,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   171,     2,
       2,     2,     2,     2,   170,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   199,   199,   200,   201,   204,   205,   208,   209,   210,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   228,   230,   232,   234,   236,   238,   240,
     242,   244,   246,   248,   250,   252,   254,   256,   258,   260,
     262,   264,   266,   268,   270,   272,   274,   276,   279,   281,
     283,   286,   291,   296,   298,   300,   302,   304,   306,   308,
     310,   312,   314,   318,   325,   324,   327,   329,   331,   335,
     337,   339,   341,   343,   345,   347,   349,   351,   353,   355,
     357,   359,   361,   363,   365,   367,   369,   371,   373,   377,
     386,   389,   393,   396,   405,   408,   417,   422,   424,   426,
     428,   430,   432,   434,   436,   438,   440,   442,   446,   448,
     453,   455,   473,   475,   477,   479,   481,   485,   487,   489,
     491,   493,   495,   497,   499,   501,   503,   505,   507,   509,
     511,   513,   515,   517,   519,   521,   523,   525,   527,   529,
     533,   535,   537,   539,   541,   543,   545,   547,   549,   551,
     553,   555,   557,   559,   561,   563,   565,   567,   569,   573,
     575,   577,   581,   583,   587,   591,   594,   595,   598,   599,
     602,   603,   606,   607,   610,   611,   614,   620,   628,   629,
     632,   636,   637,   640,   641,   644,   645,   647,   651,   652,
     655,   660,   665,   675,   676,   679,   680,   681,   682,   683,
     686,   688,   690,   691,   692,   693,   694,   695,   696,   699,
     700,   702,   707,   709,   711,   713,   717,   723,   729,   737,
     738,   741,   742,   745,   746,   749,   750,   751,   754,   755,
     758,   759,   760,   761,   764,   765,   766,   769,   770,   771,
     772,   773,   776,   777,   778,   781,   791,   792,   795,   802,
     813,   824,   832,   851,   857,   865,   873,   875,   877,   878,
     879,   880,   881,   882,   883,   885,   887,   889,   891,   892,
     893,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   911,   912,   927,
     931,   935,   939,   943,   947,   951,   955,   959,   971,   986,
     990,   994,   998,  1002,  1006,  1010,  1014,  1018,  1030,  1035,
    1043,  1044,  1045,  1046,  1050
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "H_NUMBER", "D_NUMBER",
  "O_NUMBER", "B_NUMBER", "CONVERT_OP", "B_DATA", "H_RANGE_GUESS",
  "D_NUMBER_GUESS", "O_NUMBER_GUESS", "B_NUMBER_GUESS", "BAD_CMD",
  "MEM_OP", "IF", "MEM_COMP", "MEM_DISK8", "MEM_DISK9", "MEM_DISK10",
  "MEM_DISK11", "EQUALS", "TRAIL", "CMD_SEP", "LABEL_ASGN_COMMENT",
  "CMD_SIDEFX", "CMD_RETURN", "CMD_BLOCK_READ", "CMD_BLOCK_WRITE",
  "CMD_UP", "CMD_DOWN", "CMD_LOAD", "CMD_SAVE", "CMD_VERIFY", "CMD_IGNORE",
  "CMD_HUNT", "CMD_FILL", "CMD_MOVE", "CMD_GOTO", "CMD_REGISTERS",
  "CMD_READSPACE", "CMD_WRITESPACE", "CMD_RADIX", "CMD_MEM_DISPLAY",
  "CMD_BREAK", "CMD_TRACE", "CMD_IO", "CMD_BRMON", "CMD_COMPARE",
  "CMD_DUMP", "CMD_UNDUMP", "CMD_EXIT", "CMD_DELETE", "CMD_CONDITION",
  "CMD_COMMAND", "CMD_ASSEMBLE", "CMD_DISASSEMBLE", "CMD_NEXT", "CMD_STEP",
  "CMD_PRINT", "CMD_DEVICE", "CMD_HELP", "CMD_WATCH", "CMD_DISK",
  "CMD_QUIT", "CMD_CHDIR", "CMD_BANK", "CMD_LOAD_LABELS",
  "CMD_SAVE_LABELS", "CMD_ADD_LABEL", "CMD_DEL_LABEL", "CMD_SHOW_LABELS",
  "CMD_CLEAR_LABELS", "CMD_RECORD", "CMD_MON_STOP", "CMD_PLAYBACK",
  "CMD_CHAR_DISPLAY", "CMD_SPRITE_DISPLAY", "CMD_TEXT_DISPLAY",
  "CMD_SCREENCODE_DISPLAY", "CMD_ENTER_DATA", "CMD_ENTER_BIN_DATA",
  "CMD_KEYBUF", "CMD_BLOAD", "CMD_BSAVE", "CMD_SCREEN", "CMD_UNTIL",
  "CMD_CPU", "CMD_YYDEBUG", "CMD_BACKTRACE", "CMD_SCREENSHOT", "CMD_PWD",
  "CMD_DIR", "CMD_RESOURCE_GET", "CMD_RESOURCE_SET", "CMD_LOAD_RESOURCES",
  "CMD_SAVE_RESOURCES", "CMD_ATTACH", "CMD_DETACH", "CMD_MON_RESET",
  "CMD_TAPECTRL", "CMD_CARTFREEZE", "CMD_CPUHISTORY", "CMD_MEMMAPZAP",
  "CMD_MEMMAPSHOW", "CMD_MEMMAPSAVE", "CMD_COMMENT", "CMD_LIST",
  "CMD_STOPWATCH", "RESET", "CMD_EXPORT", "CMD_AUTOSTART", "CMD_AUTOLOAD",
  "CMD_MAINCPU_TRACE", "CMD_LABEL_ASGN", "L_PAREN", "R_PAREN",
  "ARG_IMMEDIATE", "REG_A", "REG_X", "REG_Y", "COMMA", "INST_SEP",
  "L_BRACKET", "R_BRACKET", "LESS_THAN", "REG_U", "REG_S", "REG_PC",
  "REG_PCR", "REG_B", "REG_C", "REG_D", "REG_E", "REG_H", "REG_L",
  "REG_AF", "REG_BC", "REG_DE", "REG_HL", "REG_IX", "REG_IY", "REG_SP",
  "REG_IXH", "REG_IXL", "REG_IYH", "REG_IYL", "PLUS", "MINUS", "STRING",
  "FILENAME", "R_O_L", "OPCODE", "LABEL", "BANKNAME", "CPUTYPE",
  "MON_REGISTER", "COMPARE_OP", "RADIX_TYPE", "INPUT_SPEC",
  "CMD_CHECKPT_ON", "CMD_CHECKPT_OFF", "TOGGLE", "MASK", "'+'", "'-'",
  "'*'", "'/'", "'('", "')'", "'@'", "':'", "$accept", "top_level",
  "command_list", "end_cmd", "command", "machine_state_rules",
  "register_mod", "symbol_table_rules", "asm_rules", "$@1", "memory_rules",
  "checkpoint_rules", "checkpoint_control_rules", "monitor_state_rules",
  "monitor_misc_rules", "disk_rules", "cmd_file_rules", "data_entry_rules",
  "monitor_debug_rules", "rest_of_line", "opt_rest_of_line", "filename",
  "device_num", "mem_op", "opt_mem_op", "register", "reg_list", "reg_asgn",
  "checkpt_num", "address_opt_range", "address_range", "opt_address",
  "address", "opt_sep", "memspace", "memloc", "memaddr", "expression",
  "opt_if_cond_expr", "cond_expr", "compare_operand", "data_list",
  "data_element", "hunt_list", "hunt_element", "value", "d_number",
  "guess_default", "number", "assembly_instr_list", "assembly_instruction",
  "post_assemble", "asm_operand_mode", "index_reg", "index_ureg", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-398)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-195)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1117,   890,  -398,  -398,   103,   365,   890,   890,    89,    89,
      49,    49,    49,   574,  1473,  1473,  1473,  1115,   409,    60,
     983,  1231,  1231,  1115,  1473,    49,    49,   365,   742,   574,
     574,  1491,  1369,    89,    89,   890,   696,   169,  1231,  -132,
     365,  -132,   515,   189,   189,  1491,   426,  1511,  1511,    49,
     365,    49,  1369,  1369,  1369,  1369,  1491,   365,  -132,    49,
      49,   365,  1369,   137,   365,   365,    49,   365,  -126,  -122,
     -78,    49,    49,    49,   890,    89,   -70,   365,    89,   365,
      89,    49,  -126,   503,   324,   365,    49,    49,   -73,    70,
    1536,   742,   742,   121,  1254,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,   107,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,   890,  -398,   -21,   241,  -398,  -398,  -398,
    -398,  -398,  -398,   365,  -398,  -398,   927,   927,  -398,  -398,
     890,  -398,   890,  -398,  -398,   669,   691,   669,  -398,  -398,
    -398,  -398,  -398,    89,  -398,  -398,  -398,   -70,   -70,   -70,
    -398,  -398,  -398,   -70,   -70,  -398,   365,   -70,  -398,   122,
     233,  -398,    95,   365,  -398,   -70,  -398,   365,  -398,   340,
    -398,  -398,   138,  1473,  -398,  1473,  -398,   365,   -70,   365,
     365,  -398,   472,  -398,   365,   152,   109,   171,  -398,   365,
    -398,   890,  -398,   890,   241,   365,  -398,  -398,   365,  -398,
    1473,   365,  -398,   365,   365,  -398,    65,   365,   -70,   365,
     -70,   -70,   365,   -70,  -398,   365,  -398,   365,   365,  -398,
     365,  -398,   365,  -398,   365,  -398,   365,  -398,   365,  1097,
    -398,   365,   669,   669,  -398,  -398,   365,   365,  -398,  -398,
    -398,    89,  -398,  -398,   365,   365,    31,   365,   365,   890,
     241,  -398,   890,   890,  -398,  -398,   890,  -398,  -398,   890,
     -70,   365,   476,  -398,   365,   255,   365,  -398,  -398,  1592,
    1592,   365,  1491,   830,  1546,    61,   264,  1566,  1546,    66,
    -398,    74,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,    76,  -398,  -398,   365,
    -398,   365,  -398,  -398,  -398,    12,  -398,   890,   890,   890,
     890,  -398,  -398,   162,   946,   241,   241,  -398,   277,  1392,
    1415,  1455,  -398,   890,   144,  1491,  1253,  1097,  1491,  -398,
    1546,  1546,   230,  -398,  -398,  -398,  1473,  -398,  -398,   186,
     186,  -398,  1491,  -398,  -398,  -398,   716,   365,    54,  -398,
      62,  -398,   241,   241,  -398,  -398,  -398,   186,  -398,  -398,
    -398,  -398,    72,  -398,    49,  -398,    49,    71,  -398,    79,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  1513,
    -398,  -398,  -398,   277,  1435,  -398,  -398,  -398,   890,  -398,
    -398,   365,  -398,  -398,   241,  -398,   241,   241,   241,   857,
     890,  -398,  -398,  -398,  -398,  1546,  -398,  1546,  -398,   468,
      99,   113,   117,   120,   128,   143,    -1,  -398,   442,  -398,
    -398,  -398,  -398,   332,    91,  -398,   133,   362,   140,   141,
      -5,  -398,   442,   442,  1580,  -398,  -398,  -398,  -398,    -9,
      -9,  -398,  -398,   365,  1491,   365,  -398,  -398,   365,  -398,
     365,  -398,   365,   241,  -398,  -398,    53,  -398,  -398,  -398,
    -398,  -398,  1513,   365,  -398,  -398,   365,   716,   365,   365,
     365,   716,   119,  -398,    80,  -398,  -398,  -398,   365,   149,
     157,   365,  -398,  -398,   365,   365,   365,   365,   365,   365,
    -398,  1097,   365,  -398,   365,   241,  -398,  -398,  -398,  -398,
    -398,  -398,   365,   241,   365,   365,   365,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,   153,  -107,  -398,   442,  -398,   134,
     442,   422,   -98,   442,   442,   534,   173,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,   155,  -398,  -398,
    -398,     7,   127,   285,  -398,  -398,    62,    62,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,   238,   190,   193,  -398,  -398,   194,
     442,   195,  -398,   -69,   198,   212,   213,   216,   222,  -398,
    -398,  1491,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,   192,  -398,   221,  -398,  -398,   224,  -398,  -398,
    -398,  -398,  -398,  -398,   231,  -398,  -398,  -398
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,     0,    22,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,     0,     0,
     268,     0,     0,     0,     2,     5,    10,    47,    11,    13,
      12,    14,    15,    16,    17,    18,    19,    20,    21,     0,
     237,   238,   239,   240,   236,   235,   234,   195,   196,   197,
     198,   199,   176,     0,   229,     0,     0,   208,   241,   228,
       9,     8,     7,     0,   109,    35,     0,     0,   193,    42,
       0,    44,     0,   169,   168,     0,     0,     0,   182,   230,
     233,   232,   231,     0,   181,   186,   192,   194,   194,   194,
     190,   200,   201,   194,   194,    28,     0,   194,    48,     0,
       0,   179,     0,     0,   111,   194,    75,     0,   183,   194,
     173,    90,   174,     0,    96,     0,    29,     0,   194,     0,
       0,   115,     9,   104,     0,     0,     0,     0,    68,     0,
      40,     0,    38,     0,     0,     0,   165,   119,     0,    94,
       0,     0,   114,     0,     0,    23,     0,     0,   194,     0,
     194,   194,     0,   194,    59,     0,    61,     0,     0,   160,
       0,    77,     0,    79,     0,    81,     0,    83,     0,     0,
     163,     0,     0,     0,    46,    92,     0,     0,    31,   164,
     124,     0,   126,   166,     0,     0,     0,     0,     0,     0,
       0,   133,     0,     0,   136,    33,     0,    84,    85,     0,
     194,     0,     9,   151,     0,   170,     0,   139,   113,     0,
       0,     0,     0,     0,     0,   269,     0,     0,     0,   270,
     271,   272,   273,   274,   277,   280,   281,   282,   283,   284,
     285,   286,   275,   278,   276,   279,   249,   245,    98,     0,
     100,     0,     1,     6,     3,     0,   177,     0,     0,     0,
       0,   121,   108,   194,     0,     0,     0,   171,   194,   143,
       0,     0,   101,     0,     0,     0,     0,     0,     0,    27,
       0,     0,     0,    50,    49,   110,     0,    74,   172,   210,
     210,    30,     0,    36,    37,   103,     0,     0,     0,    66,
       0,    67,     0,     0,   118,   112,   120,   210,   117,   122,
      25,    24,     0,    52,     0,    54,     0,     0,    56,     0,
      58,    60,   159,   161,    76,    78,    80,    82,   222,     0,
     220,   221,   123,   194,     0,    91,    32,   127,     0,   125,
     129,     0,   131,   132,     0,   154,     0,     0,     0,     0,
       0,   137,   152,   138,   155,     0,   157,     0,   116,     0,
       0,     0,     0,     0,     0,     0,     0,   248,     0,   310,
     311,   314,   313,     0,   293,   312,     0,     0,     0,     0,
       0,   287,     0,     0,     0,    97,    99,   207,   206,   202,
     203,   204,   205,     0,     0,     0,    43,    45,     0,   144,
       0,   148,     0,     0,   227,   226,     0,   224,   225,   185,
     187,   191,     0,     0,   180,   178,     0,     0,     0,     0,
       0,     0,     0,   216,     0,   215,   217,   107,     0,   247,
     246,     0,    41,    39,     0,     0,     0,     0,     0,     0,
     162,     0,     0,   146,     0,     0,   130,   153,   134,   135,
      34,    86,     0,     0,     0,     0,     0,    62,   258,   259,
     260,   261,   262,   263,   254,     0,   295,     0,   291,   289,
       0,     0,     0,     0,     0,     0,   308,   294,   296,   250,
     251,   252,   297,   253,   288,   149,   188,   150,   140,   142,
     147,   102,    72,   223,    71,    69,    73,   209,    89,    95,
      70,     0,     0,     0,   105,   106,     0,   244,    65,    93,
      26,    51,    53,    55,    57,   219,   141,   145,   128,    87,
      88,   156,   158,    63,     0,     0,     0,   292,   290,     0,
       0,     0,   303,     0,     0,     0,     0,     0,     0,   214,
     213,     0,   212,   211,   242,   243,   264,   257,   265,   266,
     267,   255,     0,   305,     0,   301,   299,     0,   304,   306,
     307,   298,   309,   218,     0,   302,   300,   256
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -398,  -398,  -398,   572,   220,  -398,  -398,  -398,  -398,  -398,
    -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,  -398,    83,
     268,   102,  -110,  -398,   -15,    -7,  -398,    10,   374,    15,
       0,  -307,    29,    64,   -14,  -206,  -398,   234,  -288,  -397,
    -398,    28,  -133,  -398,   -91,  -398,  -398,  -398,   130,  -398,
    -355,  -398,  -398,   485,   -62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    93,    94,   134,    95,    96,    97,    98,    99,   360,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   208,
     254,   145,   274,   182,   183,   124,   170,   171,   153,   177,
     178,   453,   179,   454,   125,   160,   161,   275,   478,   484,
     485,   389,   390,   466,   467,   127,   154,   128,   162,   489,
     109,   491,   307,   434,   435
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     159,   159,   159,   167,   172,   490,   159,   185,   599,   167,
     159,   169,   585,   447,   157,   163,   164,   167,   159,   206,
     586,   458,   205,   210,   188,   253,   592,   255,   216,   218,
     220,   167,   223,   225,   227,   328,   330,   331,   159,   159,
     159,   159,   167,   158,   158,   158,   166,   199,   159,   593,
     143,   138,   187,   158,   130,   616,   110,   111,   112,   113,
     197,   130,   479,   114,   115,   116,   130,   232,   234,   236,
     238,   256,   140,   142,   221,   131,   132,   246,   617,   494,
     557,   130,   131,   132,   561,   239,   502,   131,   132,   281,
     130,   282,  -194,  -194,  -194,  -194,   130,   201,   203,  -194,
    -194,  -194,   131,   132,   130,  -194,  -194,  -194,  -194,  -194,
     357,   131,   132,   146,   147,   524,   535,   131,   132,   536,
     525,   312,   211,   600,   213,   131,   132,   189,   190,   314,
     471,   129,   393,   394,   471,   316,   129,   129,   130,   262,
     263,   241,   266,   341,   269,   217,   219,   110,   111,   112,
     113,   228,   348,   230,   114,   115,   116,   319,   320,   131,
     132,   242,   243,  -189,   563,   129,   603,   356,   251,   159,
     130,   159,   130,   257,   258,   259,   317,   318,   319,   320,
     401,   448,   428,   270,  -189,  -189,   138,   442,   279,   280,
     143,   131,   132,   131,   132,   443,   159,   444,   349,   144,
     350,   477,   464,   488,   129,   117,   118,   119,   120,   121,
     138,   604,   605,   129,    90,   518,   465,   333,   173,  -194,
     306,   334,   335,   336,   498,   367,   495,   337,   338,   519,
     138,   340,   499,   520,   130,   126,   521,   563,   529,   346,
     136,   137,   130,   335,   522,  -194,   117,   118,   119,   120,
     121,   316,   352,   129,   530,   131,   132,  -194,  -194,   523,
     358,   533,   534,   131,   132,   133,   129,   129,   167,   204,
     129,   566,   129,   562,   584,   129,   129,   129,  -189,   567,
     372,   588,   374,   138,   376,   377,   602,   379,   110,   111,
     112,   113,   247,   464,   598,   114,   115,   116,   601,  -189,
    -189,   117,   118,   119,   120,   121,   611,   465,   260,   612,
     167,   419,   563,   624,   313,   398,   159,   167,   613,   615,
     206,   167,   618,   -64,   167,   130,   317,   318,   319,   320,
     460,   129,   159,   129,   410,   169,   619,   620,   167,   144,
     621,  -184,   622,   415,   417,   625,   131,   132,   626,   483,
     271,   627,   475,   455,   342,  -184,   606,   315,   607,   158,
     462,   476,  -184,  -184,   469,   472,   130,   473,   575,   391,
     323,   324,   129,   129,   325,   553,   326,   608,   609,   610,
     159,   480,   544,   429,   430,     0,   122,   131,   132,   129,
     431,   432,   129,   129,   504,   159,   129,     0,   138,   129,
     481,     0,   194,   195,   196,   317,   318,   319,   320,     0,
     130,     0,   433,   426,   427,     0,     0,   440,   441,   317,
     318,   319,   320,   158,   512,   117,   118,   119,   120,   121,
       0,   131,   132,   276,     0,   362,     0,   363,     0,     0,
     167,   122,   117,   118,   119,   120,   121,   129,   129,   129,
     129,   429,   430,   501,     0,   482,     0,     0,   431,   432,
       0,   138,     0,   129,   468,   309,   311,   391,     0,   130,
     483,   474,     0,  -182,   483,     0,   496,  -171,   497,     0,
     527,   429,   430,   546,     0,     0,   486,     0,   431,   432,
     131,   132,   516,   404,  -182,  -182,   406,   407,  -171,  -171,
     408,     0,     0,   409,   272,     0,   110,   111,   112,   113,
     531,     0,     0,   114,   115,   116,   130,     0,     0,   117,
     118,   119,   120,   121,     0,   131,   132,     0,   129,     0,
       0,   117,   118,   119,   120,   121,   501,   131,   132,     0,
     129,   429,   430,     0,     0,   514,     0,   515,   431,   432,
       0,   449,   450,   451,   452,     0,   483,     0,     0,     0,
       0,   429,   430,     0,     0,   122,     0,   463,   431,   432,
     590,     0,     0,     0,   543,   148,     0,   135,   149,   222,
     139,   141,     0,     0,   150,   151,   152,   167,     0,   165,
     168,   174,   176,   181,   184,   186,   468,     0,     0,   191,
     193,     0,     0,     0,   198,   200,   202,   486,     0,   207,
     209,   486,   212,     0,   215,     0,     0,     0,     0,   224,
     226,     0,   229,     0,   231,   233,   235,   237,     0,   240,
     623,   391,   505,   244,   245,   248,   249,   250,     0,   252,
       0,     0,     0,     0,   513,     0,     0,   261,     0,   264,
     265,   267,   268,   429,   430,   273,   277,   278,     0,   122,
     431,   432,   596,   308,   310,     0,     0,     0,     0,   214,
     327,   123,   110,   111,   112,   113,     0,     0,     0,   114,
     115,   116,     0,     0,     0,   117,   118,   119,   120,   121,
       0,     0,   329,   486,   110,   111,   112,   113,   321,     0,
       0,   114,   115,   116,     0,   322,     0,   117,   118,   119,
     120,   121,   117,   118,   119,   120,   121,     0,     0,   110,
     111,   112,   113,     0,     0,   332,   114,   115,   116,     0,
       0,     0,   117,   118,   119,   120,   121,     0,   339,     0,
       0,     0,   343,   192,   344,   345,   149,     0,     0,   347,
       0,     0,   150,   151,   152,     0,     0,     0,     0,   351,
       0,   353,   354,     0,   131,   132,   355,     0,     0,   359,
       0,   361,     0,     0,     0,     0,   364,   365,     0,     0,
     366,     0,     0,   368,     0,   369,   370,     0,   371,   373,
       0,   375,     0,     0,   378,     0,     0,   380,     0,   381,
     382,     0,   383,     0,   384,     0,   385,     0,   386,     0,
     387,     0,     0,   392,     0,     0,     0,     0,   395,   396,
       0,     0,     0,   397,     0,   122,   399,   400,     0,   402,
     403,   481,   405,   110,   111,   112,   113,   123,     0,     0,
     114,   115,   116,   411,     0,     0,   412,   122,   413,     0,
       0,   414,   416,   418,     0,     0,     0,     0,   130,   123,
     110,   111,   112,   113,     0,     0,   155,   114,   115,   116,
       0,     0,   122,   117,   118,   119,   120,   121,     0,   131,
     132,   445,     0,   446,     0,     0,   482,     0,     0,     0,
       0,     0,     0,   110,   111,   112,   113,   456,   457,     0,
     114,   115,   116,     0,     0,     0,   117,   118,   119,   120,
     121,     0,     0,   526,     0,     0,     0,     0,   528,     0,
       0,     0,   532,     0,     0,     0,     0,   537,   538,   487,
     110,   111,   112,   113,   492,   493,     0,   114,   115,   116,
       0,     0,     0,   117,   118,   119,   120,   121,     0,   110,
     111,   112,   113,     0,     0,     0,   114,   115,   116,     0,
       0,   500,   117,   118,   119,   120,   121,   420,   421,   422,
     423,   424,   425,   506,     0,     0,   507,     0,   508,   509,
     510,   511,     0,     0,   130,     0,   110,   111,   112,   113,
       0,   517,   155,   114,   115,   116,     0,     0,     0,   117,
     118,   119,   120,   121,     0,   131,   132,     0,     0,     0,
     156,     0,   587,     0,     0,   589,   591,     0,   594,   595,
     597,   317,   318,   319,   320,   545,     0,   547,     0,     0,
     548,     0,   549,     0,   550,   551,     0,     0,   552,     0,
       0,     0,     0,     0,   554,   555,   122,     0,   556,     0,
     558,   559,   560,     0,     0,     0,   564,     0,   123,     0,
     565,     0,     0,   568,     0,     0,   569,   570,   571,   572,
     573,   574,     0,     0,   576,   614,   577,   578,     0,     0,
       0,     0,     0,   122,   579,   580,   581,   582,   583,     0,
       0,   317,   318,   319,   320,   123,     0,     0,     0,   156,
     110,   111,   112,   113,     0,     0,     0,   114,   115,   116,
     317,   318,   319,   320,     0,     0,   130,     0,   110,   111,
     112,   113,     0,     0,     1,   114,   115,   116,     0,     0,
       2,   117,   118,   119,   120,   121,   156,   131,   132,     3,
       0,   175,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,     0,     0,    19,
      20,    21,    22,    23,     0,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,     0,    85,    86,    87,
      88,    89,   130,     0,  -175,  -175,  -175,  -175,     0,     0,
    -175,  -175,  -175,  -175,     0,   180,   388,  -175,  -175,  -175,
    -175,  -175,     0,   131,   132,     0,   110,   111,   112,   113,
       0,     1,   470,   114,   115,   116,     0,     2,   156,    90,
       0,     0,     0,     0,     0,     0,     0,    91,    92,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,     0,     0,    19,    20,    21,    22,
      23,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,     0,    85,    86,    87,    88,    89,     0,
     130,     0,   110,   111,   112,   113,     0,     0,   155,   114,
     115,   116,     0,     0,  -175,   117,   118,   119,   120,   121,
       0,   131,   132,  -171,     0,  -171,  -171,  -171,  -171,     0,
       0,  -171,  -171,  -171,  -171,     0,     0,     0,  -171,  -171,
    -171,  -171,  -171,     0,    91,    92,   459,     0,   110,   111,
     112,   113,     0,     0,   155,   114,   115,   116,     0,     0,
       0,   117,   118,   119,   120,   121,   503,     0,   110,   111,
     112,   113,     0,     0,   155,   114,   115,   116,     0,     0,
       0,   117,   118,   119,   120,   121,   461,     0,   110,   111,
     112,   113,     0,     0,     0,   114,   115,   116,     0,     0,
       0,   117,   118,   119,   120,   121,   110,   111,   112,   113,
       0,     0,   155,   114,   115,   116,     0,     0,     0,   117,
     118,   119,   120,   121,   110,   111,   112,   113,     0,     0,
       0,   114,   115,   116,     0,     0,     0,   117,   118,   119,
     120,   121,   130,     0,   130,     0,  -194,  -194,  -194,  -194,
       0,     0,   156,  -194,  -194,  -194,     0,   117,   118,   119,
     120,   121,     0,   131,   132,   131,   132,     0,     0,   110,
     111,   112,   113,     0,     0,  -171,   114,   115,   116,   110,
     111,   112,   113,     0,     0,     0,   114,   115,   116,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,   110,
     111,   112,   113,     0,     0,     0,   114,   115,   116,     0,
       0,     0,     0,   110,   111,   112,   113,     0,   156,     0,
     114,   115,   116,   130,     0,  -194,  -194,  -194,  -194,     0,
       0,     0,  -194,  -194,  -194,     0,     0,     0,   156,     0,
       0,     0,     0,     0,   131,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   156,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
       0,   283,     0,   284,   285,     0,     0,   286,     0,   287,
       0,   288,  -194,     0,     0,     0,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,     0,   436,     0,     0,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,     0,   439,   539,
     540,     0,     0,     0,     0,     0,   431,   541,   542,     0,
       0,     0,     0,   138
};

static const yytype_int16 yycheck[] =
{
      14,    15,    16,    17,    18,   360,    20,    22,     1,    23,
      24,    18,   119,     1,    14,    15,    16,    31,    32,   151,
     127,   328,    36,    38,    24,   151,   124,   149,    42,    43,
      44,    45,    46,    47,    48,   145,   146,   147,    52,    53,
      54,    55,    56,    14,    15,    16,    17,    32,    62,   147,
       1,   121,    23,    24,     1,   124,     3,     4,     5,     6,
      31,     1,   350,    10,    11,    12,     1,    52,    53,    54,
      55,   149,     8,     9,    45,    22,    23,    62,   147,   367,
     477,     1,    22,    23,   481,    56,   393,    22,    23,   162,
       1,    21,     3,     4,     5,     6,     1,    33,    34,    10,
      11,    12,    22,    23,     1,    16,    17,    18,    19,    20,
       1,    22,    23,    11,    12,   116,   121,    22,    23,   124,
     121,     0,    39,   116,    41,    22,    23,    25,    26,    22,
     336,     1,   242,   243,   340,   156,     6,     7,     1,    75,
      76,    58,    78,    21,    80,    43,    44,     3,     4,     5,
       6,    49,    14,    51,    10,    11,    12,   166,   167,    22,
      23,    59,    60,     1,   157,    35,   563,    15,    66,   183,
       1,   185,     1,    71,    72,    73,   164,   165,   166,   167,
     149,   169,   121,    81,    22,    23,   121,   121,    86,    87,
       1,    22,    23,    22,    23,   121,   210,   121,   183,   150,
     185,    15,   149,   149,    74,    16,    17,    18,    19,    20,
     121,   566,   567,    83,   152,   116,   163,   153,   158,   154,
      90,   157,   158,   159,   153,   210,   154,   163,   164,   116,
     121,   167,   153,   116,     1,     1,   116,   157,   147,   175,
       6,     7,     1,   179,   116,   156,    16,    17,    18,    19,
      20,   156,   188,   123,   121,    22,    23,   168,   149,   116,
     196,   121,   121,    22,    23,   162,   136,   137,   282,    35,
     140,   122,   142,   154,   121,   145,   146,   147,     1,   122,
     216,   147,   218,   121,   220,   221,     1,   223,     3,     4,
       5,     6,   155,   149,   121,    10,    11,    12,   171,    22,
      23,    16,    17,    18,    19,    20,   116,   163,    74,   116,
     324,   282,   157,   121,    94,   251,   330,   331,   124,   124,
     151,   335,   124,   152,   338,     1,   164,   165,   166,   167,
     330,   201,   346,   203,   270,   342,   124,   124,   352,   150,
     124,     1,   120,   279,   280,   124,    22,    23,   124,   356,
      82,   120,   342,   324,   121,    15,   118,   123,   120,   330,
     331,   346,    22,    23,   335,   337,     1,   338,   501,   239,
     136,   137,   242,   243,   140,   466,   142,   139,   140,   141,
     394,   352,   444,   119,   120,    -1,   156,    22,    23,   259,
     126,   127,   262,   263,   394,   409,   266,    -1,   121,   269,
     115,    -1,    28,    29,    30,   164,   165,   166,   167,    -1,
       1,    -1,   148,   283,   284,    -1,    -1,   287,   288,   164,
     165,   166,   167,   394,   409,    16,    17,    18,    19,    20,
      -1,    22,    23,   109,    -1,   201,    -1,   203,    -1,    -1,
     454,   156,    16,    17,    18,    19,    20,   317,   318,   319,
     320,   119,   120,   389,    -1,   170,    -1,    -1,   126,   127,
      -1,   121,    -1,   333,   334,    91,    92,   337,    -1,     1,
     477,   341,    -1,     1,   481,    -1,   374,     1,   376,    -1,
     148,   119,   120,   454,    -1,    -1,   356,    -1,   126,   127,
      22,    23,    24,   259,    22,    23,   262,   263,    22,    23,
     266,    -1,    -1,   269,     1,    -1,     3,     4,     5,     6,
     148,    -1,    -1,    10,    11,    12,     1,    -1,    -1,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,   398,    -1,
      -1,    16,    17,    18,    19,    20,   472,    22,    23,    -1,
     410,   119,   120,    -1,    -1,   415,    -1,   417,   126,   127,
      -1,   317,   318,   319,   320,    -1,   563,    -1,    -1,    -1,
      -1,   119,   120,    -1,    -1,   156,    -1,   333,   126,   127,
     148,    -1,    -1,    -1,   444,     1,    -1,     5,     4,   153,
       8,     9,    -1,    -1,    10,    11,    12,   601,    -1,    17,
      18,    19,    20,    21,    22,    23,   466,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    33,    34,   477,    -1,    37,
      38,   481,    40,    -1,    42,    -1,    -1,    -1,    -1,    47,
      48,    -1,    50,    -1,    52,    53,    54,    55,    -1,    57,
     601,   501,   398,    61,    62,    63,    64,    65,    -1,    67,
      -1,    -1,    -1,    -1,   410,    -1,    -1,    75,    -1,    77,
      78,    79,    80,   119,   120,    83,    84,    85,    -1,   156,
     126,   127,   128,    91,    92,    -1,    -1,    -1,    -1,   154,
       1,   168,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      -1,    -1,     1,   563,     3,     4,     5,     6,   126,    -1,
      -1,    10,    11,    12,    -1,   133,    -1,    16,    17,    18,
      19,    20,    16,    17,    18,    19,    20,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,   153,    10,    11,    12,    -1,
      -1,    -1,    16,    17,    18,    19,    20,    -1,   166,    -1,
      -1,    -1,   170,     1,   172,   173,     4,    -1,    -1,   177,
      -1,    -1,    10,    11,    12,    -1,    -1,    -1,    -1,   187,
      -1,   189,   190,    -1,    22,    23,   194,    -1,    -1,   197,
      -1,   199,    -1,    -1,    -1,    -1,   204,   205,    -1,    -1,
     208,    -1,    -1,   211,    -1,   213,   214,    -1,   216,   217,
      -1,   219,    -1,    -1,   222,    -1,    -1,   225,    -1,   227,
     228,    -1,   230,    -1,   232,    -1,   234,    -1,   236,    -1,
     238,    -1,    -1,   241,    -1,    -1,    -1,    -1,   246,   247,
      -1,    -1,    -1,   251,    -1,   156,   254,   255,    -1,   257,
     258,   115,   260,     3,     4,     5,     6,   168,    -1,    -1,
      10,    11,    12,   271,    -1,    -1,   274,   156,   276,    -1,
      -1,   279,   280,   281,    -1,    -1,    -1,    -1,     1,   168,
       3,     4,     5,     6,    -1,    -1,     9,    10,    11,    12,
      -1,    -1,   156,    16,    17,    18,    19,    20,    -1,    22,
      23,   309,    -1,   311,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,   325,   326,    -1,
      10,    11,    12,    -1,    -1,    -1,    16,    17,    18,    19,
      20,    -1,    -1,   428,    -1,    -1,    -1,    -1,   433,    -1,
      -1,    -1,   437,    -1,    -1,    -1,    -1,   442,   443,   357,
       3,     4,     5,     6,   362,   363,    -1,    10,    11,    12,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    -1,
      -1,   389,    16,    17,    18,    19,    20,   137,   138,   139,
     140,   141,   142,   401,    -1,    -1,   404,    -1,   406,   407,
     408,   409,    -1,    -1,     1,    -1,     3,     4,     5,     6,
      -1,   419,     9,    10,    11,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    -1,    -1,
     153,    -1,   527,    -1,    -1,   530,   531,    -1,   533,   534,
     535,   164,   165,   166,   167,   453,    -1,   455,    -1,    -1,
     458,    -1,   460,    -1,   462,   463,    -1,    -1,   466,    -1,
      -1,    -1,    -1,    -1,   472,   473,   156,    -1,   476,    -1,
     478,   479,   480,    -1,    -1,    -1,   484,    -1,   168,    -1,
     488,    -1,    -1,   491,    -1,    -1,   494,   495,   496,   497,
     498,   499,    -1,    -1,   502,   590,   504,   505,    -1,    -1,
      -1,    -1,    -1,   156,   512,   513,   514,   515,   516,    -1,
      -1,   164,   165,   166,   167,   168,    -1,    -1,    -1,   153,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
     164,   165,   166,   167,    -1,    -1,     1,    -1,     3,     4,
       5,     6,    -1,    -1,     7,    10,    11,    12,    -1,    -1,
      13,    16,    17,    18,    19,    20,   153,    22,    23,    22,
      -1,   158,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   114,     1,    -1,     3,     4,     5,     6,    -1,    -1,
       9,    10,    11,    12,    -1,    14,   149,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,     3,     4,     5,     6,
      -1,     7,     9,    10,    11,    12,    -1,    13,   153,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    -1,   110,   111,   112,   113,   114,    -1,
       1,    -1,     3,     4,     5,     6,    -1,    -1,     9,    10,
      11,    12,    -1,    -1,   153,    16,    17,    18,    19,    20,
      -1,    22,    23,     1,    -1,     3,     4,     5,     6,    -1,
      -1,     9,    10,    11,    12,    -1,    -1,    -1,    16,    17,
      18,    19,    20,    -1,   160,   161,     1,    -1,     3,     4,
       5,     6,    -1,    -1,     9,    10,    11,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,     1,    -1,     3,     4,
       5,     6,    -1,    -1,     9,    10,    11,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,     1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,     3,     4,     5,     6,
      -1,    -1,     9,    10,    11,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    -1,    -1,    -1,    16,    17,    18,
      19,    20,     1,    -1,     1,    -1,     3,     4,     5,     6,
      -1,    -1,   153,    10,    11,    12,    -1,    16,    17,    18,
      19,    20,    -1,    22,    23,    22,    23,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,   153,    10,    11,    12,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,   153,    -1,
      10,    11,    12,     1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    -1,    -1,    -1,   153,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,   117,   118,    -1,    -1,   121,    -1,   123,
      -1,   125,   149,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   119,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,
      -1,    -1,    -1,   121
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    13,    22,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    42,
      43,    44,    45,    46,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   110,   111,   112,   113,   114,
     152,   160,   161,   173,   174,   176,   177,   178,   179,   180,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   222,
       3,     4,     5,     6,    10,    11,    12,    16,    17,    18,
      19,    20,   156,   168,   197,   206,   209,   217,   219,   220,
       1,    22,    23,   162,   175,   175,   209,   209,   121,   175,
     205,   175,   205,     1,   150,   193,   193,   193,     1,     4,
      10,    11,    12,   200,   218,     9,   153,   202,   204,   206,
     207,   208,   220,   202,   202,   175,   204,   206,   175,   197,
     198,   199,   206,   158,   175,   158,   175,   201,   202,   204,
      14,   175,   195,   196,   175,   196,   175,   204,   202,   193,
     193,   175,     1,   175,   200,   200,   200,   204,   175,   201,
     175,   205,   175,   205,   209,   206,   151,   175,   191,   175,
     196,   191,   175,   191,   154,   175,   206,   193,   206,   193,
     206,   204,   153,   206,   175,   206,   175,   206,   193,   175,
     193,   175,   201,   175,   201,   175,   201,   175,   201,   204,
     175,   191,   193,   193,   175,   175,   201,   155,   175,   175,
     175,   193,   175,   151,   192,   149,   149,   193,   193,   193,
     209,   175,   205,   205,   175,   175,   205,   175,   175,   205,
     193,   192,     1,   175,   194,   209,   109,   175,   175,   193,
     193,   162,    21,   115,   117,   118,   121,   123,   125,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   220,   224,   175,   200,
     175,   200,     0,   176,    22,   209,   156,   164,   165,   166,
     167,   175,   175,   209,   209,   209,   209,     1,   194,     1,
     194,   194,   175,   205,   205,   205,   205,   205,   205,   175,
     205,    21,   121,   175,   175,   175,   205,   175,    14,   201,
     201,   175,   205,   175,   175,   175,    15,     1,   205,   175,
     181,   175,   209,   209,   175,   175,   175,   201,   175,   175,
     175,   175,   205,   175,   205,   175,   205,   205,   175,   205,
     175,   175,   175,   175,   175,   175,   175,   175,   149,   213,
     214,   220,   175,   194,   194,   175,   175,   175,   205,   175,
     175,   149,   175,   175,   209,   175,   209,   209,   209,   209,
     205,   175,   175,   175,   175,   205,   175,   205,   175,   204,
     137,   138,   139,   140,   141,   142,   220,   220,   121,   119,
     120,   126,   127,   148,   225,   226,   118,   121,   130,   132,
     220,   220,   121,   121,   121,   175,   175,     1,   169,   209,
     209,   209,   209,   203,   205,   204,   175,   175,   203,     1,
     202,     1,   204,   209,   149,   163,   215,   216,   220,   204,
       9,   207,   213,   204,   220,   199,   201,    15,   210,   210,
     204,   115,   170,   197,   211,   212,   220,   175,   149,   221,
     222,   223,   175,   175,   210,   154,   193,   193,   153,   153,
     175,   205,   203,     1,   202,   209,   175,   175,   175,   175,
     175,   175,   201,   209,   220,   220,    24,   175,   116,   116,
     116,   116,   116,   116,   116,   121,   225,   148,   225,   147,
     121,   148,   225,   121,   121,   121,   124,   225,   225,   119,
     120,   127,   128,   220,   226,   175,   204,   175,   175,   175,
     175,   175,   175,   216,   175,   175,   175,   211,   175,   175,
     175,   211,   154,   157,   175,   175,   122,   122,   175,   175,
     175,   175,   175,   175,   175,   214,   175,   175,   175,   175,
     175,   175,   175,   175,   121,   119,   127,   225,   147,   225,
     148,   225,   124,   147,   225,   225,   128,   225,   121,     1,
     116,   171,     1,   211,   222,   222,   118,   120,   139,   140,
     141,   116,   116,   124,   225,   124,   124,   147,   124,   124,
     124,   124,   120,   204,   121,   124,   124,   120
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   172,   173,   173,   173,   174,   174,   175,   175,   175,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   178,   178,
     178,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   181,   180,   180,   180,   180,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   183,
     183,   183,   183,   183,   183,   183,   183,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   189,   189,   190,   191,   192,   192,   193,   193,
     194,   194,   195,   195,   196,   196,   197,   197,   198,   198,
     199,   200,   200,   201,   201,   202,   202,   202,   203,   203,
     204,   204,   204,   205,   205,   206,   206,   206,   206,   206,
     207,   208,   209,   209,   209,   209,   209,   209,   209,   210,
     210,   211,   211,   211,   211,   211,   212,   212,   212,   213,
     213,   214,   214,   215,   215,   216,   216,   216,   217,   217,
     218,   218,   218,   218,   219,   219,   219,   220,   220,   220,
     220,   220,   221,   221,   221,   222,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     225,   225,   225,   225,   226
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     3,     5,     3,     2,     2,
       3,     2,     3,     2,     4,     2,     3,     3,     2,     4,
       2,     4,     2,     4,     2,     4,     2,     1,     2,     3,
       3,     5,     3,     5,     3,     5,     3,     5,     3,     2,
       3,     2,     4,     5,     0,     5,     3,     3,     2,     5,
       5,     5,     5,     5,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     2,     2,     4,     5,     5,     5,
       2,     3,     2,     5,     2,     5,     2,     3,     2,     3,
       2,     3,     5,     3,     2,     5,     5,     4,     3,     2,
       3,     2,     3,     2,     2,     2,     3,     3,     3,     2,
       3,     3,     3,     3,     2,     3,     2,     3,     5,     3,
       4,     3,     3,     2,     4,     4,     2,     3,     3,     2,
       5,     5,     5,     3,     4,     5,     4,     5,     4,     5,
       5,     2,     3,     4,     3,     3,     5,     3,     5,     3,
       2,     3,     4,     2,     2,     1,     1,     0,     1,     1,
       1,     1,     2,     1,     1,     0,     1,     2,     3,     1,
       3,     1,     1,     1,     1,     3,     1,     3,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     2,
       0,     3,     3,     3,     3,     1,     1,     1,     4,     3,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     2,     1,     1,     2,     1,
       3,     3,     3,     3,     3,     5,     7,     5,     3,     3,
       3,     3,     3,     3,     5,     5,     5,     5,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     3,
       4,     3,     4,     2,     3,     3,     3,     3,     5,     5,
       6,     5,     6,     4,     5,     5,     5,     5,     3,     5,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* top_level: command_list  */
#line 199 "mon_parse.y"
                        { (yyval.i) = 0; }
#line 2480 "mon_parse.c"
    break;

  case 3: /* top_level: assembly_instruction TRAIL  */
#line 200 "mon_parse.y"
                                      { (yyval.i) = 0; }
#line 2486 "mon_parse.c"
    break;

  case 4: /* top_level: TRAIL  */
#line 201 "mon_parse.y"
                 { new_cmd = 1; asm_mode = 0;  (yyval.i) = 0; }
#line 2492 "mon_parse.c"
    break;

  case 9: /* end_cmd: error  */
#line 210 "mon_parse.y"
               { return ERR_EXPECT_END_CMD; }
#line 2498 "mon_parse.c"
    break;

  case 22: /* command: BAD_CMD  */
#line 225 "mon_parse.y"
                 { return ERR_BAD_CMD; }
#line 2504 "mon_parse.c"
    break;

  case 23: /* machine_state_rules: CMD_BANK end_cmd  */
#line 229 "mon_parse.y"
                     { mon_bank(e_default_space, NULL); }
#line 2510 "mon_parse.c"
    break;

  case 24: /* machine_state_rules: CMD_BANK memspace end_cmd  */
#line 231 "mon_parse.y"
                     { mon_bank((yyvsp[-1].i), NULL); }
#line 2516 "mon_parse.c"
    break;

  case 25: /* machine_state_rules: CMD_BANK BANKNAME end_cmd  */
#line 233 "mon_parse.y"
                     { mon_bank(e_default_space, (yyvsp[-1].str)); }
#line 2522 "mon_parse.c"
    break;

  case 26: /* machine_state_rules: CMD_BANK memspace opt_sep BANKNAME end_cmd  */
#line 235 "mon_parse.y"
                     { mon_bank((yyvsp[-3].i), (yyvsp[-1].str)); }
#line 2528 "mon_parse.c"
    break;

  case 27: /* machine_state_rules: CMD_GOTO address end_cmd  */
#line 237 "mon_parse.y"
                     { mon_jump((yyvsp[-1].a)); }
#line 2534 "mon_parse.c"
    break;

  case 28: /* machine_state_rules: CMD_GOTO end_cmd  */
#line 239 "mon_parse.y"
                     { mon_go(); }
#line 2540 "mon_parse.c"
    break;

  case 29: /* machine_state_rules: CMD_IO end_cmd  */
#line 241 "mon_parse.y"
                     { mon_display_io_regs(0); }
#line 2546 "mon_parse.c"
    break;

  case 30: /* machine_state_rules: CMD_IO address end_cmd  */
#line 243 "mon_parse.y"
                     { mon_display_io_regs((yyvsp[-1].a)); }
#line 2552 "mon_parse.c"
    break;

  case 31: /* machine_state_rules: CMD_CPU end_cmd  */
#line 245 "mon_parse.y"
                     { monitor_cpu_type_set(""); }
#line 2558 "mon_parse.c"
    break;

  case 32: /* machine_state_rules: CMD_CPU CPUTYPE end_cmd  */
#line 247 "mon_parse.y"
                     { monitor_cpu_type_set((yyvsp[-1].str)); }
#line 2564 "mon_parse.c"
    break;

  case 33: /* machine_state_rules: CMD_CPUHISTORY end_cmd  */
#line 249 "mon_parse.y"
                     { mon_cpuhistory(-1); }
#line 2570 "mon_parse.c"
    break;

  case 34: /* machine_state_rules: CMD_CPUHISTORY opt_sep expression end_cmd  */
#line 251 "mon_parse.y"
                     { mon_cpuhistory((yyvsp[-1].i)); }
#line 2576 "mon_parse.c"
    break;

  case 35: /* machine_state_rules: CMD_RETURN end_cmd  */
#line 253 "mon_parse.y"
                     { mon_instruction_return(); }
#line 2582 "mon_parse.c"
    break;

  case 36: /* machine_state_rules: CMD_DUMP filename end_cmd  */
#line 255 "mon_parse.y"
                     { machine_write_snapshot((yyvsp[-1].str),0,0,0); /* FIXME */ }
#line 2588 "mon_parse.c"
    break;

  case 37: /* machine_state_rules: CMD_UNDUMP filename end_cmd  */
#line 257 "mon_parse.y"
                     { machine_read_snapshot((yyvsp[-1].str), 0); }
#line 2594 "mon_parse.c"
    break;

  case 38: /* machine_state_rules: CMD_STEP end_cmd  */
#line 259 "mon_parse.y"
                     { mon_instructions_step(-1); }
#line 2600 "mon_parse.c"
    break;

  case 39: /* machine_state_rules: CMD_STEP opt_sep expression end_cmd  */
#line 261 "mon_parse.y"
                     { mon_instructions_step((yyvsp[-1].i)); }
#line 2606 "mon_parse.c"
    break;

  case 40: /* machine_state_rules: CMD_NEXT end_cmd  */
#line 263 "mon_parse.y"
                     { mon_instructions_next(-1); }
#line 2612 "mon_parse.c"
    break;

  case 41: /* machine_state_rules: CMD_NEXT opt_sep expression end_cmd  */
#line 265 "mon_parse.y"
                     { mon_instructions_next((yyvsp[-1].i)); }
#line 2618 "mon_parse.c"
    break;

  case 42: /* machine_state_rules: CMD_UP end_cmd  */
#line 267 "mon_parse.y"
                     { mon_stack_up(-1); }
#line 2624 "mon_parse.c"
    break;

  case 43: /* machine_state_rules: CMD_UP opt_sep expression end_cmd  */
#line 269 "mon_parse.y"
                     { mon_stack_up((yyvsp[-1].i)); }
#line 2630 "mon_parse.c"
    break;

  case 44: /* machine_state_rules: CMD_DOWN end_cmd  */
#line 271 "mon_parse.y"
                     { mon_stack_down(-1); }
#line 2636 "mon_parse.c"
    break;

  case 45: /* machine_state_rules: CMD_DOWN opt_sep expression end_cmd  */
#line 273 "mon_parse.y"
                     { mon_stack_down((yyvsp[-1].i)); }
#line 2642 "mon_parse.c"
    break;

  case 46: /* machine_state_rules: CMD_SCREEN end_cmd  */
#line 275 "mon_parse.y"
                     { mon_display_screen(); }
#line 2648 "mon_parse.c"
    break;

  case 48: /* register_mod: CMD_REGISTERS end_cmd  */
#line 280 "mon_parse.y"
              { (monitor_cpu_for_memspace[default_memspace]->mon_register_print)(default_memspace); }
#line 2654 "mon_parse.c"
    break;

  case 49: /* register_mod: CMD_REGISTERS memspace end_cmd  */
#line 282 "mon_parse.y"
              { (monitor_cpu_for_memspace[(yyvsp[-1].i)]->mon_register_print)((yyvsp[-1].i)); }
#line 2660 "mon_parse.c"
    break;

  case 51: /* symbol_table_rules: CMD_LOAD_LABELS memspace opt_sep filename end_cmd  */
#line 287 "mon_parse.y"
                    {
                        /* What about the memspace? */
                        mon_playback_init((yyvsp[-1].str));
                    }
#line 2669 "mon_parse.c"
    break;

  case 52: /* symbol_table_rules: CMD_LOAD_LABELS filename end_cmd  */
#line 292 "mon_parse.y"
                    {
                        /* What about the memspace? */
                        mon_playback_init((yyvsp[-1].str));
                    }
#line 2678 "mon_parse.c"
    break;

  case 53: /* symbol_table_rules: CMD_SAVE_LABELS memspace opt_sep filename end_cmd  */
#line 297 "mon_parse.y"
                    { mon_save_symbols((yyvsp[-3].i), (yyvsp[-1].str)); }
#line 2684 "mon_parse.c"
    break;

  case 54: /* symbol_table_rules: CMD_SAVE_LABELS filename end_cmd  */
#line 299 "mon_parse.y"
                    { mon_save_symbols(e_default_space, (yyvsp[-1].str)); }
#line 2690 "mon_parse.c"
    break;

  case 55: /* symbol_table_rules: CMD_ADD_LABEL address opt_sep LABEL end_cmd  */
#line 301 "mon_parse.y"
                    { mon_add_name_to_symbol_table((yyvsp[-3].a), (yyvsp[-1].str)); }
#line 2696 "mon_parse.c"
    break;

  case 56: /* symbol_table_rules: CMD_DEL_LABEL LABEL end_cmd  */
#line 303 "mon_parse.y"
                    { mon_remove_name_from_symbol_table(e_default_space, (yyvsp[-1].str)); }
#line 2702 "mon_parse.c"
    break;

  case 57: /* symbol_table_rules: CMD_DEL_LABEL memspace opt_sep LABEL end_cmd  */
#line 305 "mon_parse.y"
                    { mon_remove_name_from_symbol_table((yyvsp[-3].i), (yyvsp[-1].str)); }
#line 2708 "mon_parse.c"
    break;

  case 58: /* symbol_table_rules: CMD_SHOW_LABELS memspace end_cmd  */
#line 307 "mon_parse.y"
                    { mon_print_symbol_table((yyvsp[-1].i)); }
#line 2714 "mon_parse.c"
    break;

  case 59: /* symbol_table_rules: CMD_SHOW_LABELS end_cmd  */
#line 309 "mon_parse.y"
                    { mon_print_symbol_table(e_default_space); }
#line 2720 "mon_parse.c"
    break;

  case 60: /* symbol_table_rules: CMD_CLEAR_LABELS memspace end_cmd  */
#line 311 "mon_parse.y"
                    { mon_clear_symbol_table((yyvsp[-1].i)); }
#line 2726 "mon_parse.c"
    break;

  case 61: /* symbol_table_rules: CMD_CLEAR_LABELS end_cmd  */
#line 313 "mon_parse.y"
                    { mon_clear_symbol_table(e_default_space); }
#line 2732 "mon_parse.c"
    break;

  case 62: /* symbol_table_rules: CMD_LABEL_ASGN EQUALS address end_cmd  */
#line 315 "mon_parse.y"
                    {
                        mon_add_name_to_symbol_table((yyvsp[-1].a), mon_prepend_dot_to_name((yyvsp[-3].str)));
                    }
#line 2740 "mon_parse.c"
    break;

  case 63: /* symbol_table_rules: CMD_LABEL_ASGN EQUALS address LABEL_ASGN_COMMENT end_cmd  */
#line 319 "mon_parse.y"
                    {
                        mon_add_name_to_symbol_table((yyvsp[-2].a), mon_prepend_dot_to_name((yyvsp[-4].str)));
                    }
#line 2748 "mon_parse.c"
    break;

  case 64: /* $@1: %empty  */
#line 325 "mon_parse.y"
           { mon_start_assemble_mode((yyvsp[0].a), NULL); }
#line 2754 "mon_parse.c"
    break;

  case 65: /* asm_rules: CMD_ASSEMBLE address $@1 post_assemble end_cmd  */
#line 326 "mon_parse.y"
           { }
#line 2760 "mon_parse.c"
    break;

  case 66: /* asm_rules: CMD_ASSEMBLE address end_cmd  */
#line 328 "mon_parse.y"
           { mon_start_assemble_mode((yyvsp[-1].a), NULL); }
#line 2766 "mon_parse.c"
    break;

  case 67: /* asm_rules: CMD_DISASSEMBLE address_opt_range end_cmd  */
#line 330 "mon_parse.y"
           { mon_disassemble_lines((yyvsp[-1].range)[0], (yyvsp[-1].range)[1]); }
#line 2772 "mon_parse.c"
    break;

  case 68: /* asm_rules: CMD_DISASSEMBLE end_cmd  */
#line 332 "mon_parse.y"
           { mon_disassemble_lines(BAD_ADDR, BAD_ADDR); }
#line 2778 "mon_parse.c"
    break;

  case 69: /* memory_rules: CMD_MOVE address_range opt_sep address end_cmd  */
#line 336 "mon_parse.y"
              { mon_memory_move((yyvsp[-3].range)[0], (yyvsp[-3].range)[1], (yyvsp[-1].a)); }
#line 2784 "mon_parse.c"
    break;

  case 70: /* memory_rules: CMD_COMPARE address_range opt_sep address end_cmd  */
#line 338 "mon_parse.y"
              { mon_memory_compare((yyvsp[-3].range)[0], (yyvsp[-3].range)[1], (yyvsp[-1].a)); }
#line 2790 "mon_parse.c"
    break;

  case 71: /* memory_rules: CMD_FILL address_range opt_sep data_list end_cmd  */
#line 340 "mon_parse.y"
              { mon_memory_fill((yyvsp[-3].range)[0], (yyvsp[-3].range)[1],(unsigned char *)(yyvsp[-1].str)); }
#line 2796 "mon_parse.c"
    break;

  case 72: /* memory_rules: CMD_HUNT address_range opt_sep hunt_list end_cmd  */
#line 342 "mon_parse.y"
              { mon_memory_hunt((yyvsp[-3].range)[0], (yyvsp[-3].range)[1],(unsigned char *)(yyvsp[-1].str)); }
#line 2802 "mon_parse.c"
    break;

  case 73: /* memory_rules: CMD_MEM_DISPLAY RADIX_TYPE opt_sep address_opt_range end_cmd  */
#line 344 "mon_parse.y"
              { mon_memory_display((yyvsp[-3].rt), (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], DF_PETSCII); }
#line 2808 "mon_parse.c"
    break;

  case 74: /* memory_rules: CMD_MEM_DISPLAY address_opt_range end_cmd  */
#line 346 "mon_parse.y"
              { mon_memory_display(default_radix, (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], DF_PETSCII); }
#line 2814 "mon_parse.c"
    break;

  case 75: /* memory_rules: CMD_MEM_DISPLAY end_cmd  */
#line 348 "mon_parse.y"
              { mon_memory_display(default_radix, BAD_ADDR, BAD_ADDR, DF_PETSCII); }
#line 2820 "mon_parse.c"
    break;

  case 76: /* memory_rules: CMD_CHAR_DISPLAY address_opt_range end_cmd  */
#line 350 "mon_parse.y"
              { mon_memory_display_data((yyvsp[-1].range)[0], (yyvsp[-1].range)[1], 8, 8); }
#line 2826 "mon_parse.c"
    break;

  case 77: /* memory_rules: CMD_CHAR_DISPLAY end_cmd  */
#line 352 "mon_parse.y"
              { mon_memory_display_data(BAD_ADDR, BAD_ADDR, 8, 8); }
#line 2832 "mon_parse.c"
    break;

  case 78: /* memory_rules: CMD_SPRITE_DISPLAY address_opt_range end_cmd  */
#line 354 "mon_parse.y"
              { mon_memory_display_data((yyvsp[-1].range)[0], (yyvsp[-1].range)[1], 24, 21); }
#line 2838 "mon_parse.c"
    break;

  case 79: /* memory_rules: CMD_SPRITE_DISPLAY end_cmd  */
#line 356 "mon_parse.y"
              { mon_memory_display_data(BAD_ADDR, BAD_ADDR, 24, 21); }
#line 2844 "mon_parse.c"
    break;

  case 80: /* memory_rules: CMD_TEXT_DISPLAY address_opt_range end_cmd  */
#line 358 "mon_parse.y"
              { mon_memory_display(0, (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], DF_PETSCII); }
#line 2850 "mon_parse.c"
    break;

  case 81: /* memory_rules: CMD_TEXT_DISPLAY end_cmd  */
#line 360 "mon_parse.y"
              { mon_memory_display(0, BAD_ADDR, BAD_ADDR, DF_PETSCII); }
#line 2856 "mon_parse.c"
    break;

  case 82: /* memory_rules: CMD_SCREENCODE_DISPLAY address_opt_range end_cmd  */
#line 362 "mon_parse.y"
              { mon_memory_display(0, (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], DF_SCREEN_CODE); }
#line 2862 "mon_parse.c"
    break;

  case 83: /* memory_rules: CMD_SCREENCODE_DISPLAY end_cmd  */
#line 364 "mon_parse.y"
              { mon_memory_display(0, BAD_ADDR, BAD_ADDR, DF_SCREEN_CODE); }
#line 2868 "mon_parse.c"
    break;

  case 84: /* memory_rules: CMD_MEMMAPZAP end_cmd  */
#line 366 "mon_parse.y"
              { mon_memmap_zap(); }
#line 2874 "mon_parse.c"
    break;

  case 85: /* memory_rules: CMD_MEMMAPSHOW end_cmd  */
#line 368 "mon_parse.y"
              { mon_memmap_show(-1,BAD_ADDR,BAD_ADDR); }
#line 2880 "mon_parse.c"
    break;

  case 86: /* memory_rules: CMD_MEMMAPSHOW opt_sep expression end_cmd  */
#line 370 "mon_parse.y"
              { mon_memmap_show((yyvsp[-1].i),BAD_ADDR,BAD_ADDR); }
#line 2886 "mon_parse.c"
    break;

  case 87: /* memory_rules: CMD_MEMMAPSHOW opt_sep expression address_opt_range end_cmd  */
#line 372 "mon_parse.y"
              { mon_memmap_show((yyvsp[-2].i),(yyvsp[-1].range)[0],(yyvsp[-1].range)[1]); }
#line 2892 "mon_parse.c"
    break;

  case 88: /* memory_rules: CMD_MEMMAPSAVE filename opt_sep expression end_cmd  */
#line 374 "mon_parse.y"
              { mon_memmap_save((yyvsp[-3].str),(yyvsp[-1].i)); }
#line 2898 "mon_parse.c"
    break;

  case 89: /* checkpoint_rules: CMD_BREAK opt_mem_op address_opt_range opt_if_cond_expr end_cmd  */
#line 378 "mon_parse.y"
                  {
                      if ((yyvsp[-3].i)) {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], TRUE, (yyvsp[-3].i), FALSE);
                      } else {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], TRUE, e_exec, FALSE);
                      }
                      mon_breakpoint_set_checkpoint_condition(temp, (yyvsp[-1].cond_node));
                  }
#line 2911 "mon_parse.c"
    break;

  case 90: /* checkpoint_rules: CMD_BREAK end_cmd  */
#line 387 "mon_parse.y"
                  { mon_breakpoint_print_checkpoints(); }
#line 2917 "mon_parse.c"
    break;

  case 91: /* checkpoint_rules: CMD_UNTIL address_opt_range end_cmd  */
#line 390 "mon_parse.y"
                  {
                      mon_breakpoint_add_checkpoint((yyvsp[-1].range)[0], (yyvsp[-1].range)[1], TRUE, e_exec, TRUE);
                  }
#line 2925 "mon_parse.c"
    break;

  case 92: /* checkpoint_rules: CMD_UNTIL end_cmd  */
#line 394 "mon_parse.y"
                  { mon_breakpoint_print_checkpoints(); }
#line 2931 "mon_parse.c"
    break;

  case 93: /* checkpoint_rules: CMD_WATCH opt_mem_op address_opt_range opt_if_cond_expr end_cmd  */
#line 397 "mon_parse.y"
                  {
                      if ((yyvsp[-3].i)) {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], TRUE, (yyvsp[-3].i), FALSE);
                      } else {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], TRUE, e_load | e_store, FALSE);
                      }
                      mon_breakpoint_set_checkpoint_condition(temp, (yyvsp[-1].cond_node));
                  }
#line 2944 "mon_parse.c"
    break;

  case 94: /* checkpoint_rules: CMD_WATCH end_cmd  */
#line 406 "mon_parse.y"
                  { mon_breakpoint_print_checkpoints(); }
#line 2950 "mon_parse.c"
    break;

  case 95: /* checkpoint_rules: CMD_TRACE opt_mem_op address_opt_range opt_if_cond_expr end_cmd  */
#line 409 "mon_parse.y"
                  {
                      if ((yyvsp[-3].i)) {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], FALSE, (yyvsp[-3].i), FALSE);
                      } else {
                          temp = mon_breakpoint_add_checkpoint((yyvsp[-2].range)[0], (yyvsp[-2].range)[1], FALSE, e_load | e_store, FALSE);
                      }
                      mon_breakpoint_set_checkpoint_condition(temp, (yyvsp[-1].cond_node));
                  }
#line 2963 "mon_parse.c"
    break;

  case 96: /* checkpoint_rules: CMD_TRACE end_cmd  */
#line 418 "mon_parse.y"
                  { mon_breakpoint_print_checkpoints(); }
#line 2969 "mon_parse.c"
    break;

  case 97: /* checkpoint_control_rules: CMD_CHECKPT_ON checkpt_num end_cmd  */
#line 423 "mon_parse.y"
                          { mon_breakpoint_switch_checkpoint(e_ON, (yyvsp[-1].i)); }
#line 2975 "mon_parse.c"
    break;

  case 98: /* checkpoint_control_rules: CMD_CHECKPT_ON end_cmd  */
#line 425 "mon_parse.y"
                          { mon_breakpoint_switch_checkpoint(e_ON, -1); }
#line 2981 "mon_parse.c"
    break;

  case 99: /* checkpoint_control_rules: CMD_CHECKPT_OFF checkpt_num end_cmd  */
#line 427 "mon_parse.y"
                          { mon_breakpoint_switch_checkpoint(e_OFF, (yyvsp[-1].i)); }
#line 2987 "mon_parse.c"
    break;

  case 100: /* checkpoint_control_rules: CMD_CHECKPT_OFF end_cmd  */
#line 429 "mon_parse.y"
                          { mon_breakpoint_switch_checkpoint(e_OFF, -1); }
#line 2993 "mon_parse.c"
    break;

  case 101: /* checkpoint_control_rules: CMD_IGNORE checkpt_num end_cmd  */
#line 431 "mon_parse.y"
                          { mon_breakpoint_set_ignore_count((yyvsp[-1].i), -1); }
#line 2999 "mon_parse.c"
    break;

  case 102: /* checkpoint_control_rules: CMD_IGNORE checkpt_num opt_sep expression end_cmd  */
#line 433 "mon_parse.y"
                          { mon_breakpoint_set_ignore_count((yyvsp[-3].i), (yyvsp[-1].i)); }
#line 3005 "mon_parse.c"
    break;

  case 103: /* checkpoint_control_rules: CMD_DELETE checkpt_num end_cmd  */
#line 435 "mon_parse.y"
                          { mon_breakpoint_delete_checkpoint((yyvsp[-1].i)); }
#line 3011 "mon_parse.c"
    break;

  case 104: /* checkpoint_control_rules: CMD_DELETE end_cmd  */
#line 437 "mon_parse.y"
                          { mon_breakpoint_delete_checkpoint(-1); }
#line 3017 "mon_parse.c"
    break;

  case 105: /* checkpoint_control_rules: CMD_CONDITION checkpt_num IF cond_expr end_cmd  */
#line 439 "mon_parse.y"
                          { mon_breakpoint_set_checkpoint_condition((yyvsp[-3].i), (yyvsp[-1].cond_node)); }
#line 3023 "mon_parse.c"
    break;

  case 106: /* checkpoint_control_rules: CMD_COMMAND checkpt_num opt_sep STRING end_cmd  */
#line 441 "mon_parse.y"
                          { mon_breakpoint_set_checkpoint_command((yyvsp[-3].i), (yyvsp[-1].str)); }
#line 3029 "mon_parse.c"
    break;

  case 107: /* checkpoint_control_rules: CMD_COMMAND checkpt_num error end_cmd  */
#line 443 "mon_parse.y"
                          { return ERR_EXPECT_STRING; }
#line 3035 "mon_parse.c"
    break;

  case 108: /* monitor_state_rules: CMD_SIDEFX TOGGLE end_cmd  */
#line 447 "mon_parse.y"
                     { sidefx = (((yyvsp[-1].action) == e_TOGGLE) ? (sidefx ^ 1) : (yyvsp[-1].action)); }
#line 3041 "mon_parse.c"
    break;

  case 109: /* monitor_state_rules: CMD_SIDEFX end_cmd  */
#line 449 "mon_parse.y"
                     {
                         mon_out("I/O side effects are %s\n",
                                   sidefx ? "enabled" : "disabled");
                     }
#line 3050 "mon_parse.c"
    break;

  case 110: /* monitor_state_rules: CMD_RADIX RADIX_TYPE end_cmd  */
#line 454 "mon_parse.y"
                     { default_radix = (yyvsp[-1].rt); }
#line 3056 "mon_parse.c"
    break;

  case 111: /* monitor_state_rules: CMD_RADIX end_cmd  */
#line 456 "mon_parse.y"
                     {
                         const char *p;

                         if (default_radix == e_hexadecimal)
                             p = "Hexadecimal";
                         else if (default_radix == e_decimal)
                             p = "Decimal";
                         else if (default_radix == e_octal)
                             p = "Octal";
                         else if (default_radix == e_binary)
                             p = "Binary";
                         else
                             p = "Unknown";

                         mon_out("Default radix is %s\n", p);
                     }
#line 3077 "mon_parse.c"
    break;

  case 112: /* monitor_state_rules: CMD_DEVICE memspace end_cmd  */
#line 474 "mon_parse.y"
                     { monitor_change_device((yyvsp[-1].i)); }
#line 3083 "mon_parse.c"
    break;

  case 113: /* monitor_state_rules: CMD_EXPORT end_cmd  */
#line 476 "mon_parse.y"
                     { mon_export(); }
#line 3089 "mon_parse.c"
    break;

  case 114: /* monitor_state_rules: CMD_QUIT end_cmd  */
#line 478 "mon_parse.y"
                     { mon_quit(); YYACCEPT; }
#line 3095 "mon_parse.c"
    break;

  case 115: /* monitor_state_rules: CMD_EXIT end_cmd  */
#line 480 "mon_parse.y"
                     { mon_exit(); YYACCEPT; }
#line 3101 "mon_parse.c"
    break;

  case 116: /* monitor_state_rules: CMD_MAINCPU_TRACE TOGGLE end_cmd  */
#line 482 "mon_parse.y"
                     { mon_maincpu_toggle_trace((yyvsp[-1].action)); }
#line 3107 "mon_parse.c"
    break;

  case 117: /* monitor_misc_rules: CMD_DISK rest_of_line end_cmd  */
#line 486 "mon_parse.y"
                    { mon_drive_execute_disk_cmd((yyvsp[-1].str)); }
#line 3113 "mon_parse.c"
    break;

  case 118: /* monitor_misc_rules: CMD_PRINT expression end_cmd  */
#line 488 "mon_parse.y"
                    { mon_out("\t%d\n",(yyvsp[-1].i)); }
#line 3119 "mon_parse.c"
    break;

  case 119: /* monitor_misc_rules: CMD_HELP end_cmd  */
#line 490 "mon_parse.y"
                    { mon_command_print_help(NULL); }
#line 3125 "mon_parse.c"
    break;

  case 120: /* monitor_misc_rules: CMD_HELP rest_of_line end_cmd  */
#line 492 "mon_parse.y"
                    { mon_command_print_help((yyvsp[-1].str)); }
#line 3131 "mon_parse.c"
    break;

  case 121: /* monitor_misc_rules: CONVERT_OP expression end_cmd  */
#line 494 "mon_parse.y"
                    { mon_print_convert((yyvsp[-1].i)); }
#line 3137 "mon_parse.c"
    break;

  case 122: /* monitor_misc_rules: CMD_CHDIR rest_of_line end_cmd  */
#line 496 "mon_parse.y"
                    { mon_change_dir((yyvsp[-1].str)); }
#line 3143 "mon_parse.c"
    break;

  case 123: /* monitor_misc_rules: CMD_KEYBUF rest_of_line end_cmd  */
#line 498 "mon_parse.y"
                    { mon_keyboard_feed((yyvsp[-1].str)); }
#line 3149 "mon_parse.c"
    break;

  case 124: /* monitor_misc_rules: CMD_BACKTRACE end_cmd  */
#line 500 "mon_parse.y"
                    { mon_backtrace(); }
#line 3155 "mon_parse.c"
    break;

  case 125: /* monitor_misc_rules: CMD_DIR opt_rest_of_line end_cmd  */
#line 502 "mon_parse.y"
                     { mon_show_dir((yyvsp[-1].str)); }
#line 3161 "mon_parse.c"
    break;

  case 126: /* monitor_misc_rules: CMD_PWD end_cmd  */
#line 504 "mon_parse.y"
                     { mon_show_pwd(); }
#line 3167 "mon_parse.c"
    break;

  case 127: /* monitor_misc_rules: CMD_SCREENSHOT filename end_cmd  */
#line 506 "mon_parse.y"
                    { mon_screenshot_save((yyvsp[-1].str),-1); }
#line 3173 "mon_parse.c"
    break;

  case 128: /* monitor_misc_rules: CMD_SCREENSHOT filename opt_sep expression end_cmd  */
#line 508 "mon_parse.y"
                    { mon_screenshot_save((yyvsp[-3].str),(yyvsp[-1].i)); }
#line 3179 "mon_parse.c"
    break;

  case 129: /* monitor_misc_rules: CMD_RESOURCE_GET STRING end_cmd  */
#line 510 "mon_parse.y"
                    { mon_resource_get((yyvsp[-1].str)); }
#line 3185 "mon_parse.c"
    break;

  case 130: /* monitor_misc_rules: CMD_RESOURCE_SET STRING STRING end_cmd  */
#line 512 "mon_parse.y"
                    { mon_resource_set((yyvsp[-2].str),(yyvsp[-1].str)); }
#line 3191 "mon_parse.c"
    break;

  case 131: /* monitor_misc_rules: CMD_LOAD_RESOURCES filename end_cmd  */
#line 514 "mon_parse.y"
                    { resources_load((yyvsp[-1].str)); }
#line 3197 "mon_parse.c"
    break;

  case 132: /* monitor_misc_rules: CMD_SAVE_RESOURCES filename end_cmd  */
#line 516 "mon_parse.y"
                    { resources_save((yyvsp[-1].str)); }
#line 3203 "mon_parse.c"
    break;

  case 133: /* monitor_misc_rules: CMD_MON_RESET end_cmd  */
#line 518 "mon_parse.y"
                    { mon_reset_machine(-1); }
#line 3209 "mon_parse.c"
    break;

  case 134: /* monitor_misc_rules: CMD_MON_RESET opt_sep expression end_cmd  */
#line 520 "mon_parse.y"
                    { mon_reset_machine((yyvsp[-1].i)); }
#line 3215 "mon_parse.c"
    break;

  case 135: /* monitor_misc_rules: CMD_TAPECTRL opt_sep expression end_cmd  */
#line 522 "mon_parse.y"
                    { mon_tape_ctrl((yyvsp[-1].i)); }
#line 3221 "mon_parse.c"
    break;

  case 136: /* monitor_misc_rules: CMD_CARTFREEZE end_cmd  */
#line 524 "mon_parse.y"
                    { mon_cart_freeze(); }
#line 3227 "mon_parse.c"
    break;

  case 137: /* monitor_misc_rules: CMD_COMMENT opt_rest_of_line end_cmd  */
#line 526 "mon_parse.y"
                     { }
#line 3233 "mon_parse.c"
    break;

  case 138: /* monitor_misc_rules: CMD_STOPWATCH RESET end_cmd  */
#line 528 "mon_parse.y"
                     { mon_stopwatch_reset(); }
#line 3239 "mon_parse.c"
    break;

  case 139: /* monitor_misc_rules: CMD_STOPWATCH end_cmd  */
#line 530 "mon_parse.y"
                     { mon_stopwatch_show("Stopwatch: ", "\n"); }
#line 3245 "mon_parse.c"
    break;

  case 140: /* disk_rules: CMD_LOAD filename device_num opt_address end_cmd  */
#line 534 "mon_parse.y"
            { mon_file_load((yyvsp[-3].str), (yyvsp[-2].i), (yyvsp[-1].a), FALSE); }
#line 3251 "mon_parse.c"
    break;

  case 141: /* disk_rules: CMD_BLOAD filename device_num opt_address end_cmd  */
#line 536 "mon_parse.y"
            { mon_file_load((yyvsp[-3].str), (yyvsp[-2].i), (yyvsp[-1].a), TRUE); }
#line 3257 "mon_parse.c"
    break;

  case 142: /* disk_rules: CMD_SAVE filename device_num address_range end_cmd  */
#line 538 "mon_parse.y"
            { mon_file_save((yyvsp[-3].str), (yyvsp[-2].i), (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], FALSE); }
#line 3263 "mon_parse.c"
    break;

  case 143: /* disk_rules: CMD_SAVE filename error  */
#line 540 "mon_parse.y"
            { return ERR_EXPECT_DEVICE_NUM; }
#line 3269 "mon_parse.c"
    break;

  case 144: /* disk_rules: CMD_SAVE filename device_num error  */
#line 542 "mon_parse.y"
            { return ERR_EXPECT_ADDRESS; }
#line 3275 "mon_parse.c"
    break;

  case 145: /* disk_rules: CMD_BSAVE filename device_num address_range end_cmd  */
#line 544 "mon_parse.y"
            { mon_file_save((yyvsp[-3].str), (yyvsp[-2].i), (yyvsp[-1].range)[0], (yyvsp[-1].range)[1], TRUE); }
#line 3281 "mon_parse.c"
    break;

  case 146: /* disk_rules: CMD_BSAVE filename device_num error  */
#line 546 "mon_parse.y"
            { return ERR_EXPECT_ADDRESS; }
#line 3287 "mon_parse.c"
    break;

  case 147: /* disk_rules: CMD_VERIFY filename device_num address end_cmd  */
#line 548 "mon_parse.y"
            { mon_file_verify((yyvsp[-3].str),(yyvsp[-2].i),(yyvsp[-1].a)); }
#line 3293 "mon_parse.c"
    break;

  case 148: /* disk_rules: CMD_VERIFY filename device_num error  */
#line 550 "mon_parse.y"
            { return ERR_EXPECT_ADDRESS; }
#line 3299 "mon_parse.c"
    break;

  case 149: /* disk_rules: CMD_BLOCK_READ expression expression opt_address end_cmd  */
#line 552 "mon_parse.y"
            { mon_drive_block_cmd(0,(yyvsp[-3].i),(yyvsp[-2].i),(yyvsp[-1].a)); }
#line 3305 "mon_parse.c"
    break;

  case 150: /* disk_rules: CMD_BLOCK_WRITE expression expression address end_cmd  */
#line 554 "mon_parse.y"
            { mon_drive_block_cmd(1,(yyvsp[-3].i),(yyvsp[-2].i),(yyvsp[-1].a)); }
#line 3311 "mon_parse.c"
    break;

  case 151: /* disk_rules: CMD_LIST end_cmd  */
#line 556 "mon_parse.y"
            { mon_drive_list(-1); }
#line 3317 "mon_parse.c"
    break;

  case 152: /* disk_rules: CMD_LIST device_num end_cmd  */
#line 558 "mon_parse.y"
            { mon_drive_list((yyvsp[-1].i)); }
#line 3323 "mon_parse.c"
    break;

  case 153: /* disk_rules: CMD_ATTACH filename expression end_cmd  */
#line 560 "mon_parse.y"
            { mon_attach((yyvsp[-2].str),(yyvsp[-1].i)); }
#line 3329 "mon_parse.c"
    break;

  case 154: /* disk_rules: CMD_DETACH expression end_cmd  */
#line 562 "mon_parse.y"
            { mon_detach((yyvsp[-1].i)); }
#line 3335 "mon_parse.c"
    break;

  case 155: /* disk_rules: CMD_AUTOSTART filename end_cmd  */
#line 564 "mon_parse.y"
            { mon_autostart((yyvsp[-1].str),0,1); }
#line 3341 "mon_parse.c"
    break;

  case 156: /* disk_rules: CMD_AUTOSTART filename opt_sep number end_cmd  */
#line 566 "mon_parse.y"
            { mon_autostart((yyvsp[-3].str),(yyvsp[-1].i),1); }
#line 3347 "mon_parse.c"
    break;

  case 157: /* disk_rules: CMD_AUTOLOAD filename end_cmd  */
#line 568 "mon_parse.y"
            { mon_autostart((yyvsp[-1].str),0,0); }
#line 3353 "mon_parse.c"
    break;

  case 158: /* disk_rules: CMD_AUTOLOAD filename opt_sep number end_cmd  */
#line 570 "mon_parse.y"
            { mon_autostart((yyvsp[-3].str),(yyvsp[-1].i),0); }
#line 3359 "mon_parse.c"
    break;

  case 159: /* cmd_file_rules: CMD_RECORD filename end_cmd  */
#line 574 "mon_parse.y"
                { mon_record_commands((yyvsp[-1].str)); }
#line 3365 "mon_parse.c"
    break;

  case 160: /* cmd_file_rules: CMD_MON_STOP end_cmd  */
#line 576 "mon_parse.y"
                { mon_end_recording(); }
#line 3371 "mon_parse.c"
    break;

  case 161: /* cmd_file_rules: CMD_PLAYBACK filename end_cmd  */
#line 578 "mon_parse.y"
                { mon_playback_init((yyvsp[-1].str)); }
#line 3377 "mon_parse.c"
    break;

  case 162: /* data_entry_rules: CMD_ENTER_DATA address data_list end_cmd  */
#line 582 "mon_parse.y"
                  { mon_memory_fill((yyvsp[-2].a), BAD_ADDR, (unsigned char *)(yyvsp[-1].str)); }
#line 3383 "mon_parse.c"
    break;

  case 163: /* data_entry_rules: CMD_ENTER_BIN_DATA end_cmd  */
#line 584 "mon_parse.y"
                  { printf("Not yet.\n"); }
#line 3389 "mon_parse.c"
    break;

  case 164: /* monitor_debug_rules: CMD_YYDEBUG end_cmd  */
#line 588 "mon_parse.y"
                     { yydebug = 1; }
#line 3395 "mon_parse.c"
    break;

  case 165: /* rest_of_line: R_O_L  */
#line 591 "mon_parse.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 3401 "mon_parse.c"
    break;

  case 166: /* opt_rest_of_line: R_O_L  */
#line 594 "mon_parse.y"
                        { (yyval.str) = (yyvsp[0].str); }
#line 3407 "mon_parse.c"
    break;

  case 167: /* opt_rest_of_line: %empty  */
#line 595 "mon_parse.y"
                    { (yyval.str) = NULL; }
#line 3413 "mon_parse.c"
    break;

  case 169: /* filename: error  */
#line 599 "mon_parse.y"
                { return ERR_EXPECT_FILENAME; }
#line 3419 "mon_parse.c"
    break;

  case 171: /* device_num: error  */
#line 603 "mon_parse.y"
              { return ERR_EXPECT_DEVICE_NUM; }
#line 3425 "mon_parse.c"
    break;

  case 172: /* mem_op: mem_op MEM_OP  */
#line 606 "mon_parse.y"
                      { (yyval.i) = (yyvsp[-1].i) | (yyvsp[0].i); }
#line 3431 "mon_parse.c"
    break;

  case 173: /* mem_op: MEM_OP  */
#line 607 "mon_parse.y"
               { (yyval.i) = (yyvsp[0].i); }
#line 3437 "mon_parse.c"
    break;

  case 174: /* opt_mem_op: mem_op  */
#line 610 "mon_parse.y"
                   { (yyval.i) = (yyvsp[0].i); }
#line 3443 "mon_parse.c"
    break;

  case 175: /* opt_mem_op: %empty  */
#line 611 "mon_parse.y"
            { (yyval.i) = 0; }
#line 3449 "mon_parse.c"
    break;

  case 176: /* register: MON_REGISTER  */
#line 614 "mon_parse.y"
                                {
                                    if (!mon_register_valid(default_memspace, (yyvsp[0].reg))) {
                                        return ERR_INVALID_REGISTER;
                                    }
                                    (yyval.i) = new_reg(default_memspace, (yyvsp[0].reg));
                                }
#line 3460 "mon_parse.c"
    break;

  case 177: /* register: memspace MON_REGISTER  */
#line 620 "mon_parse.y"
                                {
                                    if (!mon_register_valid((yyvsp[-1].i), (yyvsp[0].reg))) {
                                        return ERR_INVALID_REGISTER;
                                    }
                                    (yyval.i) = new_reg((yyvsp[-1].i), (yyvsp[0].reg));
                                }
#line 3471 "mon_parse.c"
    break;

  case 180: /* reg_asgn: register EQUALS number  */
#line 633 "mon_parse.y"
          { (monitor_cpu_for_memspace[reg_memspace((yyvsp[-2].i))]->mon_register_set_val)(reg_memspace((yyvsp[-2].i)), reg_regid((yyvsp[-2].i)), (uint16_t) (yyvsp[0].i)); }
#line 3477 "mon_parse.c"
    break;

  case 181: /* checkpt_num: d_number  */
#line 636 "mon_parse.y"
                      { (yyval.i) = (yyvsp[0].i); }
#line 3483 "mon_parse.c"
    break;

  case 182: /* checkpt_num: error  */
#line 637 "mon_parse.y"
                   { return ERR_EXPECT_CHECKNUM; }
#line 3489 "mon_parse.c"
    break;

  case 184: /* address_opt_range: address  */
#line 641 "mon_parse.y"
                           { (yyval.range)[0] = (yyvsp[0].a); (yyval.range)[1] = BAD_ADDR; }
#line 3495 "mon_parse.c"
    break;

  case 185: /* address_range: address opt_sep address  */
#line 644 "mon_parse.y"
                                       { (yyval.range)[0] = (yyvsp[-2].a); (yyval.range)[1] = (yyvsp[0].a); }
#line 3501 "mon_parse.c"
    break;

  case 186: /* address_range: H_RANGE_GUESS  */
#line 646 "mon_parse.y"
               { if (resolve_range(e_default_space, (yyval.range), (yyvsp[0].str))) return ERR_ADDR_TOO_BIG; }
#line 3507 "mon_parse.c"
    break;

  case 187: /* address_range: memspace opt_sep H_RANGE_GUESS  */
#line 648 "mon_parse.y"
               { if (resolve_range((yyvsp[-2].i), (yyval.range), (yyvsp[0].str))) return ERR_ADDR_TOO_BIG; }
#line 3513 "mon_parse.c"
    break;

  case 188: /* opt_address: opt_sep address  */
#line 651 "mon_parse.y"
                             { (yyval.a) = (yyvsp[0].a); }
#line 3519 "mon_parse.c"
    break;

  case 189: /* opt_address: %empty  */
#line 652 "mon_parse.y"
             { (yyval.a) = BAD_ADDR; }
#line 3525 "mon_parse.c"
    break;

  case 190: /* address: memloc  */
#line 656 "mon_parse.y"
         {
             (yyval.a) = new_addr(e_default_space,(yyvsp[0].i));
             if (opt_asm) new_cmd = asm_mode = 1;
         }
#line 3534 "mon_parse.c"
    break;

  case 191: /* address: memspace opt_sep memloc  */
#line 661 "mon_parse.y"
         {
             (yyval.a) = new_addr((yyvsp[-2].i), (yyvsp[0].i));
             if (opt_asm) new_cmd = asm_mode = 1;
         }
#line 3543 "mon_parse.c"
    break;

  case 192: /* address: LABEL  */
#line 666 "mon_parse.y"
         {
             temp = mon_symbol_table_lookup_addr(e_default_space, (yyvsp[0].str));
             if (temp >= 0)
                 (yyval.a) = new_addr(e_default_space, temp);
             else
                 return ERR_UNDEFINED_LABEL;
         }
#line 3555 "mon_parse.c"
    break;

  case 195: /* memspace: MEM_COMP  */
#line 679 "mon_parse.y"
                   { (yyval.i) = e_comp_space; }
#line 3561 "mon_parse.c"
    break;

  case 196: /* memspace: MEM_DISK8  */
#line 680 "mon_parse.y"
                    { (yyval.i) = e_disk8_space; }
#line 3567 "mon_parse.c"
    break;

  case 197: /* memspace: MEM_DISK9  */
#line 681 "mon_parse.y"
                    { (yyval.i) = e_disk9_space; }
#line 3573 "mon_parse.c"
    break;

  case 198: /* memspace: MEM_DISK10  */
#line 682 "mon_parse.y"
                     { (yyval.i) = e_disk10_space; }
#line 3579 "mon_parse.c"
    break;

  case 199: /* memspace: MEM_DISK11  */
#line 683 "mon_parse.y"
                     { (yyval.i) = e_disk11_space; }
#line 3585 "mon_parse.c"
    break;

  case 200: /* memloc: memaddr  */
#line 686 "mon_parse.y"
                { (yyval.i) = (yyvsp[0].i); if (!CHECK_ADDR((yyvsp[0].i))) return ERR_ADDR_TOO_BIG; }
#line 3591 "mon_parse.c"
    break;

  case 201: /* memaddr: number  */
#line 688 "mon_parse.y"
                { (yyval.i) = (yyvsp[0].i); }
#line 3597 "mon_parse.c"
    break;

  case 202: /* expression: expression '+' expression  */
#line 690 "mon_parse.y"
                                      { (yyval.i) = (yyvsp[-2].i) + (yyvsp[0].i); }
#line 3603 "mon_parse.c"
    break;

  case 203: /* expression: expression '-' expression  */
#line 691 "mon_parse.y"
                                      { (yyval.i) = (yyvsp[-2].i) - (yyvsp[0].i); }
#line 3609 "mon_parse.c"
    break;

  case 204: /* expression: expression '*' expression  */
#line 692 "mon_parse.y"
                                      { (yyval.i) = (yyvsp[-2].i) * (yyvsp[0].i); }
#line 3615 "mon_parse.c"
    break;

  case 205: /* expression: expression '/' expression  */
#line 693 "mon_parse.y"
                                      { (yyval.i) = ((yyvsp[0].i)) ? ((yyvsp[-2].i) / (yyvsp[0].i)) : 1; }
#line 3621 "mon_parse.c"
    break;

  case 206: /* expression: '(' expression ')'  */
#line 694 "mon_parse.y"
                               { (yyval.i) = (yyvsp[-1].i); }
#line 3627 "mon_parse.c"
    break;

  case 207: /* expression: '(' expression error  */
#line 695 "mon_parse.y"
                                 { return ERR_MISSING_CLOSE_PAREN; }
#line 3633 "mon_parse.c"
    break;

  case 208: /* expression: value  */
#line 696 "mon_parse.y"
                  { (yyval.i) = (yyvsp[0].i); }
#line 3639 "mon_parse.c"
    break;

  case 209: /* opt_if_cond_expr: IF cond_expr  */
#line 699 "mon_parse.y"
                               { (yyval.cond_node) = (yyvsp[0].cond_node); }
#line 3645 "mon_parse.c"
    break;

  case 210: /* opt_if_cond_expr: %empty  */
#line 700 "mon_parse.y"
                  { (yyval.cond_node) = 0; }
#line 3651 "mon_parse.c"
    break;

  case 211: /* cond_expr: cond_expr COMPARE_OP cond_expr  */
#line 703 "mon_parse.y"
           {
               (yyval.cond_node) = new_cond; (yyval.cond_node)->is_parenthized = FALSE;
               (yyval.cond_node)->child1 = (yyvsp[-2].cond_node); (yyval.cond_node)->child2 = (yyvsp[0].cond_node); (yyval.cond_node)->operation = (yyvsp[-1].cond_op);
           }
#line 3660 "mon_parse.c"
    break;

  case 212: /* cond_expr: cond_expr COMPARE_OP error  */
#line 708 "mon_parse.y"
           { return ERR_INCOMPLETE_COMPARE_OP; }
#line 3666 "mon_parse.c"
    break;

  case 213: /* cond_expr: L_PAREN cond_expr R_PAREN  */
#line 710 "mon_parse.y"
           { (yyval.cond_node) = (yyvsp[-1].cond_node); (yyval.cond_node)->is_parenthized = TRUE; }
#line 3672 "mon_parse.c"
    break;

  case 214: /* cond_expr: L_PAREN cond_expr error  */
#line 712 "mon_parse.y"
           { return ERR_MISSING_CLOSE_PAREN; }
#line 3678 "mon_parse.c"
    break;

  case 215: /* cond_expr: compare_operand  */
#line 714 "mon_parse.y"
           { (yyval.cond_node) = (yyvsp[0].cond_node); }
#line 3684 "mon_parse.c"
    break;

  case 216: /* compare_operand: register  */
#line 717 "mon_parse.y"
                          { (yyval.cond_node) = new_cond;
                            (yyval.cond_node)->operation = e_INV;
                            (yyval.cond_node)->is_parenthized = FALSE;
                            (yyval.cond_node)->reg_num = (yyvsp[0].i); (yyval.cond_node)->is_reg = TRUE; (yyval.cond_node)->banknum=-1;
                            (yyval.cond_node)->child1 = NULL; (yyval.cond_node)->child2 = NULL;
                          }
#line 3695 "mon_parse.c"
    break;

  case 217: /* compare_operand: number  */
#line 723 "mon_parse.y"
                          { (yyval.cond_node) = new_cond;
                            (yyval.cond_node)->operation = e_INV;
                            (yyval.cond_node)->is_parenthized = FALSE;
                            (yyval.cond_node)->value = (yyvsp[0].i); (yyval.cond_node)->is_reg = FALSE; (yyval.cond_node)->banknum=-1;
                            (yyval.cond_node)->child1 = NULL; (yyval.cond_node)->child2 = NULL;
                          }
#line 3706 "mon_parse.c"
    break;

  case 218: /* compare_operand: '@' BANKNAME ':' address  */
#line 729 "mon_parse.y"
                                           {(yyval.cond_node)=new_cond;
                            (yyval.cond_node)->operation=e_INV;
                            (yyval.cond_node)->is_parenthized = FALSE;
                            (yyval.cond_node)->banknum=mon_banknum_from_bank(e_default_space,(yyvsp[-2].str)); (yyval.cond_node)->value = (yyvsp[0].a); (yyval.cond_node)->is_reg = FALSE;
                            (yyval.cond_node)->child1 = NULL; (yyval.cond_node)->child2 = NULL;  
                        }
#line 3717 "mon_parse.c"
    break;

  case 221: /* data_element: number  */
#line 741 "mon_parse.y"
                     { mon_add_number_to_buffer((yyvsp[0].i)); }
#line 3723 "mon_parse.c"
    break;

  case 222: /* data_element: STRING  */
#line 742 "mon_parse.y"
                     { mon_add_string_to_buffer((yyvsp[0].str)); }
#line 3729 "mon_parse.c"
    break;

  case 225: /* hunt_element: number  */
#line 749 "mon_parse.y"
                     { mon_add_number_to_buffer((yyvsp[0].i)); }
#line 3735 "mon_parse.c"
    break;

  case 226: /* hunt_element: MASK  */
#line 750 "mon_parse.y"
                     { mon_add_number_masked_to_buffer((yyvsp[0].i), 0x00); }
#line 3741 "mon_parse.c"
    break;

  case 227: /* hunt_element: STRING  */
#line 751 "mon_parse.y"
                     { mon_add_string_to_buffer((yyvsp[0].str)); }
#line 3747 "mon_parse.c"
    break;

  case 228: /* value: number  */
#line 754 "mon_parse.y"
              { (yyval.i) = (yyvsp[0].i); }
#line 3753 "mon_parse.c"
    break;

  case 229: /* value: register  */
#line 755 "mon_parse.y"
                { (yyval.i) = (monitor_cpu_for_memspace[reg_memspace((yyvsp[0].i))]->mon_register_get_val)(reg_memspace((yyvsp[0].i)), reg_regid((yyvsp[0].i))); }
#line 3759 "mon_parse.c"
    break;

  case 230: /* d_number: D_NUMBER  */
#line 758 "mon_parse.y"
                   { (yyval.i) = (yyvsp[0].i); }
#line 3765 "mon_parse.c"
    break;

  case 231: /* d_number: B_NUMBER_GUESS  */
#line 759 "mon_parse.y"
                         { (yyval.i) = strtol((yyvsp[0].str), NULL, 10); }
#line 3771 "mon_parse.c"
    break;

  case 232: /* d_number: O_NUMBER_GUESS  */
#line 760 "mon_parse.y"
                         { (yyval.i) = strtol((yyvsp[0].str), NULL, 10); }
#line 3777 "mon_parse.c"
    break;

  case 233: /* d_number: D_NUMBER_GUESS  */
#line 761 "mon_parse.y"
                         { (yyval.i) = strtol((yyvsp[0].str), NULL, 10); }
#line 3783 "mon_parse.c"
    break;

  case 234: /* guess_default: B_NUMBER_GUESS  */
#line 764 "mon_parse.y"
                              { (yyval.i) = resolve_datatype(B_NUMBER,(yyvsp[0].str)); }
#line 3789 "mon_parse.c"
    break;

  case 235: /* guess_default: O_NUMBER_GUESS  */
#line 765 "mon_parse.y"
                              { (yyval.i) = resolve_datatype(O_NUMBER,(yyvsp[0].str)); }
#line 3795 "mon_parse.c"
    break;

  case 236: /* guess_default: D_NUMBER_GUESS  */
#line 766 "mon_parse.y"
                              { (yyval.i) = resolve_datatype(D_NUMBER,(yyvsp[0].str)); }
#line 3801 "mon_parse.c"
    break;

  case 237: /* number: H_NUMBER  */
#line 769 "mon_parse.y"
                 { (yyval.i) = (yyvsp[0].i); }
#line 3807 "mon_parse.c"
    break;

  case 238: /* number: D_NUMBER  */
#line 770 "mon_parse.y"
                 { (yyval.i) = (yyvsp[0].i); }
#line 3813 "mon_parse.c"
    break;

  case 239: /* number: O_NUMBER  */
#line 771 "mon_parse.y"
                 { (yyval.i) = (yyvsp[0].i); }
#line 3819 "mon_parse.c"
    break;

  case 240: /* number: B_NUMBER  */
#line 772 "mon_parse.y"
                 { (yyval.i) = (yyvsp[0].i); }
#line 3825 "mon_parse.c"
    break;

  case 241: /* number: guess_default  */
#line 773 "mon_parse.y"
                      { (yyval.i) = (yyvsp[0].i); }
#line 3831 "mon_parse.c"
    break;

  case 245: /* assembly_instruction: OPCODE asm_operand_mode  */
#line 781 "mon_parse.y"
                                              { (yyval.i) = 0;
                                                if ((yyvsp[-1].str)) {
                                                    (monitor_cpu_for_memspace[default_memspace]->mon_assemble_instr)((yyvsp[-1].str), (yyvsp[0].mode));
                                                } else {
                                                    new_cmd = 1;
                                                    asm_mode = 0;
                                                }
                                                opt_asm = 0;
                                              }
#line 3845 "mon_parse.c"
    break;

  case 247: /* post_assemble: assembly_instr_list  */
#line 792 "mon_parse.y"
                                   { asm_mode = 0; }
#line 3851 "mon_parse.c"
    break;

  case 248: /* asm_operand_mode: ARG_IMMEDIATE number  */
#line 795 "mon_parse.y"
                                       { if ((yyvsp[0].i) > 0xff) {
                          (yyval.mode).addr_mode = ASM_ADDR_MODE_IMMEDIATE_16;
                          (yyval.mode).param = (yyvsp[0].i);
                        } else {
                          (yyval.mode).addr_mode = ASM_ADDR_MODE_IMMEDIATE;
                          (yyval.mode).param = (yyvsp[0].i);
                        } }
#line 3863 "mon_parse.c"
    break;

  case 249: /* asm_operand_mode: number  */
#line 802 "mon_parse.y"
           { if ((yyvsp[0].i) >= 0x10000) {
               (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_LONG;
               (yyval.mode).param = (yyvsp[0].i);
             } else if ((yyvsp[0].i) < 0x100) {
               (yyval.mode).addr_mode = ASM_ADDR_MODE_ZERO_PAGE;
               (yyval.mode).param = (yyvsp[0].i);
             } else {
               (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE;
               (yyval.mode).param = (yyvsp[0].i);
             }
           }
#line 3879 "mon_parse.c"
    break;

  case 250: /* asm_operand_mode: number COMMA REG_X  */
#line 813 "mon_parse.y"
                        { if ((yyvsp[-2].i) >= 0x10000) {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_LONG_X;
                            (yyval.mode).param = (yyvsp[-2].i);
                          } else if ((yyvsp[-2].i) < 0x100) { 
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_ZERO_PAGE_X;
                            (yyval.mode).param = (yyvsp[-2].i);
                          } else {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_X;
                            (yyval.mode).param = (yyvsp[-2].i);
                          }
                        }
#line 3895 "mon_parse.c"
    break;

  case 251: /* asm_operand_mode: number COMMA REG_Y  */
#line 824 "mon_parse.y"
                        { if ((yyvsp[-2].i) < 0x100) {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_ZERO_PAGE_Y;
                            (yyval.mode).param = (yyvsp[-2].i);
                          } else {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_Y;
                            (yyval.mode).param = (yyvsp[-2].i);
                          }
                        }
#line 3908 "mon_parse.c"
    break;

  case 252: /* asm_operand_mode: number COMMA REG_S  */
#line 832 "mon_parse.y"
                        { if ((yyvsp[-2].i) < 0x100) {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_STACK_RELATIVE;
                            (yyval.mode).param = (yyvsp[-2].i);
                          } else { /* 6809 */
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
                            if ((yyvsp[-2].i) >= -16 && (yyvsp[-2].i) < 16) {
                                (yyval.mode).addr_submode = (yyvsp[0].i) | ((yyvsp[-2].i) & 0x1F);
                            } else if ((yyvsp[-2].i) >= -128 && (yyvsp[-2].i) < 128) {
                                (yyval.mode).addr_submode = 0x80 | (yyvsp[0].i) | ASM_ADDR_MODE_INDEXED_OFF8;
                                (yyval.mode).param = (yyvsp[-2].i);
                            } else if ((yyvsp[-2].i) >= -32768 && (yyvsp[-2].i) < 32768) {
                                (yyval.mode).addr_submode = 0x80 | (yyvsp[0].i) | ASM_ADDR_MODE_INDEXED_OFF16;
                                (yyval.mode).param = (yyvsp[-2].i);
                            } else {
                                (yyval.mode).addr_mode = ASM_ADDR_MODE_ILLEGAL;
                                mon_out("offset too large even for 16 bits (signed)\n");
                            }
                          }
                        }
#line 3932 "mon_parse.c"
    break;

  case 253: /* asm_operand_mode: number COMMA number  */
#line 851 "mon_parse.y"
                        { if ((yyvsp[-2].i) < 0x100) {
                            (yyval.mode).addr_mode = ASM_ADDR_MODE_DOUBLE;
                            (yyval.mode).param = (yyvsp[0].i);
                            (yyval.mode).addr_submode = (yyvsp[-2].i);
                          }
                        }
#line 3943 "mon_parse.c"
    break;

  case 254: /* asm_operand_mode: L_PAREN number R_PAREN  */
#line 857 "mon_parse.y"
                           { if ((yyvsp[-1].i) < 0x100) {
                               (yyval.mode).addr_mode = ASM_ADDR_MODE_INDIRECT;
                               (yyval.mode).param = (yyvsp[-1].i);
                             } else {
                               (yyval.mode).addr_mode = ASM_ADDR_MODE_ABS_INDIRECT;
                               (yyval.mode).param = (yyvsp[-1].i);
                             }
                           }
#line 3956 "mon_parse.c"
    break;

  case 255: /* asm_operand_mode: L_PAREN number COMMA REG_X R_PAREN  */
#line 865 "mon_parse.y"
                                       { if ((yyvsp[-3].i) < 0x100) {
                                           (yyval.mode).addr_mode = ASM_ADDR_MODE_INDIRECT_X;
                                           (yyval.mode).param = (yyvsp[-3].i);
                                         } else {
                                           (yyval.mode).addr_mode = ASM_ADDR_MODE_ABS_INDIRECT_X;
                                           (yyval.mode).param = (yyvsp[-3].i);
                                         }
                                       }
#line 3969 "mon_parse.c"
    break;

  case 256: /* asm_operand_mode: L_PAREN number COMMA REG_S R_PAREN COMMA REG_Y  */
#line 874 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_STACK_RELATIVE_Y; (yyval.mode).param = (yyvsp[-5].i); }
#line 3975 "mon_parse.c"
    break;

  case 257: /* asm_operand_mode: L_PAREN number R_PAREN COMMA REG_Y  */
#line 876 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_INDIRECT_Y; (yyval.mode).param = (yyvsp[-3].i); }
#line 3981 "mon_parse.c"
    break;

  case 258: /* asm_operand_mode: L_PAREN REG_BC R_PAREN  */
#line 877 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_BC; }
#line 3987 "mon_parse.c"
    break;

  case 259: /* asm_operand_mode: L_PAREN REG_DE R_PAREN  */
#line 878 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_DE; }
#line 3993 "mon_parse.c"
    break;

  case 260: /* asm_operand_mode: L_PAREN REG_HL R_PAREN  */
#line 879 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_HL; }
#line 3999 "mon_parse.c"
    break;

  case 261: /* asm_operand_mode: L_PAREN REG_IX R_PAREN  */
#line 880 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_IX; }
#line 4005 "mon_parse.c"
    break;

  case 262: /* asm_operand_mode: L_PAREN REG_IY R_PAREN  */
#line 881 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_IY; }
#line 4011 "mon_parse.c"
    break;

  case 263: /* asm_operand_mode: L_PAREN REG_SP R_PAREN  */
#line 882 "mon_parse.y"
                           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IND_SP; }
#line 4017 "mon_parse.c"
    break;

  case 264: /* asm_operand_mode: L_PAREN number R_PAREN COMMA REG_A  */
#line 884 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_A; (yyval.mode).param = (yyvsp[-3].i); }
#line 4023 "mon_parse.c"
    break;

  case 265: /* asm_operand_mode: L_PAREN number R_PAREN COMMA REG_HL  */
#line 886 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_HL; (yyval.mode).param = (yyvsp[-3].i); }
#line 4029 "mon_parse.c"
    break;

  case 266: /* asm_operand_mode: L_PAREN number R_PAREN COMMA REG_IX  */
#line 888 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_IX; (yyval.mode).param = (yyvsp[-3].i); }
#line 4035 "mon_parse.c"
    break;

  case 267: /* asm_operand_mode: L_PAREN number R_PAREN COMMA REG_IY  */
#line 890 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_ABSOLUTE_IY; (yyval.mode).param = (yyvsp[-3].i); }
#line 4041 "mon_parse.c"
    break;

  case 268: /* asm_operand_mode: %empty  */
#line 891 "mon_parse.y"
    { (yyval.mode).addr_mode = ASM_ADDR_MODE_IMPLIED; }
#line 4047 "mon_parse.c"
    break;

  case 269: /* asm_operand_mode: REG_A  */
#line 892 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_ACCUMULATOR; }
#line 4053 "mon_parse.c"
    break;

  case 270: /* asm_operand_mode: REG_B  */
#line 893 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_B; }
#line 4059 "mon_parse.c"
    break;

  case 271: /* asm_operand_mode: REG_C  */
#line 894 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_C; }
#line 4065 "mon_parse.c"
    break;

  case 272: /* asm_operand_mode: REG_D  */
#line 895 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_D; }
#line 4071 "mon_parse.c"
    break;

  case 273: /* asm_operand_mode: REG_E  */
#line 896 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_E; }
#line 4077 "mon_parse.c"
    break;

  case 274: /* asm_operand_mode: REG_H  */
#line 897 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_H; }
#line 4083 "mon_parse.c"
    break;

  case 275: /* asm_operand_mode: REG_IXH  */
#line 898 "mon_parse.y"
            { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IXH; }
#line 4089 "mon_parse.c"
    break;

  case 276: /* asm_operand_mode: REG_IYH  */
#line 899 "mon_parse.y"
            { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IYH; }
#line 4095 "mon_parse.c"
    break;

  case 277: /* asm_operand_mode: REG_L  */
#line 900 "mon_parse.y"
          { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_L; }
#line 4101 "mon_parse.c"
    break;

  case 278: /* asm_operand_mode: REG_IXL  */
#line 901 "mon_parse.y"
            { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IXL; }
#line 4107 "mon_parse.c"
    break;

  case 279: /* asm_operand_mode: REG_IYL  */
#line 902 "mon_parse.y"
            { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IYL; }
#line 4113 "mon_parse.c"
    break;

  case 280: /* asm_operand_mode: REG_AF  */
#line 903 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_AF; }
#line 4119 "mon_parse.c"
    break;

  case 281: /* asm_operand_mode: REG_BC  */
#line 904 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_BC; }
#line 4125 "mon_parse.c"
    break;

  case 282: /* asm_operand_mode: REG_DE  */
#line 905 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_DE; }
#line 4131 "mon_parse.c"
    break;

  case 283: /* asm_operand_mode: REG_HL  */
#line 906 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_HL; }
#line 4137 "mon_parse.c"
    break;

  case 284: /* asm_operand_mode: REG_IX  */
#line 907 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IX; }
#line 4143 "mon_parse.c"
    break;

  case 285: /* asm_operand_mode: REG_IY  */
#line 908 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_IY; }
#line 4149 "mon_parse.c"
    break;

  case 286: /* asm_operand_mode: REG_SP  */
#line 909 "mon_parse.y"
           { (yyval.mode).addr_mode = ASM_ADDR_MODE_REG_SP; }
#line 4155 "mon_parse.c"
    break;

  case 287: /* asm_operand_mode: LESS_THAN number  */
#line 911 "mon_parse.y"
                     { (yyval.mode).addr_mode = ASM_ADDR_MODE_DIRECT; (yyval.mode).param = (yyvsp[0].i); }
#line 4161 "mon_parse.c"
    break;

  case 288: /* asm_operand_mode: number COMMA index_ureg  */
#line 912 "mon_parse.y"
                            {    /* Clash with addr,x addr,y addr,s modes! */
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        if ((yyvsp[-2].i) >= -16 && (yyvsp[-2].i) < 16) {
            (yyval.mode).addr_submode = (3 << 5) | ((yyvsp[-2].i) & 0x1F);
        } else if ((yyvsp[-2].i) >= -128 && (yyvsp[-2].i) < 128) {
            (yyval.mode).addr_submode = 0x80 | (3 << 5) | ASM_ADDR_MODE_INDEXED_OFF8;
            (yyval.mode).param = (yyvsp[-2].i);
        } else if ((yyvsp[-2].i) >= -32768 && (yyvsp[-2].i) < 32768) {
            (yyval.mode).addr_submode = 0x80 | (3 << 5) | ASM_ADDR_MODE_INDEXED_OFF16;
            (yyval.mode).param = (yyvsp[-2].i);
        } else {
            (yyval.mode).addr_mode = ASM_ADDR_MODE_ILLEGAL;
            mon_out("offset too large even for 16 bits (signed)\n");
        }
    }
#line 4181 "mon_parse.c"
    break;

  case 289: /* asm_operand_mode: COMMA index_reg PLUS  */
#line 927 "mon_parse.y"
                         {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_INC1;
        }
#line 4190 "mon_parse.c"
    break;

  case 290: /* asm_operand_mode: COMMA index_reg PLUS PLUS  */
#line 931 "mon_parse.y"
                              {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-2].i) | ASM_ADDR_MODE_INDEXED_INC2;
        }
#line 4199 "mon_parse.c"
    break;

  case 291: /* asm_operand_mode: COMMA MINUS index_reg  */
#line 935 "mon_parse.y"
                          {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[0].i) | ASM_ADDR_MODE_INDEXED_DEC1;
        }
#line 4208 "mon_parse.c"
    break;

  case 292: /* asm_operand_mode: COMMA MINUS MINUS index_reg  */
#line 939 "mon_parse.y"
                                {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[0].i) | ASM_ADDR_MODE_INDEXED_DEC2;
        }
#line 4217 "mon_parse.c"
    break;

  case 293: /* asm_operand_mode: COMMA index_reg  */
#line 943 "mon_parse.y"
                    {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[0].i) | ASM_ADDR_MODE_INDEXED_OFF0;
        }
#line 4226 "mon_parse.c"
    break;

  case 294: /* asm_operand_mode: REG_B COMMA index_reg  */
#line 947 "mon_parse.y"
                          {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_OFFB;
        }
#line 4235 "mon_parse.c"
    break;

  case 295: /* asm_operand_mode: REG_A COMMA index_reg  */
#line 951 "mon_parse.y"
                          {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_OFFA;
        }
#line 4244 "mon_parse.c"
    break;

  case 296: /* asm_operand_mode: REG_D COMMA index_reg  */
#line 955 "mon_parse.y"
                          {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_OFFD;
        }
#line 4253 "mon_parse.c"
    break;

  case 297: /* asm_operand_mode: number COMMA REG_PC  */
#line 959 "mon_parse.y"
                        {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).param = (yyvsp[-2].i);
        if ((yyvsp[-2].i) >= -128 && (yyvsp[-2].i) < 128) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFFPC8;
        } else if ((yyvsp[-2].i) >= -32768 && (yyvsp[-2].i) < 32768) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFFPC16;
        } else {
            (yyval.mode).addr_mode = ASM_ADDR_MODE_ILLEGAL;
            mon_out("offset too large even for 16 bits (signed)\n");
        }
    }
#line 4270 "mon_parse.c"
    break;

  case 298: /* asm_operand_mode: L_BRACKET number COMMA index_reg R_BRACKET  */
#line 971 "mon_parse.y"
                                               {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        if ((yyvsp[-3].i) >= -16 && (yyvsp[-3].i) < 16) {
            (yyval.mode).addr_submode = (yyvsp[-3].i) & 0x1F;
        } else if ((yyvsp[-4].i) >= -128 && (yyvsp[-4].i) < 128) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFF8;
            (yyval.mode).param = (yyvsp[-3].i);
        } else if ((yyvsp[-3].i) >= -32768 && (yyvsp[-3].i) < 32768) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFF16;
            (yyval.mode).param = (yyvsp[-3].i);
        } else {
            (yyval.mode).addr_mode = ASM_ADDR_MODE_ILLEGAL;
            mon_out("offset too large even for 16 bits (signed)\n");
        }
    }
#line 4290 "mon_parse.c"
    break;

  case 299: /* asm_operand_mode: L_BRACKET COMMA index_reg PLUS R_BRACKET  */
#line 986 "mon_parse.y"
                                             {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-2].i) | ASM_ADDR_MODE_INDEXED_INC1;
        }
#line 4299 "mon_parse.c"
    break;

  case 300: /* asm_operand_mode: L_BRACKET COMMA index_reg PLUS PLUS R_BRACKET  */
#line 990 "mon_parse.y"
                                                  {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-3].i) | ASM_ADDR_MODE_INDEXED_INC2;
        }
#line 4308 "mon_parse.c"
    break;

  case 301: /* asm_operand_mode: L_BRACKET COMMA MINUS index_reg R_BRACKET  */
#line 994 "mon_parse.y"
                                              {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_DEC1;
        }
#line 4317 "mon_parse.c"
    break;

  case 302: /* asm_operand_mode: L_BRACKET COMMA MINUS MINUS index_reg R_BRACKET  */
#line 998 "mon_parse.y"
                                                    {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_DEC2;
        }
#line 4326 "mon_parse.c"
    break;

  case 303: /* asm_operand_mode: L_BRACKET COMMA index_reg R_BRACKET  */
#line 1002 "mon_parse.y"
                                        {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-1].i) | ASM_ADDR_MODE_INDEXED_OFF0;
        }
#line 4335 "mon_parse.c"
    break;

  case 304: /* asm_operand_mode: L_BRACKET REG_B COMMA index_reg R_BRACKET  */
#line 1006 "mon_parse.y"
                                              {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-2].i) | ASM_ADDR_MODE_INDEXED_OFFB;
        }
#line 4344 "mon_parse.c"
    break;

  case 305: /* asm_operand_mode: L_BRACKET REG_A COMMA index_reg R_BRACKET  */
#line 1010 "mon_parse.y"
                                              {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-2].i) | ASM_ADDR_MODE_INDEXED_OFFA;
        }
#line 4353 "mon_parse.c"
    break;

  case 306: /* asm_operand_mode: L_BRACKET REG_D COMMA index_reg R_BRACKET  */
#line 1014 "mon_parse.y"
                                              {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | (yyvsp[-2].i) | ASM_ADDR_MODE_INDEXED_OFFD;
        }
#line 4362 "mon_parse.c"
    break;

  case 307: /* asm_operand_mode: L_BRACKET number COMMA REG_PC R_BRACKET  */
#line 1018 "mon_parse.y"
                                            {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).param = (yyvsp[-3].i);
        if ((yyvsp[-3].i) >= -128 && (yyvsp[-3].i) < 128) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFFPC8_IND;
        } else if ((yyvsp[-3].i) >= -32768 && (yyvsp[-3].i) < 32768) {
            (yyval.mode).addr_submode = ASM_ADDR_MODE_INDEXED_OFFPC16_IND;
        } else {
            (yyval.mode).addr_mode = ASM_ADDR_MODE_ILLEGAL;
            mon_out("offset too large even for 16 bits (signed)\n");
        }
    }
#line 4379 "mon_parse.c"
    break;

  case 308: /* asm_operand_mode: L_BRACKET number R_BRACKET  */
#line 1030 "mon_parse.y"
                               {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDEXED;
        (yyval.mode).addr_submode = 0x80 | ASM_ADDR_MODE_EXTENDED_INDIRECT;
        (yyval.mode).param = (yyvsp[-1].i);
        }
#line 4389 "mon_parse.c"
    break;

  case 309: /* asm_operand_mode: L_BRACKET number R_BRACKET COMMA REG_Y  */
#line 1035 "mon_parse.y"
                                           {
        (yyval.mode).addr_mode = ASM_ADDR_MODE_INDIRECT_LONG_Y;
        (yyval.mode).param = (yyvsp[-3].i);
        }
#line 4398 "mon_parse.c"
    break;

  case 310: /* index_reg: REG_X  */
#line 1043 "mon_parse.y"
          { (yyval.i) = (0 << 5); printf("reg_x\n"); }
#line 4404 "mon_parse.c"
    break;

  case 311: /* index_reg: REG_Y  */
#line 1044 "mon_parse.y"
          { (yyval.i) = (1 << 5); printf("reg_y\n"); }
#line 4410 "mon_parse.c"
    break;

  case 312: /* index_reg: index_ureg  */
#line 1045 "mon_parse.y"
               { (yyval.i) = (yyvsp[0].i); }
#line 4416 "mon_parse.c"
    break;

  case 313: /* index_reg: REG_S  */
#line 1046 "mon_parse.y"
          { (yyval.i) = (3 << 5); printf("reg_s\n"); }
#line 4422 "mon_parse.c"
    break;

  case 314: /* index_ureg: REG_U  */
#line 1050 "mon_parse.y"
          { (yyval.i) = (2 << 5); printf("reg_u\n"); }
#line 4428 "mon_parse.c"
    break;


#line 4432 "mon_parse.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1054 "mon_parse.y"


void parse_and_execute_line(char *input)
{
   char *temp_buf;
   int i, rc;

   temp_buf = lib_malloc(strlen(input) + 3);
   strcpy(temp_buf,input);
   i = (int)strlen(input);
   temp_buf[i++] = '\n';
   temp_buf[i++] = '\0';
   temp_buf[i++] = '\0';

   make_buffer(temp_buf);
   mon_clear_buffer();
   if ( (rc =yyparse()) != 0) {
       mon_out("ERROR -- ");
       switch(rc) {
         case ERR_BAD_CMD:
           mon_out("Bad command:\n");
           break;
         case ERR_RANGE_BAD_START:
           mon_out("Bad first address in range:\n");
           break;
         case ERR_RANGE_BAD_END:
           mon_out("Bad second address in range:\n");
           break;
         case ERR_EXPECT_CHECKNUM:
           mon_out("Checkpoint number expected:\n");
           break;
         case ERR_EXPECT_END_CMD:
           mon_out("Unexpected token:\n");
           break;
         case ERR_MISSING_CLOSE_PAREN:
           mon_out("')' expected:\n");
           break;
         case ERR_INCOMPLETE_COMPARE_OP:
           mon_out("Compare operation missing an operand:\n");
           break;
         case ERR_EXPECT_FILENAME:
           mon_out("Expecting a filename:\n");
           break;
         case ERR_ADDR_TOO_BIG:
           mon_out("Address too large:\n");
           break;
         case ERR_IMM_TOO_BIG:
           mon_out("Immediate argument too large:\n");
           break;
         case ERR_EXPECT_STRING:
           mon_out("Expecting a string.\n");
           break;
         case ERR_UNDEFINED_LABEL:
           mon_out("Found an undefined label.\n");
           break;
         case ERR_EXPECT_DEVICE_NUM:
           mon_out("Expecting a device number.\n");
           break;
         case ERR_EXPECT_ADDRESS:
           mon_out("Expecting an address.\n");
           break;
         case ERR_INVALID_REGISTER:
           mon_out("Invalid register.\n");
           break;
         case ERR_ILLEGAL_INPUT:
         default:
           mon_out("Wrong syntax:\n");
       }
       mon_out("  %s\n", input);
       for (i = 0; i < last_len; i++)
           mon_out(" ");
       mon_out("  ^\n");
       asm_mode = 0;
       new_cmd = 1;
   }
   lib_free(temp_buf);
   free_buffer();
}

static int yyerror(char *s)
{
   fprintf(stderr, "ERR:%s\n", s);
   return 0;
}

static int resolve_datatype(unsigned guess_type, const char *num)
{
   /* FIXME: Handle cases when default type is non-numerical */
   if (default_radix == e_hexadecimal) {
       return strtol(num, NULL, 16);
   }

   if ((guess_type == D_NUMBER) || (default_radix == e_decimal)) {
       return strtol(num, NULL, 10);
   }

   if ((guess_type == O_NUMBER) || (default_radix == e_octal)) {
       return strtol(num, NULL, 8);
   }

   return strtol(num, NULL, 2);
}

/*
 * Resolve a character sequence containing 8 hex digits like "08001000".
 * This could be a lazy version of "0800 1000". If the default radix is not
 * hexadecimal, we handle it like a ordinary number, in the latter case there
 * is only one number in the range.
 */
static int resolve_range(enum t_memspace memspace, MON_ADDR range[2],
                         const char *num)
{
    char start[5];
    char end[5];
    long sa;

    range[1] = BAD_ADDR;

    switch (default_radix)
    {
    case e_hexadecimal:
        /* checked twice, but as the code must have exactly 8 digits: */
        if (strlen(num) == 8) {
            memcpy(start, num, 4);
            start[4] = '\0';
            memcpy(end, num + 4, 4);
            end[4] = '\0';
            sa = strtol(start, NULL, 16);
            range[1] = new_addr(memspace, strtol(end, NULL, 16));
        }
        else
            sa = strtol(num, NULL, 16);
        break;

    case e_decimal:
       sa = strtol(num, NULL, 10);
       break;

    case e_octal:
       sa = strtol(num, NULL, 8);
       break;

    default:
       sa = strtol(num, NULL, 2);
    }

    if (!CHECK_ADDR(sa))
        return ERR_ADDR_TOO_BIG;

    range[0] = new_addr(memspace, sa);
    return 0;
}
