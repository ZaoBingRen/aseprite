/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2011  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include "commands/command.h"
#include "app.h"
#include "modules/gui.h"
#include "raster/sprite.h"
#include "undoable.h"
#include "widgets/color_bar.h"
#include "sprite_wrappers.h"
#include "app/color_utils.h"

//////////////////////////////////////////////////////////////////////
// flatten_layers

class FlattenLayersCommand : public Command
{
public:
  FlattenLayersCommand();
  Command* clone() { return new FlattenLayersCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

FlattenLayersCommand::FlattenLayersCommand()
  : Command("flatten_layers",
	    "Flatten Layers",
	    CmdUIOnlyFlag)
{
}

bool FlattenLayersCommand::onEnabled(Context* context)
{
  const CurrentSpriteReader sprite(context);
  return sprite != NULL;
}

void FlattenLayersCommand::onExecute(Context* context)
{
  CurrentSpriteWriter sprite(context);
  int bgcolor = color_utils::color_for_image(app_get_colorbar()->getBgColor(), sprite->getImgType());
  {
    Undoable undoable(sprite, "Flatten Layers");
    undoable.flattenLayers(bgcolor);
    undoable.commit();
  }
  update_screen_for_sprite(sprite);
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_flatten_layers_command()
{
  return new FlattenLayersCommand;
}
