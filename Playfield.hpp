#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "TetrominoTypes.hpp"
#include "ClearLinesAnimation.hpp"
#include "SoundPlayer.hpp"
#include "Game.hpp"

class Tetromino;
class GameState;

class Playfield : public sf::Drawable
{
	public:
		static const int Columns = 10;
		static const int Rows = 20;

		Playfield(sf::Texture& texture, GameState& game, SoundPlayer& soundPlayer);

		bool update(Tetromino& tetromino, sf::Time elapsedTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		TetrominoTypes::Type cellTypeByRowAndColumn(int row, int column) const;
		bool addLinesToClear(sf::Time elapsedTime);
		bool hasLinesToClear() const;
		void clearLines();
		std::vector<int> linesToClear() const;
		bool isGameOver(Tetromino& tetromino) const;

	private:
		void mergeTetromino(Tetromino& tetromino);
		void setCellTypeByRowAndColumn(int row, int column, TetrominoTypes::Type type);
		bool isLineFilledUp(int row) const;
		void clearLine(int row);
		void dropBlocks(int clearedLinesCount, int fromRow);

		std::array<TetrominoTypes::Type, Rows * Columns> mPlayfieldData;
		sf::RectangleShape mBorder;
		sf::Texture mSpritesheet;
		GameState& mGame;
		std::vector<int> mLinesToClear;
		SoundPlayer& mSoundPlayer;
};
