#include "Collisions.h"
#include "Character.h"
// initialise instance to nullptr
Collisions* Collisions::m_instance = nullptr;

Collisions::Collisions()
{

}

Collisions::~Collisions()
{
	m_instance = nullptr;
}

Collisions* Collisions::Instance()
{
	if (!m_instance)
	{
		m_instance = new Collisions;
	}

	return m_instance;
}

bool Collisions::Circle(Circle2D circle1, Circle2D circle2)
{
	// Calculate vector between the 2 objects
	Vector2D vec = Vector2D(
		(circle1.x - circle2.x),
		(circle1.y - circle2.y));

	// Find distance using pythag
	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	// Get collision radius of each character and add them
	double combined_distance = (circle1.radius + circle2.radius);

	// Check if the actual distance is less than the defined max distance
	return distance < combined_distance;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2)
{
	// Changed Code
	if (((rect1.x > rect2.x &&
		rect1.x < rect2.x + rect2.width) || (rect1.x + rect1.width > rect2.x && rect1.x + rect1.width < rect2.x + rect2.width)) &&
		((rect1.y > rect2.y &&
		rect1.y < rect2.y + rect2.height) || (rect1.y + rect1.width > rect2.y && rect1.y + rect1.width < rect2.y + rect2.height)))
	{
		return true;
	}
	return false;
}