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

#ifndef CVUNITEXT_H_C706D321_47C9_11E5_BB17_005056C00008_INCLUDED_
#define CVUNITEXT_H_C706D321_47C9_11E5_BB17_005056C00008_INCLUDED_

typedef struct CvUniLibrary CvUniLibrary;
typedef struct CvUniFont CvUniFont;


CvUniLibrary* cvInitUniLibrary();
void cvCleanUniLibrary(CvUniLibrary* font_lib);

CvUniFont* cvInitUniFont(CvUniLibrary* font_lib, const char* font_face, int font_size);
void cvCleanUniFont(CvUniFont* font);

//org: font.left font.bottom
CvRect cvPutUniText(IplImage* img, const char* text,
        CvPoint org, CvUniFont* font, CvScalar color,
        const char* encoding, int calc);


#endif

