/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

/**
* A label widget
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "gui_widget_api.h"
#include "../../renderable/entities/text.h"

namespace Ness
{
	namespace Gui
	{

		// a label widget TBD!!!
		class Label : public WidgetAPI
		{
		protected:
			TextPtr		m_text;		// the text entity that is the label
			Point		m_position;	// position in pixels relative to parent

		public:

			NESSENGINE_API Label(GuiManager* manager, GuiContainerAPI* parent, const String& text);

			// set container position, relative to parent, in pixels
			NESSENGINE_API virtual void set_position(const Point& new_pos, const Point& anchor = Point::ZERO);

			// recalculate text position when moved
			NESSENGINE_API virtual void __invoke_event_update_position();
			
			// render the label
			NESSENGINE_API virtual void render();

			// return label bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() const;


		};

		// a pointer to a label widget
		NESSENGINE_API typedef SharedPtr<Label> LabelPtr;
	}
}