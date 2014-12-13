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

#include "root_container.h"
#include "../gui_manager.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	namespace Gui
	{
		const BoundingBox& RootContainer::get_bounding_box() 
		{
			static BoundingBox ret(Pointi::ZERO, Pointi::ZERO);
			ret.w = m_manager->renderer()->get_screen_size().x;
			ret.h = m_manager->renderer()->get_screen_size().y;
			return ret;
		}

		bool RootContainer::handle_mouse_move(const Pointi& mouse_pos)
		{
			// return if reacted to this event
			bool reacted = false;

			// loop over all the sub entities
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				// get current element and check if mouse is inside bounderies
				GuiElementPtr& curr = m_sons[i];

				// skip if disabled or invisible
				if (!curr->is_enabled() || !curr->is_visible())
				{
					continue;
				}

				if (curr->is_point_on(mouse_pos))
				{
					reacted = true;

					// invoke mouse enter event
					if (!curr->is_mouse_on())
					{
						curr->invoke_event_mouse_enter(mouse_pos);
					}

					// invoke mouse hover event
					curr->invoke_event_mouse_hover(mouse_pos);
				}
				// if mouse is not on this element
				else
				{
					// invoke mouse leave event
					if (curr->is_mouse_on())
					{
						curr->invoke_event_mouse_leave(mouse_pos);
					}
				}
			}

			return reacted;
		}
	}
}