#pragma once

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

class Playfield;

class ClearLinesAnimation : public Animation
{
	public:
		ClearLinesAnimation(Playfield& playfield);
		
		void update(sf::Time elapsedTime);

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		Playfield& mPlayfield;
};
