
#include <vector>
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define WIDTH 800
#define HEIGHT 600

namespace playground3d
{

	struct Vertex
	{
		float x, y, z;
	};

	struct Triangle
	{
		Vertex v0, v1, v2;
	};

	struct Mesh
	{
		std::vector<Triangle> tris;
	};

	void drawLine(float x1, float y1, float x2, float y2, sf::RenderTarget& window)
	{
		sf::Vertex line[2];
		line[0].position = sf::Vector2f(x1, y1);
		line[0].color = sf::Color::Red;
		line[1].position = sf::Vector2f(x2, y2);
		line[1].color = sf::Color::Red;

		window.draw(line, 2, sf::Lines);
	}

	void drawTriangle(const Triangle& tri, sf::RenderTarget& window)
	{
		drawLine(tri.v0.x, tri.v0.y, tri.v1.x, tri.v1.y, window);
		drawLine(tri.v1.x, tri.v1.y, tri.v2.x, tri.v2.y, window);
		drawLine(tri.v2.x, tri.v2.y, tri.v0.x, tri.v0.y, window);
	}

	void drawMesh(const Mesh& mesh, sf::RenderTarget& window)
	{
		for (const auto& tri : mesh.tris)
		{
			drawTriangle(tri, window);
		}
	}

}

namespace p3d = playground3d;

int main()
{
	sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Playground 3D", sf::Style::Close);

	p3d::Mesh data = {
		{
			{
				{10, 10, 0},
				{20, 20, 0},
				{30, 10, 0}
			},
			{
				{50, 50, 0},
				{60, 60, 0},
				{70, 50, 0}
			}
		}
	};

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		100.0f             // Far clipping plane. Keep as little as possible.
	);

	glm::vec4 vec

	while (window.isOpen())
	{
		// Process events
		for (sf::Event event; window.pollEvent(event);)
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		p3d::drawMesh(data, window);
		//p3d::drawLine(data.tris[0].v0.x, data.tris[0].v0.y, data.tris[0].v1.x, data.tris[0].v1.y, window);

		// Finally, display the rendered frame on screen
		window.display();
	}

	return 0;
}