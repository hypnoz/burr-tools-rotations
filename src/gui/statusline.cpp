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
#include "statusline.h"

#include "buttongroup.h"
#include "configuration.h"

LStatusLine::LStatusLine(int x, int y, int w, int h) : layouter_c(x, y, w, h), colorModeIndex(0) {

  weight(1, 0);
  box(FL_FLAT_BOX);
  color(FL_BACKGROUND_COLOR);

  text = new LFl_Box(0, 0, 1, 1);
  text->box(FL_NO_BOX);
  text->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  text->weight(1, 0);
  text->pitch(4);

  Fl_Button * b;

  rstyle = new ButtonGroup_c(1, 0, 1, 1);

  b = rstyle->addButton();
  b->image(pm.get(RenderModeVoxel_xpm));
  b->tooltip(" Draw each voxel separately ");

  b = rstyle->addButton();
  b->image(pm.get(RenderModeEdges_xpm));
  b->tooltip(" Draw flat faces with lines at the real edges ");

  b = rstyle->addButton();
  b->image(pm.get(RenderModeSTL_xpm));
  b->tooltip(" Draw pieces like the STL export produces them ");

  rstyle->select(config.renderStyle());

#ifdef __APPLE__
  (new LFl_Box(0, 2, 0, 1, 1))->setMinimumSize(20, 0);
#endif

  clear_visible_focus();

  end();
}

void LStatusLine::setText(const char * t) {

  text->copy_label(t);
}

void LStatusLine::setColorModeIndex(int i) {
  colorModeIndex = i;
}

voxelFrame_c::colorMode LStatusLine::getColorMode(void) const {

  switch (colorModeIndex) {
    case 0: return voxelFrame_c::pieceColor;
    case 1: return voxelFrame_c::paletteColor;
    case 2: return voxelFrame_c::anaglyphColor;
    case 3: return voxelFrame_c::anaglyphColorL;
    default: return voxelFrame_c::pieceColor;
  }
}

voxelFrame_c::renderStyle LStatusLine::getRenderStyle(void) const {

  switch (rstyle->getSelected()) {
    case 0: return voxelFrame_c::styleVoxel;
    case 1: return voxelFrame_c::styleEdges;
    case 2: return voxelFrame_c::styleSTL;
    default: return voxelFrame_c::styleVoxel;
  }
}

void LStatusLine::callback(Fl_Callback* fkt, void * dat) {
  rstyle->callback(fkt, dat);
}
