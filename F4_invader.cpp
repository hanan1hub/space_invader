#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Audio.hpp> 

struct Shield {
	std::vector<sf::RectangleShape> pixels;  // Pixels forming the shield
	int health;                              // Health of the shield
	sf::Vector2f position;                   // Position of the shield

	// Declare the getBounds method
	sf::FloatRect getBounds() const;
};

sf::FloatRect Shield::getBounds() const {
	if (pixels.empty()) {
		return sf::FloatRect();  // Return an empty rectangle if no pixels exist
	}

	// Initialize bounds to the first pixel's bounds
	sf::FloatRect bounds = pixels[0].getGlobalBounds();

	// Expand bounds to include all other pixels
	for (const auto& pixel : pixels) {
		sf::FloatRect pixelBounds = pixel.getGlobalBounds();

		// Calculate the union manually
		float left = std::min(bounds.left, pixelBounds.left);
		float top = std::min(bounds.top, pixelBounds.top);
		float right = std::max(bounds.left + bounds.width, pixelBounds.left + pixelBounds.width);
		float bottom = std::max(bounds.top + bounds.height, pixelBounds.top + pixelBounds.height);

		bounds.left = left;
		bounds.top = top;
		bounds.width = right - left;
		bounds.height = bottom - top;
	}

	return bounds;
}


class EnemyBullet {
public:
	EnemyBullet(float startX, float startY) {
		bullet.setRadius(2.5);  // Bullet radius
		bullet.setFillColor(sf::Color::Yellow);  // Bullet color
		bullet.setPosition(startX, startY);   // Set starting position of the bullet
	}

	void update(float deltaTime);

	void draw(sf::RenderWindow& window) {
		window.draw(bullet);
	}



	sf::Vector2f getPosition() const {
		return bullet.getPosition();
	}
	sf::FloatRect getBounds() const {
		return bullet.getGlobalBounds();  // Replace bulletSprite with bullet
	}


private:
	sf::CircleShape bullet;  // Bullet shape

};
// Implementation of the update function
void EnemyBullet::update(float deltaTime) {
	bullet.move(0.f, 70.f * deltaTime);  // Move the bullet downward
}

// Bullet class 
class Bullet {
public:
	Bullet(float startX, float startY) {
		bullet.setRadius(2.5);  // Bullet radius
		bullet.setFillColor(sf::Color::White);  // Bullet color
		bullet.setPosition(startX, startY);     // Set starting position of the bullet
	}

	void update(float deltaTime) {
		bullet.move(0.f, -200.f * deltaTime);  // Move the bullet upwards
	}

	void draw(sf::RenderWindow& window) {
		window.draw(bullet);
	}

	sf::FloatRect getBounds() const {
		return bullet.getGlobalBounds();
	}

	sf::Vector2f getPosition() const {
		return bullet.getPosition();
	}

private:
	sf::CircleShape bullet;  // Bullet shape
};

// Spaceship class 
class Spaceship {
public:
	Spaceship() {
		// Create the body (a rectangle shape)
		body.setSize(sf::Vector2f(60.f, 20.f));  // width x height
		body.setFillColor(sf::Color::Green);
		body.setPosition(370.f, 570.f);  // Initial position

		// Create the wings (rectangles)
		leftWing.setSize(sf::Vector2f(20.f, 10.f));
		leftWing.setFillColor(sf::Color::Blue);
		leftWing.setPosition(350.f, 580.f);

		rightWing.setSize(sf::Vector2f(20.f, 10.f));
		rightWing.setFillColor(sf::Color::Blue);
		rightWing.setPosition(430.f, 580.f);

		// Create the nose (triangle shape)
		nose.setPointCount(3);  // 3 points for a triangle
		nose.setPoint(0, sf::Vector2f(390.f, 560.f));  // left point
		nose.setPoint(1, sf::Vector2f(410.f, 560.f));  // Right point
		nose.setPoint(2, sf::Vector2f(400.f, 550.f));  // top point
		nose.setFillColor(sf::Color::Red);
	}

	void update(float deltaTime) {
		sf::Vector2f position = body.getPosition();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (position.x > 0) { // Prevent moving out of the left bound
				body.move(-200 * deltaTime, 0);
				leftWing.move(-200 * deltaTime, 0);
				rightWing.move(-200 * deltaTime, 0);
				nose.move(-200 * deltaTime, 0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if (position.x + body.getSize().x < 800) { // Prevent moving out of the right bound
				body.move(200 * deltaTime, 0);
				leftWing.move(200 * deltaTime, 0);
				rightWing.move(200 * deltaTime, 0);
				nose.move(200 * deltaTime, 0);
			}
		}
	}

	sf::Vector2f getNosePosition() const {
		return sf::Vector2f(nose.getPosition().x + 372 + body.getSize().x / 2 - 5, nose.getPosition().y + 540);
	}

	// Method to get the bounding box of the spaceship (body, wings, and nose)
	sf::FloatRect getBounds() const {
		// Combine the bounding boxes of the body, wings, and nose
		sf::FloatRect bodyBounds = body.getGlobalBounds();
		sf::FloatRect leftWingBounds = leftWing.getGlobalBounds();
		sf::FloatRect rightWingBounds = rightWing.getGlobalBounds();
		sf::FloatRect noseBounds = nose.getGlobalBounds();

		// Get the smallest bounding box that can fit all parts
		float left = std::min({ bodyBounds.left, leftWingBounds.left, rightWingBounds.left, noseBounds.left });
		float top = std::min({ bodyBounds.top, leftWingBounds.top, rightWingBounds.top, noseBounds.top });
		float right = std::max({ bodyBounds.left + bodyBounds.width, leftWingBounds.left + leftWingBounds.width,
								rightWingBounds.left + rightWingBounds.width, noseBounds.left + noseBounds.width });
		float bottom = std::max({ bodyBounds.top + bodyBounds.height, leftWingBounds.top + leftWingBounds.height,
								  rightWingBounds.top + rightWingBounds.height, noseBounds.top + noseBounds.height });

		// Return the combined bounding box
		return sf::FloatRect(left, top, right - left, bottom - top);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(body);
		window.draw(leftWing);
		window.draw(rightWing);
		window.draw(nose);
	}

private:
	sf::RectangleShape body;
	sf::RectangleShape leftWing;
	sf::RectangleShape rightWing;
	sf::ConvexShape nose;
};
class Alien {
public:
	Alien(float x, float y) : initialPosition(x, y), isAlive(true), respawnTimer(0.f), respawnCount(0), fireTimer(0.f)//, amplitude(-0.1f), speed(-0.1f)
	{
		createAlien();
	}

	void update(float deltaTime) {
		if (!isAlive) {
			respawnTimer += deltaTime;
			if (respawnTimer >= 1.5f && respawnCount < 5) {
				isAlive = true;
				respawnTimer = 0.f;
				respawnCount++;
				resetPosition();
			}
			return;
		}

		//      // Oscillating horizontal movement
		//float time = clock.getElapsedTime().asSeconds();
		//float offsetX = amplitude * std::sin(speed * time);
		//for (auto& pixel : pixels) {
		//    pixel.setPosition(initialPosition.x + offsetX + (pixel.getPosition().x - initialPosition.x), pixel.getPosition().y);
		//}

		fireTimer += deltaTime;  // Increment the fire timer
		if (fireTimer >= 12.f) {  // Fire every 12 seconds
			fireBullet();
			fireTimer = -2.f;
		}

		// Update enemy bullets if the alien is alive
		for (auto& bullet : enemyBullets) {
			bullet.update(deltaTime);
		}
	}

	void fireBullet() {
		if (isAlive) {
			// Create and add an enemy bullet
			enemyBullets.push_back(EnemyBullet(initialPosition.x + 30.f, initialPosition.y + 20.f)); // Adjust position
		}
	}

	void draw(sf::RenderWindow& window) {
		if (isAlive) {
			for (auto& pixel : pixels) {
				window.draw(pixel);
			}
		}

		// Draw bullets only if the alien is alive
		for (auto& bullet : enemyBullets) {
			bullet.draw(window);
		}
	}

	std::vector<EnemyBullet> enemyBullets;  // Bullets fired by this alien

	sf::FloatRect getBounds() const {
		if (pixels.empty() || !isAlive) {
			return sf::FloatRect(); // Return an empty rectangle if not alive or no pixels
		}

		sf::FloatRect bounds = pixels[0].getGlobalBounds();
		for (size_t i = 1; i < pixels.size(); ++i) {
			sf::FloatRect pixelBounds = pixels[i].getGlobalBounds();
			bounds.left = std::min(bounds.left, pixelBounds.left);
			bounds.top = std::min(bounds.top, pixelBounds.top);
			bounds.width = std::max(bounds.left + bounds.width, pixelBounds.left + pixelBounds.width) - bounds.left;
			bounds.height = std::max(bounds.top + bounds.height, pixelBounds.top + pixelBounds.height) - bounds.top;
		}
		return bounds;
	}

	bool isAlive = true;  // Status of the alien

private:
	std::vector<sf::RectangleShape> pixels;  // Pixels forming the alien
	sf::Vector2f initialPosition;            // Initial position of the alien
	float respawnTimer;                      // Timer for respawn
	int respawnCount;                        // Number of times the alien has respawned
	float fireTimer;                         // Timer for firing bullets
	//float amplitude;                         // Horizontal movement range
	//float speed;                             // Oscillation speed
	//sf::Clock clock;                         // Time tracker for oscillation

	void createAlien() {
		float pixelSize = 4.f;
		int pattern[8][8] = {
			{0, 1, 1, 0, 0, 1, 1, 0},
			{1, 0, 1, 1, 1, 1, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{0, 1, 1, 1, 1, 1, 1, 0},
			{0, 0, 1, 1, 1, 1, 0, 0},
			{0, 1, 0, 0, 0, 0, 1, 0}
		};

		pixels.clear(); // Clear any previous alien pixels
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (pattern[i][j] == 1) {
					sf::RectangleShape pixel;
					pixel.setSize(sf::Vector2f(pixelSize, pixelSize));
					pixel.setFillColor(sf::Color::Blue);
					pixel.setPosition(initialPosition.x + j * pixelSize, initialPosition.y + i * pixelSize);
					pixels.push_back(pixel);
				}
			}
		}
	}

	void resetPosition() {
		createAlien(); // Reset the alien to its initial position
	}
};

void createShield(std::vector<sf::RectangleShape>& shieldPixels, sf::Vector2f position) {
	float pixelSize = 4.f; // Size of individual pixels

	// Shield Pattern (12x23 grid)
	int pattern[12][23] = {
	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}
	};

	shieldPixels.clear(); // Clear any previous shield pixels

	// Create pixels based on the shield pattern
	for (int i = 0; i < 12; ++i) { // Rows
		for (int j = 0; j < 23; ++j) { // Columns
			if (pattern[i][j] == 1) {
				sf::RectangleShape pixel;
				pixel.setSize(sf::Vector2f(pixelSize, pixelSize));
				pixel.setFillColor(sf::Color::Green); // Green color 
				pixel.setPosition(position.x + j * pixelSize, position.y + i * pixelSize);
				shieldPixels.push_back(pixel);
			}
		}
	}
}

std::vector<sf::RectangleShape> shieldPixels;
bool shieldActive = true; // Activate/Deactivate shield

std::vector<Shield> shields;  // Declare shields as a vector of Shield structures

void initializeShields() {
	shields.clear();  // Clear any existing shields

	// Create shield 1
	std::vector<sf::RectangleShape> shieldPixels1;
	createShield(shieldPixels1, sf::Vector2f(50.f, 450.f));
	shields.push_back({ shieldPixels1, 191, sf::Vector2f(50.f, 450.f) });  // Set health to 191

	// Create shield 2
	std::vector<sf::RectangleShape> shieldPixels2;
	createShield(shieldPixels2, sf::Vector2f(350.f, 450.f));
	shields.push_back({ shieldPixels2, 191, sf::Vector2f(350.f, 450.f) });  // Set health to 191

	// Create shield 3
	std::vector<sf::RectangleShape> shieldPixels3;
	createShield(shieldPixels3, sf::Vector2f(650.f, 450.f));
	shields.push_back({ shieldPixels3, 191, sf::Vector2f(650.f, 450.f) });  // Set health to 191
}


// Function to handle bullet-shield collision detection
void checkBulletShieldCollisions(std::vector<Bullet>& bullets, std::vector<Shield>& shields) {
	for (auto& shield : shields) {
		if (shield.health <= 0) continue;  // Skip destroyed shields

		for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
			bool bulletRemoved = false;

			// Check for collisions with shield pixels
			for (auto pixelIt = shield.pixels.begin(); pixelIt != shield.pixels.end();) {
				if (bulletIt->getBounds().intersects(pixelIt->getGlobalBounds())) {
					bulletIt = bullets.erase(bulletIt);    // Remove bullet
					pixelIt = shield.pixels.erase(pixelIt);  // Remove shield pixel
					shield.health--;  // Reduce shield health
					bulletRemoved = true;
					break;
				}
				else {
					++pixelIt;
				}
			}

			if (!bulletRemoved) {
				++bulletIt;
			}
		}
	}
}
void checkShieldEnemyBulletCollisions(std::vector<EnemyBullet>& enemyBullets, std::vector<Shield>& shields) {
	for (auto& shield : shields) {
		if (shield.health <= 0) continue;  // Skip destroyed shields
		for (auto bulletIt = enemyBullets.begin(); bulletIt != enemyBullets.end();) {
			bool bulletRemoved = false;  // Track if the bullet was removed
			// Check for collisions with shield pixels
			for (auto pixelIt = shield.pixels.begin(); pixelIt != shield.pixels.end();) {
				if (bulletIt->getBounds().intersects(pixelIt->getGlobalBounds())) {
					// Remove the bullet and set bulletRemoved to true
					bulletIt = enemyBullets.erase(bulletIt);
					pixelIt = shield.pixels.erase(pixelIt);  // Remove shield pixel
					shield.health--;  // Reduce shield health
					bulletRemoved = true;
					break;
				}
				else {
					++pixelIt;  // Advance to the next pixel
				}
			}
			if (!bulletRemoved) {
				++bulletIt;
			}
		}
	}
}



class Button {
public:
	Button(float x, float y, float width, float height, float radius, const std::string& text, sf::Font& font)
		: radius(radius) {
		// Create the main rectangle
		body.setSize(sf::Vector2f(width - 2 * radius, height));
		body.setPosition(x + radius, y); // Position the rectangle
		body.setFillColor(sf::Color::Black);

		// Create the corner circles
		for (int i = 0; i < 4; ++i) {
			sf::CircleShape corner(radius);
			corner.setFillColor(sf::Color::Black);
			corner.setPointCount(30); // Smooth circle

			// Position the corners
			if (i == 0) { // Top-left
				corner.setPosition(x, y);
			}
			else if (i == 1) { // Top-right
				corner.setPosition(x + width - radius * 2, y);
			}
			else if (i == 2) { // Bottom-left
				corner.setPosition(x, y + height - radius * 2);
			}
			else if (i == 3) { // Bottom-right
				corner.setPosition(x + width - radius * 2, y + height - radius * 2);
			}

			corners.push_back(corner);
		}

		// Create the label
		label.setFont(font);
		label.setString(text);
		label.setCharacterSize(27);
		label.setFillColor(sf::Color::White);

		// Center the text
		sf::FloatRect textBounds = label.getLocalBounds();
		label.setOrigin(textBounds.width / 2, textBounds.height / 2); // Set origin to center
		label.setPosition(x + width / 2, y + height / 2); // Center the text in the button
	}

	void draw(sf::RenderWindow& window) {
		// Draw the main rectangle
		window.draw(body);
		// Draw the corner circles
		for (auto& corner : corners) {
			window.draw(corner);
		}
		// Draw the label
		window.draw(label);
	}

	bool isHovered(sf::Vector2i mousePos) {
		// Check if the mouse is over the button's rectangle
		return body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
	}

	bool isClicked(sf::Vector2i mousePos) {
		return isHovered(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	void setColor(const sf::Color& color) {
		body.setFillColor(color);
		for (auto& corner : corners) {
			corner.setFillColor(color);
		}
	}

private:
	sf::RectangleShape body; // Main button body
	std::vector<sf::CircleShape> corners; // Corner circles
	sf::Text label; // Button label
	float radius; // Corner radius
};
//for restarting game
void restartGame(Spaceship& player, std::vector<Bullet>& bullets, std::vector<Alien>& aliens, std::vector<Shield>& shields, int& score, int& lives) {
	// Reset player position
	player = Spaceship();

	// Clear bullets
	bullets.clear();

	// Reset aliens
	aliens = {
		Alien(30.f, 30.f), Alien(150.f, 30.f), Alien(270.f, 30.f), Alien(390.f, 30.f), Alien(510.f, 30.f), Alien(630.f, 30.f), Alien(750.f, 30.f),
	   Alien(30.f, 100.f), Alien(150.f, 100.f), Alien(270.f, 100.f), Alien(390.f, 100.f), Alien(510.f, 100.f), Alien(630.f, 100.f), Alien(750.f, 100.f),
		Alien(30.f, 170.f), Alien(150.f, 170.f), Alien(270.f, 170.f), Alien(390.f, 170.f), Alien(510.f, 170.f), Alien(630.f, 170.f), Alien(750.f, 170.f)
	};

	// Reset shields
	initializeShields();

	// Reset score and lives
	score = 500;
	lives = 5;
}
//for restarting game
void resetGame(Spaceship& player, std::vector<Bullet>& bullets, std::vector<Alien>& aliens, int& score, int& lives) {
	// Reset player position
	player = Spaceship();

	// Clear bullets
	bullets.clear();

	// Reset aliens
	aliens = {
		Alien(30.f, 30.f), Alien(150.f, 30.f), Alien(270.f, 30.f), Alien(390.f, 30.f), Alien(510.f, 30.f), Alien(630.f, 30.f), Alien(750.f, 30.f),
		//    Alien(30.f, 100.f), Alien(150.f, 100.f), Alien(270.f, 100.f), Alien(390.f, 100.f), Alien(510.f, 100.f), Alien(630.f, 100.f), Alien(750.f, 100.f),
		//     Alien(30.f, 170.f), Alien(150.f, 170.f), Alien(270.f, 170.f), Alien(390.f, 170.f), Alien(510.f, 170.f), Alien(630.f, 170.f), Alien(750.f, 170.f)
	};

	// Reset shields
 //   initializeShields();

	// Reset score and lives
	score = 0;
	lives = 5;
}
// for udating level
void levelup(Spaceship& player, std::vector<Bullet>& bullets, std::vector<Alien>& aliens, std::vector<Shield>& shields, int& score, int& lives) {
	// Reset player position
	player = Spaceship();

	// Clear bullets
	bullets.clear();

	// Reset aliens
	aliens = {
		Alien(30.f, 30.f), Alien(150.f, 30.f), Alien(270.f, 30.f), Alien(390.f, 30.f), Alien(510.f, 30.f), Alien(630.f, 30.f), Alien(750.f, 30.f),
		Alien(30.f, 100.f), Alien(150.f, 100.f), Alien(270.f, 100.f), Alien(390.f, 100.f), Alien(510.f, 100.f), Alien(630.f, 100.f), Alien(750.f, 100.f),
	   Alien(30.f, 170.f), Alien(150.f, 170.f), Alien(270.f, 170.f), Alien(390.f, 170.f), Alien(510.f, 170.f), Alien(630.f, 170.f), Alien(750.f, 170.f)
	};

	// Reset shields
	initializeShields();

	// Reset score and lives
	score = 500;
	lives = 5;
}

// Add a high score variable and update logic
int highScore = 0;  // This will hold the highest score

void loadHighScore() {
	std::ifstream file("highscore.txt");
	if (file.is_open()) {
		file >> highScore;  // Load the high score from file
		file.close();
	}
}

void saveHighScore() {
	std::ofstream file("highscore.txt");
	if (file.is_open()) {
		file << highScore;  // Save the high score to file
		file.close();
	}
}
int main() {
menu:
	sf::RenderWindow window(sf::VideoMode(800, 600), "Spaceship Game", sf::Style::Default | sf::Style::Fullscreen);
	Spaceship player;
	int score = 0;  // Initialize score
	bool gameRunning = false;
	int lives = 7;
	// Load bullet firing sound
	sf::SoundBuffer bulletBuffer;
	if (!bulletBuffer.loadFromFile("C:/Users/H.M/Desktop/gunfire.wav")) {
		std::cerr << "Error loading bullet sound!" << std::endl;
		return -1;
	}
	sf::Sound bulletSound;
	bulletSound.setBuffer(bulletBuffer);

	// Load collision sound
	sf::SoundBuffer hitBuffer;
	if (!hitBuffer.loadFromFile("C:/Users/H.M/Desktop/hit.wav")) {
		std::cerr << "Error loading collision sound!" << std::endl;
		return -1;
	}
	sf::Sound collisionSound;
	collisionSound.setBuffer(hitBuffer);

	// Load background music
	sf::Music backgroundMusic;
	if (!backgroundMusic.openFromFile("C:/Users/H.M/Desktop/newbackground.wav")) {
		std::cerr << "Error loading background music!" << std::endl;
		return -1;
	}
	backgroundMusic.setLoop(true); // Loop the music
	backgroundMusic.play();       // Start playing music

	sf::Font font;
	if (!font.loadFromFile("C:/Users/H.M/Desktop/Roboto-Regular.ttf")) {
		std::cerr << "Error loading font!" << std::endl;
		return -1;
	}
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("C:/Users/H.M/Desktop/space.jpg")) {
		std::cerr << "Error loading background image!" << std::endl;
		return -1;
	}
	sf::Sprite backgroundSprite(backgroundTexture);

	// Load high score from file
	loadHighScore();

	// Create aliens
	std::vector<Alien> aliens = {

		Alien(30.f, 30.f),Alien(150.f, 30.f), Alien(270.f, 30.f), Alien(390.f, 30.f), Alien(510.f, 30.f), Alien(630.f, 30.f),Alien(750.f, 30.f)
	};
	//Shield
	sf::Vector2f shieldPosition = sf::Vector2f(600.f, 450.f); // Set shield position
	createShield(shieldPixels, shieldPosition);

	//Bullets
	std::vector<Bullet> bullets;
	std::vector<EnemyBullet> enemyBullets;
	sf::Clock clock;
	bool canFire = true;

	//  initializeShields();  // Initialize shields with health and position


	// Score and High Score Texts
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);

	sf::Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(24);
	highScoreText.setFillColor(sf::Color::White);
	highScoreText.setPosition(600.f, 0.f);  // Position in the top right corner


	sf::Text livesText;
	livesText.setFont(font);
	livesText.setCharacterSize(24);
	livesText.setFillColor(sf::Color::White);
	livesText.setPosition(350.f, 0.f); // Adjust as needed
	livesText.setString("Lives: " + std::to_string(lives));


	// Create buttons
	Button startButton(350.f, 290.f, 150.f, 50.f, 25.f, "Start", font);//Button(float x, float y, float width, float height, float radius, const std::string& text, sf::Font& font)
	Button quitButton(350.f, 500.f, 150.f, 50.f, 20.f, "Quit", font);
	Button creditsButton(350.f, 430.f, 150.f, 50.f, 20.f, "Credits", font);
	Button HIGHSCOREButton(350.f, 360.f, 150.f, 50.f, 20.f, "High Score", font);
	// Load background texture and create sprite

	// Get the size of the window and the background texture
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2u textureSize = backgroundTexture.getSize();

	// Scale the background to fit the window size
	float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
	float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
	backgroundSprite.setScale(scaleX, scaleY);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();


			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
				canFire = true;
			}
		}

		if (!gameRunning) {
			// Display menu
			window.clear();
			window.draw(backgroundSprite);  // Draw the background image during the menu screen
			startButton.draw(window);

			HIGHSCOREButton.draw(window);
			creditsButton.draw(window);
			quitButton.draw(window);
			// Track the selected button
			int selectedButtonIndex = 0; // 0 = Start,1 = high score, 2 = Quit, 3 = Credits 
			std::vector<Button*> buttons = { &startButton, &quitButton, &creditsButton,&HIGHSCOREButton };

			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {

					if (event.type == sf::Event::Closed)
						window.close();

					// Check for key presses
				  // Check if Escape key is pressed
					if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
						window.close(); // Close the window when Escape is pressed
					}

					if (event.type == sf::Event::KeyReleased) {

						if (event.key.code == sf::Keyboard::Down) {
							selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
						}
						else if (event.key.code == sf::Keyboard::Up) {
							selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
						}
						else if (event.key.code == sf::Keyboard::Enter) {
							if (selectedButtonIndex == 0) {
								gameRunning = true; // Start the game

							}
							else if (selectedButtonIndex == 1) {
								window.close(); // Quit the game
							}
							else if (selectedButtonIndex == 2) {
								// Show credits
								sf::Text creditsText;
								creditsText.setFont(font);
								creditsText.setCharacterSize(30);
								creditsText.setFillColor(sf::Color::White);
								creditsText.setString("Credits:\nHANAN MAJEED\nANAS NORANI\nNOOR UL SABA\nHAMNA SHAH");
								creditsText.setPosition(200.f, 250.f);

								window.clear();
								window.draw(creditsText);
								window.display();
								sf::sleep(sf::seconds(2));  // Display credits for 2 seconds
							}
							else if (selectedButtonIndex == 3) {
								sf::Text highscoreText;
								highscoreText.setFont(font); // Set the font
								highscoreText.setCharacterSize(30); // Set font size
								highscoreText.setFillColor(sf::Color::White); // Set text color
								highscoreText.setString("High Score: " + std::to_string(highScore)); // Display the highscore 

								highscoreText.setPosition(200.f, 250.f); // Positioning the text on the screen

								window.clear(); // Clear the window
								window.draw(highscoreText); // Draw the highscore text
								window.display(); // Display the updated content
								sf::sleep(sf::seconds(2));
							}
						}
					}
				}

				// Display menu
				window.clear();
				window.draw(backgroundSprite);
				for (size_t i = 0; i < buttons.size(); ++i) {
					if (i == selectedButtonIndex) {
						buttons[i]->setColor(sf::Color(128, 128, 128)); // Highlight selected button
					}
					else {
						buttons[i]->setColor(sf::Color::Black); // Default color
					}
					buttons[i]->draw(window);
				}
				window.display();
				if (gameRunning) {

					break; // Exit the menu loop to start the game

				}
			}
		}


		float deltaTime = clock.restart().asSeconds();
		player.update(deltaTime);

		// If the player earns points, check if it's a new high score
		if (score > highScore) {
			highScore = score;  // Update high score
			saveHighScore();    // Save new high score to file
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canFire) {
			sf::Vector2f nosePosition = player.getNosePosition();
			bullets.push_back(Bullet(nosePosition.x, nosePosition.y));
			// Play the bullet firing sound
			bulletSound.play();
			canFire = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
		// Update bullets
		for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
			bulletIt->update(deltaTime);

			if (bulletIt->getPosition().y < 0) {
				bulletIt = bullets.erase(bulletIt);
			}
			else {
				++bulletIt;
			}
		}
		// Check for collisions between bullets and shields
		checkBulletShieldCollisions(bullets, shields);

		// Update the enemy bullets
		for (auto& enemy : aliens) {
			for (auto& bullet : enemy.enemyBullets) {
				bullet.update(deltaTime);  // Update each enemy bullet
			}
		}

		std::vector<EnemyBullet> enemyBullets;  // This holds all enemy bullets

		// Collect enemy bullets from all aliens
		for (auto& alien : aliens) {
			for (auto& bullet : alien.enemyBullets) {
				enemyBullets.push_back(bullet);
			}
		}

		// Now, check for collisions between these bullets and shields
		checkShieldEnemyBulletCollisions(enemyBullets, shields);

		// Remove shields with zero health
		shields.erase(
			std::remove_if(shields.begin(), shields.end(), [](Shield& shield) {
				return shield.health <= 0;  // Remove shields with zero health
				}),
			shields.end()
		);

		// Collision detection and score update
		for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
			bool bulletRemoved = false;
			for (auto& alien : aliens) {
				if (alien.isAlive && bulletIt->getBounds().intersects(alien.getBounds())) {
					// Play hit sound
					collisionSound.play();
					alien.isAlive = false;
					bulletIt = bullets.erase(bulletIt);
					score += 10;  // Add points for each alien killed
					bulletRemoved = true;
					break;
				}
			}
			if (!bulletRemoved) {
				++bulletIt;
			}
		}

		bool allAliensDead = true;
		for (auto& alien : aliens) {
			if (alien.isAlive) {
				allAliensDead = false;
				break;
			}
		}

		if (allAliensDead) {
			// Handle game over because all aliens are dead
			gameRunning = false;  // Stop the gameplay updates

			// Display "Game Over" message
			sf::Text gameOverText;
			gameOverText.setFont(font);
			gameOverText.setCharacterSize(50);
			gameOverText.setFillColor(sf::Color::White);
			gameOverText.setString("Congrats you cleared the level\nYour Score: " + std::to_string(score) +
				"\npress L to levelup\nPress Q to Quit \nPress M to Return to Menu");
			gameOverText.setPosition(100.f, 200.f);

			window.clear();
			window.draw(gameOverText);
			window.display();

			while (!gameRunning && window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::KeyPressed) {

						if (event.key.code == sf::Keyboard::M) {
							goto menu;
						}
						else   if (event.key.code == sf::Keyboard::L) {
							levelup(player, bullets, aliens, shields, score, lives);
							gameRunning = true;
						}
						else if (event.key.code == sf::Keyboard::Q || event.type == sf::Event::Closed) {
							window.close();
						}
					}
				}
			}
		}

		for (auto& alien : aliens) {
			for (auto& bullet : alien.enemyBullets) {
				if (player.getBounds().intersects(bullet.getBounds())) {
					lives--; // Deduct one life
					bullet = alien.enemyBullets.back();
					alien.enemyBullets.pop_back();

					if (lives <= 0) {
						gameRunning = false;

						// Display "Game Over" message
						sf::Text gameOverText;
						gameOverText.setFont(font);
						gameOverText.setCharacterSize(50);
						gameOverText.setFillColor(sf::Color::White);
						gameOverText.setString("Game Over\nYour Score: " + std::to_string(score) +
							"\nPress P to replay level 1\n R to play level 2\n Q to Quit \nM to Return to Menu");
						gameOverText.setPosition(100.f, 200.f);

						window.clear();
						window.draw(gameOverText);
						window.display();

						while (!gameRunning && window.isOpen()) {
							sf::Event event;
							while (window.pollEvent(event)) {
								if (event.type == sf::Event::KeyPressed) {
									if (event.key.code == sf::Keyboard::P) {
										resetGame(player, bullets, aliens, score, lives);
										gameRunning = true;
									}
									else if (event.key.code == sf::Keyboard::R) {
										restartGame(player, bullets, aliens, shields, score, lives);
										gameRunning = true;
									}
									else if (event.key.code == sf::Keyboard::M) {
										goto menu;
									}

									else if (event.key.code == sf::Keyboard::Q || event.type == sf::Event::Closed) {
										window.close();
									}
								}
							}
						}
					}
					break;
				}
			}
		}

		// Update aliens for respawn
		for (auto& alien : aliens) {
			alien.update(deltaTime);
		}

		for (auto& enemy : aliens) {
			enemy.update(deltaTime);  // Update each alien

			// Update enemy bullets
			for (auto& bullet : enemy.enemyBullets) {
				bullet.update(deltaTime);
			}
		}

		// Check for collisions between enemy bullets and player spaceship
		for (auto bulletIt = enemyBullets.begin(); bulletIt != enemyBullets.end();) {
			if (bulletIt->getBounds().intersects(player.getBounds())) {
				// Handle collision (e.g., reduce player health, destroy the bullet)
				bulletIt = enemyBullets.erase(bulletIt);  // Remove the bullet after collision
			}
			else {
				++bulletIt;
			}
		}

		// Update score text
		scoreText.setString("Score: " + std::to_string(score));
		highScoreText.setString("High Score: " + std::to_string(highScore));

		// Draw everything
		window.clear(sf::Color::Black);

		player.draw(window);

		for (auto& alien : aliens) {
			alien.draw(window);
		}

		for (auto& bullet : bullets) {
			bullet.draw(window);
		}

		// Draw shield health
		for (auto& shield : shields) {
			if (shield.health > 0) {  // Only draw shields with health > 0
				for (auto& pixel : shield.pixels) {
					window.draw(pixel);
				}
			}
		}
		for (const auto& shield : shields) {
			for (const auto& pixel : shield.pixels) {
				window.draw(pixel);  // Draw each remaining pixel of the shield
			}
		}
		//update live text
		livesText.setString("Lives: " + std::to_string(lives));

		window.draw(scoreText);  // Draw score
		window.draw(highScoreText);  // Draw the high score
		window.draw(livesText);
		window.display();
	}

	return 0;
}