/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2009 - 2019 Teunis van Beelen
*
* Email: teuniz@protonmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/







#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

/************************* BANNED ************************/

#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__) || defined(__FreeBSD__)
/* nothing */
#else
#define BANNED(func) sorry_##func##_is_a_banned_function

#undef strcpy
#define strcpy(x,y) BANNED(strcpy)
#undef strcat
#define strcat(x,y) BANNED(strcat)
/*
#undef strncpy
#define strncpy(x,y,n) BANNED(strncpy)
#undef strncat
#define strncat(x,y,n) BANNED(strncat)
*/
#undef sprintf
#undef vsprintf
#ifdef HAVE_VARIADIC_MACROS
#define sprintf(...) BANNED(sprintf)
#define vsprintf(...) BANNED(vsprintf)
#else
#define sprintf(buf,fmt,arg) BANNED(sprintf)
#define vsprintf(buf,fmt,arg) BANNED(vsprintf)
#endif

#endif

/*********************************************************/


void remove_trailing_spaces(char *);
void remove_leading_spaces(char *);
void remove_trailing_zeros(char *);
void convert_trailing_zeros_to_spaces(char *);
void remove_extension_from_filename(char *);  /* removes extension including the dot */
int get_filename_from_path(char *dest, const char *src, int size);  /* size is size of destination, returns length of filename */
int get_directory_from_path(char *dest, const char *src, int size);  /* size is size of destination, returns length of directory */
void latin1_to_ascii(char *, int);
void latin1_to_utf8(char *, int);
void utf8_to_latin1(char *);
int antoi(const char *, int);
int atoi_nonlocalized(const char *);
double atof_nonlocalized(const char *);
int sprint_number_nonlocalized(char *, double);
long long atoll_x(const char *, int);
void strntolower(char *, int);

/* returns also empty tokens */
char * strtok_r_e(char *, const char *, char **);

/* 3th argument is the minimum digits that will be printed (minus sign not included), leading zero's will be added if necessary */
/* if 4th argument is zero, only negative numbers will have the sign '-' character */
/* if 4th argument is one, the sign '+' or '-' character will always be printed */
/* returns the amount of characters printed */
int sprint_int_number_nonlocalized(char *, int, int, int);
int sprint_ll_number_nonlocalized(char *, long long, int, int);
int fprint_int_number_nonlocalized(FILE *, int, int, int);
int fprint_ll_number_nonlocalized(FILE *, long long, int, int);

/* returns 1 in case the string is not a number */
int is_integer_number(char *);
int is_number(char *);

int round_125_cat(double);  /* returns 10, 20 or 50, depending on the value */

void hextoascii(char *);  /* inline copy */
void bintoascii(char *);  /* inline copy */
void bintohex(char *);    /* inline copy */
void asciitohex(char *, const char *);  /* destination must have double the size of source! */
void asciitobin(char *, const char *);  /* destination must have eight times the size of source! */
void hextobin(char *, const char *);    /* destination must have four times the size of source! */

/* Converts a double to Giga/Mega/Kilo/milli/micro/etc. */
/* int is number of decimals and size of destination. Result is written into the string argument */
int convert_to_metric_suffix(char *, double, int, int);

double round_up_step125(double, double *);      /* Rounds the value up to 1-2-5 steps */
double round_down_step125(double, double *);    /* Rounds the value down to 1-2-5 steps */
double round_to_3digits(double);   /* Rounds the value to max 3 digits */

int strtoipaddr(unsigned int *, const char *);  /* convert a string "192.168.1.12" to an integer */

int dblcmp(double, double);  /* returns 0 when equal */

int base64_dec(const void *, void *, int);

/* sz is size of destination, returns length of string in dest.
 * This is different than the official BSD implementation!
 * From the BSD man-page:
 * "The strlcpy() and strlcat() functions return the total length of
 * the string they tried to create. For strlcpy() that means the
 * length of src. For strlcat() that means the initial length of dst
 * plus the length of src. While this may seem somewhat confusing,
 * it was done to make truncation detection simple."
 */
#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__) || defined(__FreeBSD__)
/* nothing here */
#else
int strlcpy(char *, const char *, int);
int strlcat(char *, const char *, int);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


