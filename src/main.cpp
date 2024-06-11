#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int window_width = 800;
const int window_height = 600;
const int rows = 80;
const int cols = 60;

static void draw(const std::vector<std::vector<bool>>& alive, sf::RenderWindow& rw);
static void update(std::vector<std::vector<bool>>& alive);
static bool isAlive(const std::vector<std::vector<bool>>& alive, int i, int j);

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Automata Celular", sf::Style::Close);
	window.setFramerateLimit(10);

	std::vector<std::vector<bool>> alive(rows);
	for (int i = 0; i < rows; i++) {
		alive[i] = std::vector<bool>(cols);
		for (int j = 0; j < cols; j++)
			alive[i][j] = false;
	}

	alive[40][31] = true;
	alive[41][31] = true;
	alive[42][31] = true;

	bool paused = true;
	std::cout << "Currently Paused" << std::endl;
	std::cout << "Press [space] to pause/unpause" << std::endl;
	std::cout << "While paused: Press [mouse RB] to draw and [mouse LB] to erase" << std::endl;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			switch (event.type) {

			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.scancode == sf::Keyboard::Scan::Space)
					paused = !paused;
				break;

			case sf::Event::MouseButtonPressed:
				float x = event.mouseButton.x * rows;
				int i = x / window_width;
				float y = event.mouseButton.y * cols;
				int j = y / window_height;
				if (event.mouseButton.button == sf::Mouse::Left)
					alive[i][j] = true;
				else if (event.mouseButton.button == sf::Mouse::Right)
					alive[i][j] = false;
				break;
			}

		}

		window.clear(sf::Color(32, 32, 32, 255));

		if (!paused)
			update(alive);

		draw(alive, window);
		window.display();
	}

	return 0;

}

static void draw(const std::vector<std::vector<bool>>& alive, sf::RenderWindow& rw) {

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

static bool isAlive(const std::vector<std::vector<bool>>& alive, int i, int j) {

	if (i < 0)
		i = rows - 1;
	else if (i >= rows)
		i = 0;
	if (j < 0)
		j = cols - 1;
	else if (j >= cols)
		j = 0;

	return alive[i][j];

}
