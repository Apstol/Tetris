#include "GameOverAnimation.hpp"
#include "Playfield.hpp"

GameOverAnimation::GameOverAnimation(const sf::Texture& texture)
: Animation(Playfield::Rows, 0, sf::Time::Zero, sf::seconds(2.f))
, mSpritesheet(texture)
, mTextureRect(sf::IntRect(232, 0, 116, 116))
{
}

void GameOverAnimation::update(sf::Time deltaTime)
{
	sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);

	mElapsedTime += deltaTime;

	while (mElapsedTime >= timePerFrame && mCurrentFrame < mNumFrames)
	{
		mElapsedTime -= timePerFrame;
		++mCurrentFrame;
	}
}

void GameOverAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite sprite;
	sprite.setTexture(mSpritesheet);
	sprite.setTextureRect(mTextureRect);
	// TODO: pass size to tetromino constructor
	sprite.setScale(sf::Vector2f(20.f, 20.f) / 116.f);

	for (int row = 0; row < mCurrentFrame; ++row)
	{
		for (int column = 0; column < Playfield::Columns; ++column)
		{
			sprite.setPosition(sf::Vector2f(column, row) * 20.f + sf::Vector2f(400.f, 300.f) - sf::Vector2f(10.f * 20.f / 2, 20.f * 20.f / 2));
			target.draw(sprite, states);
		}
	}
}
