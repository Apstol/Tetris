#pragma once

class Utility
{
	public:
		template <typename T>
		static void centerOrigin(T& transformable)
		{
			sf::FloatRect bounds = transformable.getLocalBounds();
			transformable.setOrigin(bounds.width / 2, bounds.height / 2);
		}
};
