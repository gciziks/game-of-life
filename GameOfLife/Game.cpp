#include "Game.h"

// Initializers

void Game::initVariables()
{
	this->window = nullptr;
}

void Game::initWindow()
{
	this->videoMode.height = 625;
	this->videoMode.width = 1400;
	this->window = new sf::RenderWindow(this->videoMode, "C++ Game Of Life");

}

void Game::initCells()
{
	int xPos = 0;
	int yPos = 0;
	for (int x = 0;x < XMAX;x++) {
		for (int y = 0;y < YMAX;y++) {
			this->map[x][y].textureLoader.loadFromFile(this->emptyTilePath);
			this->map[x][y].sprite.setTexture(this->map[x][y].textureLoader);
			this->map[x][y].sprite.setScale(5, 5);
			this->map[x][y].sprite.setPosition(xPos, yPos);
			yPos = yPos + 25;
		}
		yPos = 0;
		xPos = xPos + 25;
	}
}

void Game::initMenu()
{
	this->button.textureLoader.loadFromFile(this->buttonPath);
	this->button.sprite.setTexture(this->button.textureLoader);
	this->button.sprite.setScale(8, 8);
	this->button.sprite.setPosition(1262, 25);

	this->font.loadFromFile("./resources/pixelmix.ttf");
	this->genText.setCharacterSize(20);
	this->genText.setFont(this->font);
	this->genText.setString("Gen: 0");
	this->genText.setPosition(1262, 500);
}




// Constructors | Destructors

Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initCells();
	this->initMenu();
}

Game::~Game()
{
	delete this->window;
	delete this->map;
}


// Accessors

const bool Game::getWindowIsOpen() const
{
	return this->window->isOpen();
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
			break;
		default:
			break;
		}
		
		
	}
}

void Game::renderCells()
{
	for (int x = 0;x < XMAX;x++) {
		for (int y = 0; y < YMAX;y++) {
			this->window->draw(this->map[x][y].sprite);
		}
	}
	
}

void Game::renderMenu()
{
	this->window->draw(this->button.sprite);
	this->window->draw(this->genText);
}

void Game::checkClick()
{
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{		
		if (!this->isPressed) {
			this->checkSprite();
			this->isPressed = true;
		}
		
		
	}
	else {
		this->isPressed = false;
	}
	
}
void Game::checkSprite()
{
	if (this->button.sprite.getGlobalBounds().contains(this->mousePosView)) {
		this->startGame = true;
		return;
		
	}
	for (int x = 0;x < XMAX;x++) {
		for (int y = 0; y < YMAX;y++) {
			if (this->map[x][y].sprite.getGlobalBounds().contains(this->mousePosView))
			{
				this->map[x][y].textureLoader.loadFromFile(this->blackTilePath);
				this->map[x][y].state = true;
				break;
			}
		}
	}

}
void Game::updateMousePositions()
{
	this->mouserPosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mouserPosWindow);
	this->checkClick();

}
void Game::updateGeneration()
{
	for (auto &cell : this->toDieCells) {
		this->map[cell.x][cell.y].state = 0;
		/*this->toAliveCells.erase(std::remove(this->toAliveCells.begin(), this->toAliveCells.end(), sf::Vector2i(cell.x, cell.y)), this->toAliveCells.end());*/
	}
	this->toDieCells.clear();
	for (auto& cell : this->toAliveCells) {
		this->map[cell.x][cell.y].state = 1;
	}
	this->toAliveCells.clear();
	
	for (int x = 0;x < XMAX;x++) {
		for (int y = 0; y < YMAX;y++) {
			if (this->map[x][y].state == true) {
				this->map[x][y].textureLoader.loadFromFile(this->blackTilePath);
			}
			else {
				this->map[x][y].textureLoader.loadFromFile(this->emptyTilePath);
			}
		}
	}
	this->currentGeneration++;
	this->genText.setString("Gen: " + std::to_string(this->currentGeneration));

}

int Game::checkCorners(sf::Vector2i cell)
{

	int aliveCount = 0;
	for (int x = cell.x - 1;x <= cell.x + 1;x++) {
		for (int y = cell.y - 1;y <= cell.y + 1;y++) {
			if ((x != XMAX && x != -1) && (y != YMAX && y != -1)) {
				// Eliminate chances of improper access
				if (this->map[x][y].state == true && !(x == cell.x && y == cell.y)) {
					aliveCount++;
				}
				
			}

		}
	}

	return aliveCount;
	
}

void Game::gameRounds()
{
	for (int x = 0;x < XMAX;x++) {
		for (int y = 0; y < YMAX;y++) {
			int aliveCount = this->checkCorners(sf::Vector2i(x, y));
			
			if (this->map[x][y].state == true) {
				if (aliveCount < 2 || aliveCount > 3) {
					this->toDieCells.push_back(sf::Vector2i(x, y));
				}
			}
			else {
				if (aliveCount == 3) {
					this->toAliveCells.push_back(sf::Vector2i(x, y));
				}
				
			}
		}
	}
	this->endGame = this->checkEnd();
	this->updateGeneration();
}

bool Game::checkEnd()
{
	if (this->toAliveCells.size() <= 0 && this->toDieCells.size() <= 0) {
		return true;
	}
	else {
		return false;
	}

}

// Functions

void Game::update()
{


	if (!startGame) {
		this->pollEvents();
		this->updateMousePositions();
	}
	else {
		if (!this->initRounds) {
			this->startRounds();
			this->initRounds = true;
		}
		else {
			if (!this->endGame) {
				this->gameRounds();
			}
		}
	}
}

void Game::render()
{
	// Clear last frame
	this->window->clear(sf::Color::Black);
	// Draw
	this->renderCells();
	this->renderMenu();
	// Display in Screen
	this->window->display();
}
