#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <gio/gio.h>

#include "cgif.h"

#define WIDTH  100
#define HEIGHT 100

/* This is an example code that creates a GIF-image with all green pixels. */
int main(void) {
  GIF*         pGIF;
  GIFConfig     gConfig;
  FrameConfig   fConfig;
  uint8_t*      pImageData;
  uint8_t*      aPalette;
  uint16_t      numColors         = 256; // number of colors in aPalette  
  //
  // create an image
  aPalette   = malloc(256 * 3);
  memset(aPalette, 0, 256 * 3);
  pImageData = malloc(WIDTH * HEIGHT);   // actual image data
  memset(pImageData, 0, WIDTH * HEIGHT);
  //
  // create new GIF
  memset(&gConfig, 0, sizeof(GIFConfig));
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.pGlobalPalette          = aPalette;
  gConfig.numGlobalPaletteEntries = numColors;
  gConfig.path                    = "max_color_table_test.gif";
  GFile *file = g_file_new_for_path("max_color_table_test.gif");
  GOutputStream *outputStream = (GOutputStream*) g_file_replace(file, NULL, FALSE, G_FILE_CREATE_PRIVATE, NULL, NULL);
  gConfig.outputStream = outputStream;
  pGIF = cgif_newgif(&gConfig);  
  //
  // add frames to GIF
  memset(&fConfig, 0, sizeof(FrameConfig));
  fConfig.pImageData = pImageData;
  cgif_addframe(pGIF, &fConfig);
  free(pImageData);  
  free(aPalette);
  //
  // free allocated space at the end of the session
  cgif_close(pGIF);  
  g_output_stream_close(outputStream, NULL, NULL);
  g_object_unref(outputStream);
  g_object_unref(file);
  return 0;
}
