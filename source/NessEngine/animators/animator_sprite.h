/**
* Animator that animates sprite from spritesheet
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "animator_api.h"
#include "../renderer/renderer.h"

namespace Ness
{
	namespace Animators
	{
		// sprite from spritesheet animation
		class AnimatorSprite : public AnimatorAPI
		{
		private:
			Sizei m_spritesheet_total_steps;		// total steps (x and y) of the sprite sheet

		public:
			// if fadeIn = true, will increase opacity of object and remove animator once reach 1.0
			// if false, will reduce opacity until 0.0f
			AnimatorFader(bool fadeIn, float fadeSpeed = 1.0f) : m_fade_in(fadeIn), m_speed(fadeSpeed) {}

			NESSENGINE_API virtual void set_target(RenderablePtr object)
			{
				m_target = object;
			}

			NESSENGINE_API virtual void animate(Renderer* renderer)
			{
				if (m_fade_in)
				{
					m_target->set_opacity(m_target->get_opacity() + renderer->time_factor() * m_speed);
					if (m_target->get_opacity() >= 1.0f)
					{
						m_target->set_opacity(1.0f);
						this->destroy();
					}
				}
				if (!m_fade_in)
				{
					m_target->set_opacity(m_target->get_opacity() - renderer->time_factor() * m_speed);
					if (m_target->get_opacity() <= 0.0f)
					{
						m_target->set_opacity(0.0f);
						this->destroy();
					}
				}
			}
		};

	};
};