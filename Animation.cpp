#include "Animation.hpp"

Animation::Animation(int numFrames, int currentFrame, sf::Time elapsedTime, sf::Time duration)
: mNumFrames(numFrames)
, mCurrentFrame(currentFrame)
, mElapsedTime(elapsedTime)
, mDuration(duration)
{
}

Animation::~Animation()
{
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

void Animation::restart()
{
	mCurrentFrame = 0;
}
