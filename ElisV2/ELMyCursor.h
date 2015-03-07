# pragma once
# include <Siv3D.hpp>

class ELMyCursor
{
private:
	Point m_cursorPos;

	Point m_offSet;

public:
	ELMyCursor();
	bool init();
	void update();
	void draw();
	Point getCursorPos() const;
	Rect getCursorRect() const
	{
		return Rect(m_cursorPos - m_offSet, m_offSet * 2);
	}
};
