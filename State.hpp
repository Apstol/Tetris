#pragma once

#include <SFML/Graphics.hpp>

#include "StateIdentifiers.hpp"
#include "SoundPlayer.hpp"
#include "MusicPlayer.hpp"

class StateStack;
class Game;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context(sf::RenderWindow& window, sf::Font& font, sf::Texture& texture, SoundPlayer& soundPlayer, MusicPlayer& musicPlayer, Game& game);

			sf::RenderWindow& window;
			sf::Font& font;
			sf::Texture& spritesheet;
			SoundPlayer& soundPlayer;
			MusicPlayer& musicPlayer;
			Game& game;
		};

		State(StateStack& stack, Context context);
		virtual ~State();

		virtual bool handleEvent(const sf::Event& event) = 0;
		virtual bool handleRealtimeInput(sf::Time elapsedTime) = 0;
		virtual bool update(sf::Time elapsedTime) = 0;
		virtual void draw() = 0;

	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();
		Context& context();

	private:
		StateStack& mStack;
		Context mContext;
};
