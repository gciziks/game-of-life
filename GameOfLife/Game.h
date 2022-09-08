#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include <vector>

const int XMAX = 50;
const int YMAX = 25;
const int NCOLORS = 11;

class Game
{


	private:
		// SFML Variables
		sf::RenderWindow* window;
		sf::Event ev;
		sf::VideoMode videoMode;

		//Sprite Files./resources/BlackTile.png
		std::string emptyTilePath = "./resources/BlackTile.png";
		std::string blackTilePath = "./resources/MapTile.png";
		std::string buttonPath = "./resources/ButtonTile.png";

		//Mouse Positions
		sf::Vector2i mouserPosWindow;
		sf::Vector2f mousePosView;
		bool mouseHeld;

		// Structures
		struct point {
			bool state;
			sf::Texture textureLoader;
			sf::Sprite sprite;
		};
		// Menu
		sf::Font font;
		sf::Text genText;
		int currentGeneration = 0;
		point button;

		// Vars
		point (*map)[YMAX] = new point[XMAX][YMAX];
		bool startGame = false;
		bool initRounds = false;
		bool isPressed = false;
		bool endGame = false;
		std::vector<sf::Vector2i> toAliveCells;
		std::vector<sf::Vector2i> toDieCells;
		
		// Initializers

		void initVariables();
		void initWindow();
		void initCells();
		void initMenu();

		//Functions
		void renderCells();
		void renderMenu();
		void checkClick();
		void checkSprite();
		void updateMousePositions();
		void updateGeneration();
		int checkCorners(sf::Vector2i cell);
		void gameRounds();
		bool checkEnd();
		



	public:

		//Constructors | Desctructors
		Game();
		virtual ~Game();

		//Accessors
		const bool getWindowIsOpen() const;

		// Functions
		void pollEvents();
		

		// Main Functions
		void update();
		void render();
};

