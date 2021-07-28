#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <gio/gio.h>

#include "cgif.h"

#define WIDTH  103
#define HEIGHT 104

/* This is an example code that creates a GIF-animation with a moving stripe-pattern. */
int main(void) {
  GIF*         pGIF;
  GIFConfig     gConfig;
  FrameConfig   fConfig;
  uint8_t*      pImageData;
  uint8_t aPalette[] = {
    0x00, 0x00, 0x00, // black
    0xFF, 0xFF, 0xFF, // white
  };
  uint8_t numColors = 2;   // number of colors in aPalette
  //
  // Create new GIF
  //
  memset(&gConfig, 0, sizeof(GIFConfig));
  memset(&fConfig, 0, sizeof(FrameConfig));
  gConfig.attrFlags               = GIF_ATTR_NO_GLOBAL_TABLE;
  gConfig.width                   = WIDTH;
  gConfig.height                  = HEIGHT;
  gConfig.path                    = "only_local_table.gif";
  GFile *file = g_file_new_for_path("only_local_table.gif");
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
  fConfig.pLocalPalette = aPalette;
  fConfig.numLocalPaletteEntries = numColors;
  fConfig.attrFlags = FRAME_ATTR_USE_LOCAL_TABLE;
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
