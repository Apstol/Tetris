#pragma once

#include "State.hpp"

class MenuState : public State
{
	public:
		enum MenuOptions
		{
			NewGame,
			Exit,
			
			Count	
		};

		MenuState(StateStack& stack, Context context);
		~MenuState();

		virtual bool handleEvent(const sf::Event& event);
		virtual bool handleRealtimeInput(sf::Time elapsedTime);
		virtual bool update(sf::Time elapsedTime);
		virtual void draw();

	private:
		sf::Text mControls;
		int mSelectedOption;
		std::array<sf::Text, MenuOptions::Count> mOptions;
};
