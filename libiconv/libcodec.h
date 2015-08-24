/*  
 *  Copyright 2013 buaa.byl@gmail.com
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef LIBCODEC_H_B7F1E351_49FB_11E5_B9B2_005056C00008_INCLUDED_
#define LIBCODEC_H_B7F1E351_49FB_11E5_B9B2_005056C00008_INCLUDED_



//iconv
//
//@param    instr   input string pointer
//@param    inlen   input string length
//@param    outlen  output string length
//@param    inenc   input string encoding
//@param    outenc  output string encoding
//
//@return           output string buffer
//                  must free by 'libcodec_free'
char* libcodec_iconv(
        const char* instr,
        int inlen,
        int* outlen,
        const char* inenc,
        const char* outenc);

void libcodec_free(char* s);

#endif

