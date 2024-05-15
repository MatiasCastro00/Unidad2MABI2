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

			UpdatePhysics();
			wnd->clear(sf::Color::Black);
			DrawGame2();
			wnd->display();
		}
	}

private:
	sf::RenderWindow* wnd;
	b2World* phyWorld;
	b2Body* ballBody;
	float frameTime;

	void InitPhysics() {
		b2Vec2 gravity(0.0f, 0.0f);
		phyWorld = new b2World(gravity);


		CreateWall(400, 0, 800, 20);
		CreateWall(400, 600, 800, 20);
		CreateWall(0, 300, 20, 600);
		CreateWall(800, 300, 20, 600);

		b2BodyDef ballDef;
		ballDef.type = b2_dynamicBody;
		ballDef.position.Set(400.0f / 30.0f, 300.0f / 30.0f);
		ballBody = phyWorld->CreateBody(&ballDef);

		b2CircleShape ballShape;
		ballShape.m_radius = 10.0f / 30.0f;

		b2FixtureDef ballFixture;
		ballFixture.shape = &ballShape;
		ballFixture.density = 1.0f;
		ballFixture.restitution = 0.8f;
		ballFixture.friction = 0.0f;
		ballBody->CreateFixture(&ballFixture);


		ballBody->SetLinearVelocity(b2Vec2(8.0f, -6.0f));
	}

	void CreateWall(float x, float y, float width, float height) {
		b2BodyDef wallDef;
		wallDef.position.Set(x / 30.0f, y / 30.0f);
		b2Body* wall = phyWorld->CreateBody(&wallDef);

		b2PolygonShape wallShape;
		wallShape.SetAsBox(width / 60.0f, height / 60.0f);

		b2FixtureDef wallFixture;
		wallFixture.shape = &wallShape;
		wallFixture.density = 0.0f;
		wallFixture.restitution = 0.5f;
		wall->CreateFixture(&wallFixture);
	}

	void UpdatePhysics() {
		phyWorld->Step(frameTime, 8, 3);
	}

	void DrawGame2() {
		DrawWall(400, 0, 800, 20);
		DrawWall(400, 600, 800, 20);
		DrawWall(0, 300, 20, 600);
		DrawWall(800, 300, 20, 600);

		sf::CircleShape ballShape(10.0f);
		ballShape.setFillColor(sf::Color::Red);
		b2Vec2 position = ballBody->GetPosition();
		ballShape.setPosition(position.x * 30.0f - 10.0f, position.y * 30.0f - 10.0f);
		wnd->draw(ballShape);
	}

	void DrawWall(float x, float y, float width, float height) {
		sf::RectangleShape wallShape(sf::Vector2f(width, height));
		wallShape.setFillColor(sf::Color::White);
		wallShape.setOrigin(width / 2, height / 2);
		wallShape.setPosition(x, y);
		wnd->draw(wallShape);
	}

	void SetZoom() {
		sf::View camara(sf::FloatRect(0, 0, 800, 600));
		wnd->setView(camara);
	}
};

int main() {
	Game game(800, 600, "Bouncing Ball Simulation");
	game.Loop();
	return 0;
}
