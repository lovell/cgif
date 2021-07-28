#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <gio/gio.h>

#include "cgif.h"

#define WIDTH  110
#define HEIGHT 125

/* This is an example code that creates a GIF-animation with a moving stripe-pattern. */
int main(void) {
  GIF*         pGIF;
  GIFConfig     gConfig;
  FrameConfig   fConfig;
  uint8_t*      pImageData;
  uint8_t aLocalPalette[] = {
    0x00, 0x00, 0x00, // black
    0xFF, 0xFF, 0xFF, // white
  };
  uint8_t aGlobalPalette[] = {
    0x00, 0xFF, 0x00, // green
    0x00, 0x00, 0xFF, // blue
    0xFF, 0x00, 0x00, // red
  };
  uint8_t numColorsLocal = 2;   // number of colors in aPalette
  uint8_t numColorsGlobal = 3;   // number of colors in aPalette
  //
  // Create new GIF
  //
  memset(&gConfig, 0, sizeof(GIFConfig));
  memset(&fConfig, 0, sizeof(FrameConfig));
  gConfig.pGlobalPalette               = aGlobalPalette;
  gConfig.numGlobalPaletteEntries = numColorsGlobal;
  gConfig.attrFlags               = GIF_ATTR_IS_ANIMATED;
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.path                    = "global_plus_local_table.gif";
  GFile *file = g_file_new_for_path("global_plus_local_table.gif");
  GOutputStream *outputStream = (GOutputStream*) g_file_replace(file, NULL, FALSE, G_FILE_CREATE_PRIVATE, NULL, NULL);
  gConfig.outputStream = outputStream;
  pGIF = cgif_newgif(&gConfig);
  //
  // Add frame to GIF
  //
  pImageData = malloc(WIDTH * HEIGHT);         // Actual image data
  memset(pImageData, 1, WIDTH * HEIGHT);
  memset(pImageData + WIDTH * 2, 0, WIDTH * 3); 
  fConfig.pImageData = pImageData;
  fConfig.pLocalPalette = aLocalPalette;
  fConfig.numLocalPaletteEntries = numColorsLocal;
  fConfig.attrFlags = FRAME_ATTR_USE_LOCAL_TABLE;
  fConfig.delay = 100;
  cgif_addframe(pGIF, &fConfig); // append the new frame
  //
  // add next frame
  fConfig.attrFlags = 0;
  fConfig.genFlags = FRAME_GEN_USE_TRANSPARENCY;
  memset(pImageData + WIDTH * 9, 2, WIDTH * 10);
  cgif_addframe(pGIF, &fConfig); // append the new frame
  //
  free(pImageData);
  //
  // Free allocated space at the end of the session
  //
  cgif_close(pGIF);
  g_output_stream_close(outputStream, NULL, NULL);
  g_object_unref(outputStream);
  g_object_unref(file);
  return 0;
}
