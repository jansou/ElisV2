# pragma once
# include <Siv3D.hpp>

class ELFade
{
private:
	int m_framecount;

public:
	ELFade();

	bool FadeOut();

	bool CircleFadeOut();

	void CircleFadeOutDraw(const Point& pos);

	void resetCount() {m_framecount = 0;}
};