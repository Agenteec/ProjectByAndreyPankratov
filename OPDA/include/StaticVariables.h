#pragma once
#include <SFML/Graphics/Font.hpp>
namespace sv {
	class Fonts {
	public:
		static sf::Font arial;

		static void Init() {
			arial.loadFromFile("C:/Windows/Fonts/arial.ttf");
		}
	};
}
