LIBOPENCV_PATH 	 = sdk/opencv_sdk

LIBICONV_PATH 	 = ../libiconv
LIBFREETYPE_PATH = ../libfreetype

CFLAGS = /nologo /c /W3 /WX \
		 /I . \
		 /I $(LIBOPENCV_PATH)/include \
		 /I $(LIBFREETYPE_PATH)/include \
		 /I $(LIBICONV_PATH)

LFLAGS = /nologo \
		 /libpath:$(LIBOPENCV_PATH)/staticlib \
		 /libpath:$(LIBFREETYPE_PATH) \
		 /libpath:$(LIBICONV_PATH)

LLIBS  = \
	User32.lib AdvAPI32.lib Comctl32.lib Gdi32.lib vfw32.lib Ole32.lib OleAut32.lib \
	ilmimf.lib libjasper.lib libjpeg.lib libpng.lib libtiff.lib \
	opencv_calib3d243.lib opencv_contrib243.lib opencv_core243.lib \
	opencv_features2d243.lib opencv_flann243.lib opencv_gpu243.lib \
	opencv_haartraining_engine.lib opencv_highgui243.lib opencv_imgproc243.lib \
	opencv_legacy243.lib opencv_ml243.lib opencv_nonfree243.lib \
	opencv_objdetect243.lib opencv_photo243.lib opencv_stitching243.lib \
	opencv_ts243.lib opencv_video243.lib opencv_videostab243.lib \
	zlib.lib iconv.lib freetype.lib 


all:test.exe

test.exe:./test.obj ./cvUniText.obj ./cvUtils.obj
	cl $(CFLAGS) $(LIBICONV_PATH)/libcodec.c
	link $(LFLAGS) \
		./test.obj ./libcodec.obj \
		./cvUniText.obj ./cvUtils.obj \
		$(LLIBS) /out:test.exe


{.}obj{.}c:
	cl $(CFLAGS) $<

clean:
	rm -rf *.obj
	rm -rf *.exe


