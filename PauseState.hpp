#pragma once

#include "State.hpp"

class PauseState : public State
{
	public:
		PauseState(StateStack& stack, Context context);
		~PauseState();

		virtual bool handleEvent(const sf::Event& event);
		virtual bool handleRealtimeInput(sf::Time elapsedTime);
		virtual bool update(sf::Time elapsedTime);
		virtual void draw();

	private:
		static const sf::Time TimePerTextBlink;

		sf::RectangleShape mOverlay;
		sf::Text mText;
		sf::Time mTextBlinkingTimeElapsed;
};
