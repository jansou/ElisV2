# pragma once
# include <Siv3D.hpp>

class ELFade
{
private:
	int m_framecount;

	bool m_circleFadeInFlag;

public:
	ELFade();

	bool FadeOut();

	bool CircleFadeOut();

	void CircleFadeOutDraw(const Point& pos);

	bool ELFade::CircleFadeIn();

	void CircleFadeInDraw(const Point& pos);

	void resetCount() {m_framecount = 0;}

	void setCircleFadeInFlag(bool b)
	{
		m_circleFadeInFlag = b;
	}

	bool getCircleFadeInFlag(){ return m_circleFadeInFlag; }
};