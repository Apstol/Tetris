#pragma once

#include "State.hpp"
#include "GameOverAnimation.hpp"

class GameOverState : public State
{
	public:
		enum GameOverOptions
		{
			PlayAgain,
			Exit,
			
			Count	
		};

		GameOverState(StateStack& stack, Context context);

		virtual bool handleEvent(const sf::Event& event);
		virtual bool handleRealtimeInput(sf::Time elapsedTime);
		virtual bool update(sf::Time elapsedTime);
		virtual void draw();

	private:
		static const sf::Time TimePerHighscoreBlink;

		bool isNewHighscore();

		sf::Text mScoreText;
		sf::Text mHighscoreText;
		GameOverAnimation mGameOverAnimation;
		sf::RectangleShape mOverlay;
		int mSelectedOption;
		std::array<sf::Text, GameOverOptions::Count> mOptions;
		sf::Time mHighscoreBlinkingTimeElapsed;
		bool mPlayedHighscoreSound;
};
