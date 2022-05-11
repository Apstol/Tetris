#pragma once

#include <SFML/Graphics.hpp>

#include "TetrominoTypes.hpp"
#include "Playfield.hpp"
#include "SoundPlayer.hpp"

class Tetromino : public sf::Drawable
{
	public:
		typedef std::array<sf::Vector2f, 4> TetrominoShapeData;
		
		static const sf::Vector2f Size;
		static const sf::Vector2f Scale;

		Tetromino(TetrominoTypes::Type type, const sf::Texture& texture, const Playfield& playfield, SoundPlayer& soundPlayer);

		static sf::IntRect textureRectByType(TetrominoTypes::Type type);

		void update(sf::Time elapsedTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		
		bool move(sf::Vector2f direction, bool isHardDrop = false);
		void rotate();
		TetrominoShapeData& shapeData();
		TetrominoTypes::Type type() const;
		bool isLocked() const;
		void hardDrop();
		sf::Vector2f relativePosition(const sf::Vector2f& blockPosition) const;
	
	protected:
		const sf::Texture& mSpritesheet;
		TetrominoShapeData mShapeData;
		TetrominoTypes::Type mType;
		sf::Vector2f mAbsolutePosition;

	private:
		static const sf::Time ManualDropPeriod;

		void createShape();
		bool isCollidingWithBorders() const;
		bool isCollidingWithBottomBorder() const;
		bool isCollidingWithBorders(const sf::Vector2f& blockPosition) const;
		bool isCollidingWithPlayfield(sf::Vector2f blockPosition);
		bool isCollidingWithPlayfield();
		void resetLockCountdown();
		bool canMoveSideways();
		bool canMoveDown();

		sf::Time mAutoDropPeriod;
		const Playfield& mPlayfield;
		sf::Vector2f mTransformOrigin;
		sf::Vector2f mVelocity;
		bool mIsLocking;
		bool mIsLocked;
		bool mToggledRotation; // rotation bookkeeping for I type tetromino
		sf::Time mLockCountdown;
		sf::Time mTimeSinceLastAutoDrop;
		sf::Time mTimeSinceLastManualDrop;
		SoundPlayer& mSoundPlayer;
};
