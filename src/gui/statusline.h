/* BurrTools
 *
 * BurrTools is the legal property of its developers, whose
 * names are listed in the COPYRIGHT file, which is included
 * within the source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */
#ifndef __STATUS_LINE_H__
#define __STATUS_LINE_H__

#include "Layouter.h"
#include "voxelframe.h"

// Status text. Colour mode and render style are chosen from the View menu.
class LStatusLine : public layouter_c {

private:

  LFl_Box * text;
  int colorModeIndex;
  int renderStyleIndex;

public:

  LStatusLine(int x, int y, int w, int h);

  void setText(const char * t);
  voxelFrame_c::colorMode getColorMode(void) const;
  void setColorModeIndex(int i);
  int getColorModeIndex(void) const { return colorModeIndex; }
  voxelFrame_c::renderStyle getRenderStyle(void) const;
  void setRenderStyleIndex(int i);
  int getRenderStyleIndex(void) const { return renderStyleIndex; }

  virtual void getMinSize(int *width, int *height) const {
    *width = 30;
    *height = 25;
  }
};

#endif
