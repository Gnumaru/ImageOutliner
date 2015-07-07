# the preprocessor flag "-Dcimg_display=0" prevents cimg from including x11 headers, since we do not want to exibhit any kind of image.
# the preprocessor flag "-Dcimg_use_png" tells cimg to load pngs using libpng, so we can effectively load the image's alpha channel (we do need the alpha channel)
AM_CPPFLAGS=-Dcimg_use_png -Dcimg_display=0
bin_PROGRAMS=image_outliner
image_outliner_SOURCES=main.cpp ImageOutliner.cpp CommandLineOptionsBoost.cpp
