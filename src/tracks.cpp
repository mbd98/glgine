#include <tracks.hpp>

Tracks::Tracks()
{
	segments[0] = new Reusable(new Model("track_straight"));
	segments[1] = new Reusable(new Model("track_sbend"));
	segments[2] = new Reusable(new Model("track_slight"));
	segments[3] = new Reusable(new Model("track_180"));

	segments[0]->insertTransform(glm::mat4(1.0f));
}

Tracks::~Tracks()
{
	for (auto &s : segments)
		delete s;
}

void Tracks::render(GLuint shader)
{
	for (auto &s : segments)
		s->render(shader);
}
