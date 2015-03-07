# pragma once
# include <Siv3D.hpp>

# include "ELCommon.h"



class ELTitle
{
private:
	ELGameState m_nextState;
	Key m_MouseLeft;
	bool m_onPut;
	ELTitleMode m_titleMode;//É^ÉCÉgÉãÇÃèÛë‘

	bool updateTitle(const Point &cursorpos);
	bool updateConfig(const Point &cursorpos);

	bool m_isRightHand;

	bool m_pressed;
public:
	ELTitle();

	bool update(const Point& cursorpos);

	void init();

	void draw(const Point& cursorpos);

	void rereaseTex();

	bool getIsRightHand() const
	{return m_isRightHand;}

	ELTitleMode getTitleMode() const
	{
		return m_titleMode;
	}
};