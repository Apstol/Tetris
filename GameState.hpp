#pragma once

#include <memory>

#include "State.hpp"
#include "Tetromino.hpp"
#include "TetrominoPreview.hpp"
#include "Playfield.hpp"

class GameState : public State
{
	public:
		GameState(StateStack& stack, Context context);

		virtual bool handleEvent(const sf::Event& event);
		virtual bool handleRealtimeInput(sf::Time elapsedTime);
		virtual bool update(sf::Time elapsedTime);
		virtual void draw();

		void updateScore(int clearedLinesCount);

	private:
		void setNextTetrominoType();
		void spawnTetromino();
		void createNextTetrominoPreview();
		void setTimePerUpdate();
		void incrementLevel(int clearedLinesCount);
		int linesBeforeLevelUp();

		Playfield mPlayfield;
		std::unique_ptr<Tetromino> mActiveTetromino;
		std::unique_ptr<TetrominoPreview> mNextTetrominoPreview;
		int mLevel;
		int mCurrentLinesCount;
		int mTotalLinesCount;
		sf::Text mScoreText;
		sf::Text mLevelText;
		sf::Text mLinesText;
		sf::Text mNextPieceText;
		sf::Time mTimeSinceLastUpdate;
		sf::Time mTimePerUpdate;
		TetrominoTypes::Type mNextPieceType;
		ClearLinesAnimation mClearLinesAnimation;
};
