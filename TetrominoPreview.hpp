#pragma once

#include "Tetromino.hpp"

class TetrominoPreview : public Tetromino
{
	public:
		TetrominoPreview(TetrominoTypes::Type type, sf::Texture& texture, Playfield& playfield, SoundPlayer& soundPlayer);

	private:
		sf::Vector2f blockOffset() const;
};
