#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <gio/gio.h>

#include "cgif.h"

#define WIDTH  100
#define HEIGHT 100

static void initGIFConfig(GIFConfig* pConfig, uint8_t* pGlobalPalette, uint16_t numColors, uint32_t attrFlags, uint16_t width, uint16_t height) {
  memset(pConfig, 0, sizeof(GIFConfig));
  pConfig->attrFlags               = attrFlags;
  pConfig->width                   = width;
  pConfig->height                  = height;
  pConfig->pGlobalPalette          = pGlobalPalette;
  pConfig->numGlobalPaletteEntries = numColors;
  pConfig->path                   = "all_optim.gif";
}

static void initFrameConfig(FrameConfig* pConfig, uint8_t* pImageData, uint16_t delay, uint32_t genFlags) {
  memset(pConfig, 0, sizeof(FrameConfig));
  pConfig->pImageData = pImageData;
  pConfig->delay      = delay;
  pConfig->genFlags   = genFlags;
}

int main(void) {
  GIF*          pGIF;
  GIFConfig      gConfig;
  FrameConfig    fConfig;
  uint8_t*      pImageData;
  uint8_t aPalette[] = {
    0x00, 0x00, 0x00, // black
    0xFF, 0xFF, 0xFF, // white
  };
  uint8_t numColors = 2;   // number of colors in aPalette
  int numFrames     = 2;      // number of frames in the video
  //
  // create new GIF
  initGIFConfig(&gConfig, aPalette, numColors, GIF_ATTR_IS_ANIMATED, WIDTH, HEIGHT);
  GFile *file = g_file_new_for_path("all_optim.gif");
  GOutputStream *outputStream = (GOutputStream*) g_file_replace(file, NULL, FALSE, G_FILE_CREATE_PRIVATE, NULL, NULL);
  gConfig.outputStream = outputStream;
  pGIF = cgif_newgif(&gConfig);
  //
  // add frames to GIF
  pImageData = malloc(WIDTH * HEIGHT);         // Actual image data
  memset(pImageData, 0, WIDTH * HEIGHT);
  initFrameConfig(&fConfig, pImageData, 100, FRAME_GEN_USE_TRANSPARENCY | FRAME_GEN_USE_DIFF_WINDOW);
  cgif_addframe(pGIF, &fConfig); // append the new frame
  memset(pImageData + 7, 1, 41);
  memset(pImageData + 7 + WIDTH, 1, 41);
  memset(pImageData + 7 + WIDTH * 2, 1, 41);
  memset(pImageData + 7 + WIDTH * 2, 0, 29);
  memset(pImageData + 7 + WIDTH * 3, 1, 41);
  memset(pImageData + 7 + WIDTH * 3, 0, 23);
  memset(pImageData + 7 + WIDTH * 4, 0, 37);
  cgif_addframe(pGIF, &fConfig); // append the next frame
  //
  free(pImageData);
  //
  // Free allocated space at the end of the session
  cgif_close(pGIF);
  g_output_stream_close(outputStream, NULL, NULL);
  g_object_unref(outputStream);
  g_object_unref(file);
  return 0;
}
