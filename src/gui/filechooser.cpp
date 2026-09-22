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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "filechooser.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <string.h>

static char bt_file_chooser_result[4096];

/* While the native dialog is up the window behind it may be dimmed by the
 * window system. An Fl_Gl_Window does not receive that dim and paints it
 * itself (see platform::modalDimWash), but only when it is asked to
 * redraw -- and nothing damages it meanwhile, because FLTK is just pumping
 * Fl::wait() inside the dialog. So drive the redraw for the duration.
 */
static const double GL_REDRAW_INTERVAL = 0.1;

static void redrawGlViews(Fl_Widget * w)
{
  if (dynamic_cast<Fl_Gl_Window *>(w))
  {
    w->redraw();
    return;
  }

  if (Fl_Group * g = dynamic_cast<Fl_Group *>(w))
    for (int i = 0; i < g->children(); i++)
      redrawGlViews(g->child(i));
}

static void redrawAllGlViews(void)
{
  for (Fl_Window * w = Fl::first_window(); w; w = Fl::next_window(w))
    redrawGlViews(w);
}

static void glRedrawPump(void *)
{
  redrawAllGlViews();
  Fl::repeat_timeout(GL_REDRAW_INTERVAL, glRedrawPump);
}

static const char * run_file_chooser(const char *title, const char *pattern, const char *preset, int type)
{
  bt_file_chooser_result[0] = 0;

  Fl_Native_File_Chooser chooser;
  chooser.title(title ? title : "");
  chooser.type(type);

  if (pattern && pattern[0])
    chooser.filter(pattern);

  if (preset && preset[0])
    chooser.preset_file(preset);

  Fl::add_timeout(GL_REDRAW_INTERVAL, glRedrawPump);
  const int shown = chooser.show();
  Fl::remove_timeout(glRedrawPump);
  redrawAllGlViews();

  if (shown != 0)
    return 0;

  const char * filename = chooser.filename();
  if (!filename || !filename[0])
    return 0;

  strncpy(bt_file_chooser_result, filename, sizeof(bt_file_chooser_result) - 1);
  bt_file_chooser_result[sizeof(bt_file_chooser_result) - 1] = 0;
  return bt_file_chooser_result;
}

const char * bt_file_chooser_open(const char *title, const char *pattern, const char *preset)
{
  return run_file_chooser(title, pattern, preset, Fl_Native_File_Chooser::BROWSE_FILE);
}

const char * bt_file_chooser_save(const char *title, const char *pattern, const char *preset)
{
  return run_file_chooser(title, pattern, preset, Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
}
