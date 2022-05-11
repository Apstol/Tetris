#include <cmath>

#include "Tetromino.hpp"
#include "Utility.hpp"

const sf::Vector2f Tetromino::Size = sf::Vector2f(20.f, 20.f);
const sf::Vector2f Tetromino::Scale = Tetromino::Size / 116.f;

const sf::Time Tetromino::ManualDropPeriod = sf::seconds(0.05f);

Tetromino::Tetromino(TetrominoTypes::Type type, const sf::Texture& texture, const Playfield& playfield, SoundPlayer& soundPlayer)
: mType(type)
, mPlayfield(playfield)
, mSpritesheet(texture)
, mAutoDropPeriod(sf::seconds(1.f))
, mTimeSinceLastAutoDrop(sf::Time::Zero)
, mTimeSinceLastManualDrop(sf::Time::Zero)
, mVelocity(sf::Vector2f(0.f, 0.f))
, mToggledRotation(false)
, mIsLocked(false)
, mIsLocking(false)
, mSoundPlayer(soundPlayer)
{
	resetLockCountdown();
	createShape();
}

sf::IntRect Tetromino::textureRectByType(TetrominoTypes::Type type)
{
	switch(type)
	{
		case TetrominoTypes::Type::I:
			return sf::IntRect(232, 0, 116, 116);
			break;
		case TetrominoTypes::Type::J:
			return sf::IntRect(0, 0, 116, 116);
			break;
		case TetrominoTypes::Type::L:
			return sf::IntRect(116, 0, 116, 116);
			break;
		case TetrominoTypes::Type::O:
			return sf::IntRect(348, 0, 116, 116);
			break;
		case TetrominoTypes::Type::T:
			return sf::IntRect(464, 0, 116, 116);
			break;
		case TetrominoTypes::Type::S:
			return sf::IntRect(580, 0, 116, 116);
			break;
		case TetrominoTypes::Type::Z:
			return sf::IntRect(696, 0, 116, 116);
			break;
		default:
			return sf::IntRect(0, 0, 0, 0);
			break;
	}
}

void Tetromino::update(sf::Time elapsedTime)
{
	if (mIsLocked)
	{
		return;
	}

	if (mIsLocking)
	{
		mLockCountdown -= elapsedTime;
		if (mLockCountdown <= sf::seconds(0.f))
		{
			mIsLocked = true;
			resetLockCountdown();
		}
	}
}

void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite sprite(mSpritesheet, Tetromino::textureRectByType(mType));
	sprite.setScale(Scale);

	for (const sf::Vector2f& blockPosition : mShapeData)
	{
		sf::Vector2f positionOffset = sf::Vector2f(Game::ScreenWidth / 2, Game::ScreenHeight / 2) - sf::Vector2f(Playfield::Columns / 2 * Size.x, Playfield::Rows / 2 * Size.x);
		sprite.setPosition(relativePosition(blockPosition) * Size.x + positionOffset);
		target.draw(sprite, states);
	}
}

bool Tetromino::move(sf::Vector2f direction, bool isHardDrop)
{
	if (mIsLocked)
	{
		return false;
	}

	mVelocity = direction;

	mAbsolutePosition += mVelocity;

	if (isCollidingWithBottomBorder() 
	    || (!canMoveSideways() && !canMoveDown())
	    || (isCollidingWithPlayfield() && isHardDrop))
	{
		mAbsolutePosition -= mVelocity;
		mIsLocked = true;
		mSoundPlayer.play(SoundPlayer::SoundEffect::Drop);
		return false;
	}
	else if (isCollidingWithPlayfield() 
		&& !isCollidingWithBottomBorder()
		&& canMoveSideways() 
		&& !canMoveDown())
	{
		mAbsolutePosition -= mVelocity;
		mIsLocking = true;
		return false;
	}
	else if (isCollidingWithPlayfield() || isCollidingWithBorders())
	{
		mAbsolutePosition -= mVelocity;
		return false;
	}

	return true;
}


// Game Boy rotation system
void Tetromino::rotate()
{
	if (mType == TetrominoTypes::Type::O)
	{
		return;
	}

	TetrominoShapeData blockPositionsAfterRotation;
	float angle = 90.f;

	// rotation fix for I type tetromino
	if (mType == TetrominoTypes::Type::I)
	{
		angle = mToggledRotation ? -90.f : 90.f;
	}

	for (std::size_t i = 0; i < mShapeData.size(); ++i)
	{
		sf::Vector2f blockPosition = sf::Vector2f(-mShapeData[i].y, mShapeData[i].x);

		if (isCollidingWithBorders(blockPosition) || isCollidingWithPlayfield(blockPosition))
		{
			return;
		}

		blockPositionsAfterRotation[i] = blockPosition;
	}

	mShapeData = blockPositionsAfterRotation;
	mToggledRotation = !mToggledRotation; // rotation fix for I type tetromino
	mSoundPlayer.play(SoundPlayer::SoundEffect::Rotation);
}

Tetromino::TetrominoShapeData& Tetromino::shapeData()
{
	return mShapeData;
}

TetrominoTypes::Type Tetromino::type() const
{
	return mType;
}

bool Tetromino::isLocked() const
{
	return mIsLocked;
}

void Tetromino::hardDrop()
{
	while (move(sf::Vector2f(0.f, 1.f), true))
	{
	}
}

sf::Vector2f Tetromino::relativePosition(const sf::Vector2f& blockPosition) const
{
	return blockPosition + mAbsolutePosition + mTransformOrigin;
}

void Tetromino::createShape()
{
	switch(mType)
	{
		case TetrominoTypes::Type::I:
			mShapeData[0] = sf::Vector2f(0.f, -2.f);
			mShapeData[1] = sf::Vector2f(0.f, -1.f);
			mShapeData[2] = sf::Vector2f(0.f, 0.f);
			mShapeData[3] = sf::Vector2f(0.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 2.f);
			mAbsolutePosition = sf::Vector2f(4.f, 0.f);
			break;
		case TetrominoTypes::Type::J:
			mShapeData[0] = sf::Vector2f(0.f, -1.f);
			mShapeData[1] = sf::Vector2f(0.f, 0.f);
			mShapeData[2] = sf::Vector2f(0.f, 1.f);
			mShapeData[3] = sf::Vector2f(-1.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 1.f);
			mAbsolutePosition = sf::Vector2f(4.f, 0.f);
			break;
		case TetrominoTypes::Type::L:
			mShapeData[0] = sf::Vector2f(0.f, -1.f);
			mShapeData[1] = sf::Vector2f(0.f, 0.f);
			mShapeData[2] = sf::Vector2f(0.f, 1.f);
			mShapeData[3] = sf::Vector2f(1.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 1.f);
			mAbsolutePosition = sf::Vector2f(4.f, 0.f);
			break;
		case TetrominoTypes::Type::O:
			mShapeData[0] = sf::Vector2f(0.f, 0.f);
			mShapeData[1] = sf::Vector2f(1.f, 0.f);
			mShapeData[2] = sf::Vector2f(0.f, 1.f);
			mShapeData[3] = sf::Vector2f(1.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 0.f);
			mAbsolutePosition = sf::Vector2f(4.f, 0.f);
			break;
		case TetrominoTypes::Type::T:
			mShapeData[0] = sf::Vector2f(-1.f, 0.f);
			mShapeData[1] = sf::Vector2f(0.f, 0.f);
			mShapeData[2] = sf::Vector2f(1.f, 0.f);
			mShapeData[3] = sf::Vector2f(0.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 1.f);
			mAbsolutePosition = sf::Vector2f(4.f, -1.f);
			break;
		case TetrominoTypes::Type::S:
			mShapeData[0] = sf::Vector2f(-1.f, 1.f);
			mShapeData[1] = sf::Vector2f(0.f, 1.f);
			mShapeData[2] = sf::Vector2f(0.f, 0.f);
			mShapeData[3] = sf::Vector2f(1.f, 0.f);
			mTransformOrigin = sf::Vector2f(0.f, 2.f);
			mAbsolutePosition = sf::Vector2f(4.f, -2.f);
			break;
		case TetrominoTypes::Type::Z:
			mShapeData[0] = sf::Vector2f(-1.f, 0.f);
			mShapeData[1] = sf::Vector2f(0.f, 0.f);
			mShapeData[2] = sf::Vector2f(0.f, 1.f);
			mShapeData[3] = sf::Vector2f(1.f, 1.f);
			mTransformOrigin = sf::Vector2f(0.f, 1.f);
			mAbsolutePosition = sf::Vector2f(4.f, -1.f);
			break;
	}
}

bool Tetromino::isCollidingWithBorders() const
{
	for (const sf::Vector2f& blockPosition : mShapeData)
	{
		if (isCollidingWithBorders(blockPosition))
		{
			return true;
		}
	}

	return false;
}

bool Tetromino::isCollidingWithBottomBorder() const
{
	for (const sf::Vector2f& blockPosition : mShapeData)
	{
		sf::Vector2f relPos = relativePosition(blockPosition);
		if (relPos.y > 19)
		{
			return true;
		}
	}
	
	return false;
}

bool Tetromino::isCollidingWithBorders(const sf::Vector2f& blockPosition) const
{
	sf::Vector2f relPos = relativePosition(blockPosition);

	if (relPos.x < 0 
	|| relPos.x > 9
	|| relPos.y > 19)
	{
		return true;
	}
	
	return false;
}

bool Tetromino::isCollidingWithPlayfield(sf::Vector2f blockPosition)
{
	sf::Vector2f relPos = relativePosition(blockPosition);
	return mPlayfield.cellTypeByRowAndColumn(relPos.y, relPos.x) != TetrominoTypes::Type::None;
}

bool Tetromino::isCollidingWithPlayfield()
{
	for (sf::Vector2f& blockPosition : mShapeData)
	{
		sf::Vector2f relPos = relativePosition(blockPosition);

		for (int row = 0; row < Playfield::Rows; ++row)
		{
			for (int column = 0; column < Playfield::Columns; ++column)
			{
				if (mPlayfield.cellTypeByRowAndColumn(row, column) != TetrominoTypes::Type::None
				&& static_cast<int>(relPos.x) == column 
				&& static_cast<int>(relPos.y) == row)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void Tetromino::resetLockCountdown()
{
	mLockCountdown = sf::seconds(1.f);
}

bool Tetromino::canMoveSideways()
{
	mAbsolutePosition -= mVelocity;

	bool canMoveLeft = true;
	bool canMoveRight = true;

	mAbsolutePosition += sf::Vector2f(-1.f, 0.f);
	if (isCollidingWithPlayfield() || isCollidingWithBorders())
	{
		canMoveLeft = false;
	}
	mAbsolutePosition -= sf::Vector2f(-1.f, 0.f);

	mAbsolutePosition += sf::Vector2f(1.f, 0.f);
	if (isCollidingWithPlayfield() || isCollidingWithBorders())
	{
		canMoveRight = false;
	}
	mAbsolutePosition -= sf::Vector2f(1.f, 0.f);

	mAbsolutePosition += mVelocity;

	return canMoveLeft || canMoveRight;
}

bool Tetromino::canMoveDown()
{
	mAbsolutePosition -= mVelocity;

	bool canMoveDown = true;

	mAbsolutePosition += sf::Vector2f(0.f, 1.f);
	if (isCollidingWithPlayfield() || isCollidingWithBorders())
	{
		canMoveDown = false;
	}
	mAbsolutePosition -= sf::Vector2f(0.f, 1.f);

	mAbsolutePosition += mVelocity;

	return canMoveDown;
}
