#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <cmath>
#include <vector>

class Game {
public:
	Game(int ancho, int alto, std::string titulo) {
		wnd = new sf::RenderWindow(sf::VideoMode(ancho, alto), titulo);
		wnd->setFramerateLimit(60);
		frameTime = 1.0f / 60.0f;
		cannonAngle = 45.0f;
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
				else if (evt.type == sf::Event::KeyPressed) {
					if (evt.key.code == sf::Keyboard::Space) {
						Fire();
					}
					else if (evt.key.code == sf::Keyboard::Up) {
						cannonAngle -= 5.0f;
						if (cannonAngle > 85.0f) cannonAngle = 85.0f;
					}
					else if (evt.key.code == sf::Keyboard::Down) {
						cannonAngle += 5.0f;
						if (cannonAngle < -85.0f) cannonAngle = -85.0f;
					}
				}
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
	float frameTime;
	float cannonAngle;
	std::vector<b2Body*> projectiles;

	void InitPhysics() {
		b2Vec2 gravity(0.0f, 9.8f);
		phyWorld = new b2World(gravity);
	}

	void Fire() {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(50.0f / 30.0f, 300.0f / 30.0f);
		b2Body* body = phyWorld->CreateBody(&bodyDef);

		b2CircleShape circleShape;
		circleShape.m_radius = 15.0f / 30.0f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = 1.0f;
		fixtureDef.restitution = 0.6f;
		body->CreateFixture(&fixtureDef);

		float radians = cannonAngle * b2_pi / 180.0f;
		float forceMagnitude = 12.0f;
		b2Vec2 force(forceMagnitude * cos(radians), forceMagnitude * sin(radians));
		body->ApplyLinearImpulseToCenter(force, true);
		projectiles.push_back(body);
	}

	void UpdatePhysics() {
		phyWorld->Step(frameTime, 8, 3);
	}

	void DrawGame() {

		sf::RectangleShape cannon(sf::Vector2f(100.0f, 20.0f));
		cannon.setFillColor(sf::Color::White);
		cannon.setPosition(30.0f, 300.0f);
		cannon.setOrigin(0.0f, 10.0f);
		cannon.setRotation(cannonAngle);
		wnd->draw(cannon);

		// Dibuja los proyectiles
		for (auto& projectile : projectiles) {
			sf::CircleShape shape(15.0f);
			shape.setFillColor(sf::Color::Red);
			b2Vec2 pos = projectile->GetPosition();
			shape.setPosition(pos.x * 30.0f - 15.0f, pos.y * 30.0f - 15.0f);
			wnd->draw(shape);
		}
	}

	void SetZoom() {
		sf::View camara(sf::FloatRect(0, 0, 800, 600));
		wnd->setView(camara);
	}
};

int main() {
	Game game(800, 600, "Cannon Game");
	game.Loop();
	return 0;
}
