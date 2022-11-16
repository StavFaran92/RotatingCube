
#include <vector>
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <functional>

#define WIDTH 800
#define HEIGHT 600
#define PI 3.14159265359f

#define logInfo(msg) std::cout << msg << std::endl;
#define logDebug(msg) std::cout << msg << std::endl;
#define logWarning(msg) std::cout << msg << std::endl;
#define logError(msg) std::cout << msg << std::endl;

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

	void mulVecByMat(Vertex& v, const glm::mat4& mat)
	{
		glm::vec4 tempVec(v.x, v.y, v.z, 1);

		tempVec = mat * tempVec;

		tempVec.x /= tempVec.w;
		tempVec.y /= tempVec.w;
		tempVec.z /= tempVec.w;

		v.x = tempVec.x;
		v.y = tempVec.y;
		v.z = tempVec.z;
	}

	void applyToVec(Vertex& v, std::function<void(Vertex& v)> func)
	{
		func(v);
	}

	void applyToTri(Triangle& tri, std::function<void(Vertex& v)> func)
	{
		applyToVec(tri.v0, func);
		applyToVec(tri.v1, func);
		applyToVec(tri.v2, func);
	}

	void applyToMesh(Mesh& mesh, std::function<void(Vertex& v)> func)
	{
		for (auto& tri : mesh.tris)
		{
			applyToTri(tri, func);
		}
	}

}

playground3d::Mesh originalData
{
	{
		// Front face
		{
			{-1, -1, 1},
			{-1, 1, 1},
			{1, 1, 1},
		},
		{
			{-1, -1, 1},
			{1, 1, 1},
			{-1, 1, 1},
		},

		// Back face
		{
			{-1, -1, -1},
			{-1, 1, -1},
			{1, 1, -1},
		},
		{
			{-1, -1, -1},
			{1, 1, -1},
			{-1, 1, -1},
		},

		// Right face
		{
			{1, -1, -1},
			{1, -1, 1},
			{1, 1, 1},
		},
		{
			{1, -1, -1},
			{1, 1, 1},
			{1, 1, -1},
		},

		// Left face
		{
			{-1, -1, -1},
			{-1, -1, 1},
			{-1, 1, 1},
		},
		{
			{-1, -1, -1},
			{-1, 1, 1},
			{-1, 1, -1},
		},

		// Top face
		{
			{-1, 1, -1},
			{1, 1, -1},
			{1, 1, 1},
		},
		{
			{-1, 1, -1},
			{1, 1, 1},
			{-1, 1, 1},
		},

		// Bottom face
		{
			{-1, -1, -1},
			{1, -1, -1},
			{1, -1, 1},
		},
		{
			{-1, -1, -1},
			{1, -1, 1},
			{-1, -1, 1},
		},
	}
};

namespace p3d = playground3d;

int main()
{
	sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Playground 3D", sf::Style::Close);

	while (window.isOpen())
	{
		window.clear();

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

		static float angle = 0;
		angle += 0.005f;
		float angleRad = angle / 180 * PI;

		p3d::Mesh data = originalData;

		// Model
		glm::mat4 model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
		model = glm::translate(model, glm::vec3(0, 0, -5));
		model = glm::rotate(model, angleRad, glm::vec3(0,1,0));

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(cos(angleRad), 1, sin(angleRad)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		// Projection
		glm::mat4 projection = glm::perspective( glm::radians(45.0f), 4.0f / 3.0f,  0.1f, 100.0f);

		// MVP
		glm::mat4 mvp = projection * view * model;

		// Project vertices
		p3d::applyToMesh(data, [&](p3d::Vertex& v) {
			p3d::mulVecByMat(v, mvp);
			});

		// Offset and scale
		p3d::applyToMesh(data, [](p3d::Vertex& v) {
			v.x = (v.x + 1) / 2;
			v.y = (v.y + 1) / 2;

			v.x *= WIDTH;
			v.y *= HEIGHT;
			});

		p3d::drawMesh(data, window);

		// Finally, display the rendered frame on screen
		window.display();
	}

	return 0;
}