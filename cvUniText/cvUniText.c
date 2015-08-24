#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ft2build.h>
#include <freetype.h>

#include "cvUniText.h"
#include "libcodec.h"

//#define CVUNITEXT_DEBUG

struct CvUniLibrary
{
    FT_Library  m_library;
};

struct CvUniFont
{
	CvUniLibrary* lib;
    FT_Face     m_face;

    int         m_fontType;
    CvScalar    m_fontSize;
    float       m_fontDiaphaneity;
};


static void _cvRenderChar(CvUniFont* font, wchar_t wc);

static double _cvPutUniChar(IplImage *img, wchar_t wc,
        CvPoint pos, CvUniFont* font, CvScalar color, int calc);

static CvRect _cvPutUniTextUCS2(IplImage* img, const wchar_t* text,
        CvPoint org, CvUniFont* font, CvScalar color, int calc);

CvUniLibrary* cvInitUniLibrary()
{
	CvUniLibrary* font_lib;

	font_lib = (CvUniLibrary*)malloc(sizeof(CvUniLibrary));

	if (FT_Init_FreeType(&font_lib->m_library) != 0) {
		free(font_lib);
		return NULL;
	}

	return font_lib;
}

void cvCleanUniLibrary(CvUniLibrary* font_lib)
{
    if (font_lib->m_library) {
        FT_Done_FreeType(font_lib->m_library);
        free(font_lib);
    }
}

CvUniFont* cvInitUniFont(CvUniLibrary* font_lib, const char* font_face, int font_size)
{
    CvUniFont* font;

    font = (CvUniFont*)malloc(sizeof(CvUniFont));

    font->lib = font_lib;

    if (FT_New_Face(font->lib->m_library, font_face, 0, &font->m_face) != 0) {
        free(font);
        return NULL;
    }

    font->m_fontType = 0;
    font->m_fontSize.val[0] = font_size; //FontSize
    font->m_fontSize.val[1] = 0.5;//whitechar ratio, such like ' '
    font->m_fontSize.val[2] = 0.2;//inverval ratio, for each char.
    font->m_fontSize.val[3] = 0;  //rotate
    font->m_fontDiaphaneity = 0.9f;//alpha

    FT_Set_Pixel_Sizes(font->m_face, (int)font->m_fontSize.val[0], 0);

    //---top--------.------------------
    //              | ascender
    //---baseline---.--------.---------
    //                       |descender
    //---bottom--------------.---------
    //
#ifdef CVUNITEXT_DEBUG
    printf("font.ascender  %10d\n", font->m_face->size->metrics.ascender/64);
    printf("font.baseline  %10d\n", 0);
    printf("font.descender %10d\n", font->m_face->size->metrics.descender/64);
#endif

    //FT_Set_Char_Size(face, 0, 16*64, 300, 300); //unknown

    //FT_Select_Charmap(font->face,FT_ENCODING_UNICODE); //default

    //setlocale(LC_ALL, "");

    return font;
}

void cvCleanUniFont(CvUniFont* font)
{
    if (font->m_face) {
        FT_Done_Face(font->m_face);
    }

    free(font);
}

void _cvRenderChar(CvUniFont* font, wchar_t wc)
{
    FT_UInt glyph_index;

    //get font_id from database of char
    glyph_index = FT_Get_Char_Index(font->m_face, wc);

    //load bitmap font to slot
    FT_Load_Glyph(font->m_face, glyph_index, FT_LOAD_DEFAULT);

    //render to 8bits
    FT_Render_Glyph(font->m_face->glyph, FT_RENDER_MODE_NORMAL);
}

static double _cvPutUniChar(IplImage *img, wchar_t wc,
        CvPoint pos, CvUniFont* font, CvScalar color, int calc)
{
    // generate font bitmap from unicode
    FT_GlyphSlot ft_slot;
    int ft_ascender;
    int ft_bmp_height;
    int ft_bmp_width;//0 when ' '
    int ft_bmp_step;
    int ft_bmp_left;
    int ft_bmp_top;
    unsigned char* ft_bmp_buffer;

    int bmp_i;
    int bmp_j;
    int bmp_valoff;
    int img_x;
    int img_y;
    int img_channel;
    unsigned int bmp_val;
    float bmp_valf;
    CvScalar img_pixel;

    double whitespace_width;
    double interval_width;
    double horizontal_offset;

    //
    //IplImage.origin == 0 coordinate
    //  0------+ x
    //  |
    //  +
    //  y
    //
    //IplImage.origin != 0 coordinate
    //  y
    //  +
    //  |
    //  0------+ x
    //
    //
    //freetype bmp coordinate
    //  y
    //  +
    //  |
    //  0------+ x
    //
    //freetype algin
    //  'a'
    //               -+-        -+-
    //                |          |
    //                |height    |top
    //                |          |
    //  -baseline-:  -+-        -+-
    //
    //  'g'
    //               -+-        -+-
    //                |          |
    //                |          |top
    //                |          |
    //  -baseline-:   |height   -+-
    //                |
    //               -+-
    //
    int loc_x;//font left
    int loc_y;//font bottom

    //get font_id from database of char
    ft_ascender   = font->m_face->size->metrics.ascender/64;

    _cvRenderChar(font, wc);
    ft_slot         = font->m_face->glyph;
    ft_bmp_width    = ft_slot->bitmap.width;
    ft_bmp_height   = ft_slot->bitmap.rows;
    ft_bmp_step     = ft_slot->bitmap.pitch;
    ft_bmp_buffer   = ft_slot->bitmap.buffer;
    ft_bmp_left     = ft_slot->bitmap_left;
    ft_bmp_top      = ft_slot->bitmap_top;

#ifdef CVUNITEXT_DEBUG
    if (wc < 256) {
        printf(" %c: ", ((char*)&wc)[0]);
    } else {
        printf(" 0x%02x%02x: ", ((char*)&wc)[1] & 0xFF, ((char*)&wc)[0] & 0xFF);
    }
    printf("width %4d, height %4d, ", ft_bmp_width,  ft_bmp_height);
    printf("left %4d, ", ft_bmp_left);
    printf("top %4d, ", ft_bmp_top);
    printf("\n");
#endif

    //calculate char width
    whitespace_width = font->m_fontSize.val[0] * font->m_fontSize.val[1];
    interval_width   = font->m_fontSize.val[0] * font->m_fontSize.val[2];
    if (ft_bmp_width != 0) {
        horizontal_offset = ft_bmp_width + interval_width;
    } else {
        horizontal_offset = whitespace_width + interval_width;
    }

    loc_x = pos.x + ft_bmp_left;
    loc_y = pos.y + ft_bmp_height - ft_bmp_top;
    
	if (calc) {
		return horizontal_offset;
	}

    //draw font bitmap to opencv image
    //(bmp_j,bmp_i) is freetype bitmap location
    for (bmp_i = 0; bmp_i < ft_bmp_height; ++bmp_i) {
        for (bmp_j = 0; bmp_j < ft_bmp_width; ++bmp_j) {
            if (img->origin == 0) {//normal
                bmp_valoff = bmp_i * ft_bmp_step + bmp_j;
            } else {//upsidedown
                bmp_valoff = (ft_bmp_height-1 - bmp_i) * ft_bmp_step + bmp_j;
            }

            bmp_val  = ft_bmp_buffer[bmp_valoff];
            bmp_valf = (float)bmp_val/255;
            if (bmp_val == 0) {
                continue;
            }

            //(img_x,img_y) is opencv bitmap location
            if (img->origin == 0) {//normal
                img_y = loc_y - (ft_bmp_height-1-bmp_i);
            } else {//upsidedown
                img_y = loc_y + bmp_i;
            }
            img_x = loc_x + bmp_j;

            //update pixel when location is valid
            //  alpha = font_bitmap_val / 255;
            //  pixel = alpha * color + (1 - alpha) * pixel;
            if ((0 <= img_y) && (img_y < img->height) && (0 <= img_x) && (img_x < img->width)) {
                img_pixel = cvGet2D(img, img_y, img_x);
                for (img_channel = 0;img_channel < img->nChannels; img_channel++) {
                    img_pixel.val[img_channel] = 
                        (1-bmp_valf) * img_pixel.val[img_channel] + 
                        bmp_valf * color.val[img_channel];
                }
                cvSet2D(img, img_y, img_x, img_pixel);
            }
        }
    }

    return horizontal_offset;
}

CvRect _cvPutUniTextUCS2(IplImage* img, const wchar_t* text,
        CvPoint org, CvUniFont* font, CvScalar color, int calc)
{
	int i;
	int n;
	CvPoint pt0;
	CvPoint pt1;
    double offset;

    CvRect rect;
    int ascender;
    int descender;

	pt0 = org;
    pt1 = org;

    ascender  = font->m_face->size->metrics.ascender/64;
    descender = font->m_face->size->metrics.descender/64;

	n = wcslen(text);

	for (i = 0;i < n;i++) {
		offset = _cvPutUniChar(img, text[i], pt1, font, color, calc);
        pt1.x += (int)offset;
	}

    rect.width  = pt1.x - pt0.x;
    rect.height = ascender - descender;
    rect.x      = pt0.x;
    rect.y      = pt0.y - descender;

	return rect;
}

CvRect cvPutUniText(IplImage* img, const char* text,
        CvPoint org, CvUniFont* font, CvScalar color, const char* encoding, int calc)
{
	wchar_t* ws;
    int outlen;
    CvRect sz;

    ws = (wchar_t*)libcodec_iconv(text, strlen(text), &outlen, encoding, "UCS-2LE");
    sz = _cvPutUniTextUCS2(img, ws, org, font, color, calc);
    free(ws);

    return sz;
}

