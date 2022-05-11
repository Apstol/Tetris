#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Animation : public sf::Drawable
{
	public:
		Animation(int numFrames, int currentFrame, sf::Time elapsedTime, sf::Time duration);
		virtual ~Animation();

		virtual void update(sf::Time elapsedTime) = 0;
		bool isFinished() const;
		void restart();
	
	protected:
		int mNumFrames;
		int mCurrentFrame;
		sf::Time mElapsedTime;
		sf::Time mDuration;
};
