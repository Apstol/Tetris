#pragma once

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

class GameOverAnimation : public Animation
{
	public:
		GameOverAnimation(const sf::Texture& texture);
		
		void update(sf::Time elapsedTime);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		sf::IntRect mTextureRect;
		const sf::Texture& mSpritesheet;
};
