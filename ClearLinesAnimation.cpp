#include "ClearLinesAnimation.hpp"
#include "Playfield.hpp"

ClearLinesAnimation::ClearLinesAnimation(Playfield& playfield)
: Animation(Playfield::Columns, 0, sf::Time::Zero, sf::seconds(1.f))
, mPlayfield(playfield)
{
}

void ClearLinesAnimation::update(sf::Time deltaTime)
{
	sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);

	mElapsedTime += deltaTime;

	while (mElapsedTime >= timePerFrame && mCurrentFrame < mNumFrames)
	{
		mElapsedTime -= timePerFrame;
		++mCurrentFrame;
	}
}

void ClearLinesAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(20, 20));
	rectangle.setFillColor(sf::Color::Black);

	for (int row : mPlayfield.linesToClear())
	{
		for (int column = 0; column < mCurrentFrame; ++column)
		{
			rectangle.setPosition(sf::Vector2f(column, row) * 20.f + sf::Vector2f(400.f, 300.f) - sf::Vector2f(10.f * 20.f / 2, 20.f * 20.f / 2));
			target.draw(rectangle, states);
		}
	}
}
