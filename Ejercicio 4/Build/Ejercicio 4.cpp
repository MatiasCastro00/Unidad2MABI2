#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

class Game {
public:
	Game(int ancho, int alto, std::string titulo) {
		wnd = new sf::RenderWindow(sf::VideoMode(ancho, alto), titulo);
		wnd->setFramerateLimit(60);
		frameTime = 1.0f / 60.0f;
		InitPhysics();
		SetZoom();
	}

	~Game() {
		delete wnd;
		delete phyWorld;
	}

	void Loop() {
		while (wnd->isOpen()) {
			sf::Event evt;
			while (wnd->pollEvent(evt)) {
				if (evt.type == sf::Event::Closed)
					wnd->close();
			}

			HandleInput();
			UpdatePhysics();
			wnd->clear(sf::Color::Black);
			DrawGame();
			wnd->display();
		}
	}

private:
	sf::RenderWindow* wnd;
	b2World* phyWorld;
	b2Body* boxBody;
	float frameTime;
	float forceMagnitude = 100.0f;

	void InitPhysics() {
		b2Vec2 gravity(0.0f, 9.8f);
		phyWorld = new b2World(gravity);

		b2BodyDef groundDef;
		groundDef.position.Set(400.0f / 30.0f, 500.0f / 30.0f);
		b2Body* groundBody = phyWorld->CreateBody(&groundDef);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(500.0f / 30.0f, 10.0f / 30.0f);
		b2FixtureDef groundFixture;
		groundFixture.shape = &groundBox;
		groundFixture.density = 0.0f;
		groundFixture.friction = 0.35f;
		groundBody->CreateFixture(&groundFixture);

		// Create the box
		b2BodyDef boxDef;
		boxDef.type = b2_dynamicBody;
		boxDef.position.Set(400.0f / 30.0f, 450.0f / 30.0f);
		boxBody = phyWorld->CreateBody(&boxDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(20.0f / 30.0f, 20.0f / 30.0f);
		b2FixtureDef boxFixture;
		boxFixture.shape = &boxShape;
		boxFixture.density = 1.0f;
		boxFixture.friction = 0.35f;
		boxBody->CreateFixture(&boxFixture);
	}

	void HandleInput() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			boxBody->ApplyForceToCenter(b2Vec2(-forceMagnitude, 0.0f), true);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			boxBody->ApplyForceToCenter(b2Vec2(forceMagnitude, 0.0f), true);
		}
	}

	void UpdatePhysics() {
		phyWorld->Step(frameTime, 8, 3);
	}

	void DrawGame() {
		// Draw the ground
		sf::RectangleShape groundShape(sf::Vector2f(1000.0f, 20.0f));
		groundShape.setFillColor(sf::Color::White);
		groundShape.setPosition(100.0f, 470.0f);
		wnd->draw(groundShape);

		// Draw the box
		sf::RectangleShape boxShape(sf::Vector2f(40.0f, 40.0f));
		boxShape.setFillColor(sf::Color::Red);
		b2Vec2 position = boxBody->GetPosition();
		boxShape.setPosition(position.x * 30.0f - 20.0f, position.y * 30.0f - 20.0f);
		wnd->draw(boxShape);
	}

	void SetZoom() {
		sf::View camara(sf::FloatRect(0, 0, 800, 600));
		wnd->setView(camara);
	}
};

int main() {
	Game game(800, 600, "Box2D Sliding Box Simulation with Friction");
	game.Loop();
	return 0;
}
