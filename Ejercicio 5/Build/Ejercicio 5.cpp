#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <cmath>

class Game {
public:
	Game(int ancho, int alto, std::string titulo, float angle) :
		angleDegrees(angle) {
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
	b2Body* groundBody;
	float frameTime;
	float angleDegrees;

	void InitPhysics() {
		b2Vec2 gravity(0.0f, 9.8f);
		phyWorld = new b2World(gravity);

		b2BodyDef groundDef;
		groundDef.position.Set(400.0f / 30.0f, 500.0f / 30.0f);
		groundBody = phyWorld->CreateBody(&groundDef);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(500.0f / 30.0f, 10.0f / 30.0f);
		b2FixtureDef groundFixture;
		groundFixture.shape = &groundBox;
		groundFixture.density = 0.0f;
		groundFixture.friction = 0.5f;
		groundBody->CreateFixture(&groundFixture);
		groundBody->SetTransform(groundBody->GetPosition(), -angleDegrees * b2_pi / 180.0f);

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
		boxFixture.friction = 0.0f;
		boxBody->CreateFixture(&boxFixture);
	}

	void UpdatePhysics() {
		phyWorld->Step(frameTime, 8, 3);
	}

	void DrawGame() {
		sf::RectangleShape groundShape(sf::Vector2f(1000.0f, 20.0f));
		groundShape.setFillColor(sf::Color::White);
		groundShape.setOrigin(500.0f, 10.0f);
		groundShape.setPosition(400.0f, 500.0f);
		groundShape.setRotation(-angleDegrees);

		wnd->draw(groundShape);

		sf::RectangleShape boxShape(sf::Vector2f(40.0f, 40.0f));
		boxShape.setFillColor(sf::Color::Red);
		b2Vec2 position = boxBody->GetPosition();
		float angle = boxBody->GetAngle() * 180.0f / b2_pi;
		boxShape.setOrigin(20.0f, 20.0f);
		boxShape.setPosition(position.x * 30.0f, position.y * 30.0f);
		boxShape.setRotation(angle);

		wnd->draw(boxShape);
	}

	void SetZoom() {
		sf::View camara(sf::FloatRect(0, 0, 800, 600));
		wnd->setView(camara);
	}
};

int main() {
	Game game(800, 600, "Box2D Inclined Plane Simulation with Friction", 15.0f);
	game.Loop();
	return 0;
}
