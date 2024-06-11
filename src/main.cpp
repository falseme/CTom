#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int window_width = 800;
const int window_height = 600;
const int rows = 80;
const int cols = 60;

static void draw(std::vector<std::vector<bool>>& alive, sf::RenderWindow& rw);
static void update(std::vector<std::vector<bool>>& alive);
static bool isAlive(std::vector<std::vector<bool>> alive, int i, int j);

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Automata Celular");
	window.setFramerateLimit(5);

	std::vector<std::vector<bool>> alive(rows);
	for (int i = 0; i < rows; i++) {
		alive[i] = std::vector<bool>(cols);
		for (int j = 0; j < cols; j++)
			alive[i][j] = false;
	}

	alive[15][10] = true;
	alive[15][11] = true;
	alive[15][12] = true;

	alive[35][20] = true;
	alive[35][21] = true;
	alive[35][22] = true;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();

		}

		window.clear(sf::Color(100, 100, 100, 255));

		update(alive);
		draw(alive, window);

		window.display();
	}

	return 0;

}

static void draw(std::vector<std::vector<bool>>& alive, sf::RenderWindow& rw) {

	int w = window_width / rows;
	int h = window_height / cols;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			if (!alive[i][j])
				continue;

			sf::RectangleShape pixel(sf::Vector2f(w, h));
			pixel.setPosition(sf::Vector2f(i * w, j * h));
			pixel.setFillColor(sf::Color(255, 255, 255, 255));
			rw.draw(pixel);

		}
	}

}

static void update(std::vector<std::vector<bool>>& alive) {

	//init & update
	std::vector<std::vector<bool>> tempAlive(alive);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			// IF ALIVE: needs 2 or 3 neighbors to stay alive
			// IF DEAD: needs 3 neighbors to live

			int total = 0;
			//up-left
			if (isAlive(alive, i - 1, j - 1)) total++;
			//up
			if (isAlive(alive, i - 1, j)) total++;
			//up-right
			if (isAlive(alive, i - 1, j + 1)) total++;
			//left
			if (isAlive(alive, i, j - 1)) total++;
			//right
			if (isAlive(alive, i, j + 1)) total++;
			//bellow-left
			if (isAlive(alive, i + 1, j - 1)) total++;
			//bellow
			if (isAlive(alive, i + 1, j)) total++;
			//bellow-right
			if (isAlive(alive, i + 1, j + 1)) total++;

			if (total == 3) {
				tempAlive[i][j] = true;
				//std::cout << "alive: [" << i << ", " << j << "]" << std::endl;
			}
			else if (alive[i][j] && total != 2) {
				tempAlive[i][j] = false;
				//std::cout << "dead: [" << i << ", " << j << "]" << std::endl;
			}
		}

	}

	//copy
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			alive[i][j] = tempAlive[i][j];


}

static bool isAlive(std::vector<std::vector<bool>> alive, int i, int j) {

	if (i < 0 || i >= rows)
		return false;
	if (j < 0 || j >= cols)
		return false;

	return alive[i][j];

}
