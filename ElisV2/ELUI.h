# pragma once
# include <Siv3D.hpp>
# include "ELCommon.h"
# include "../Common.hpp"

struct SelectMagicIcon 
{
	SelectMagicIcon(const Circle &_cir,const ElisMagicType &_magicType)
	{
		cir = _cir;
		magicType = _magicType;
	}

	Circle cir;
	ElisMagicType magicType;
};

class ELUI
{
private:
	bool m_paused;

	ElisMagicType m_choicedMagic;

	Key m_keyRight;
	Key m_keyShift;
	Key m_keyLeft;
	Key m_keySpace;

	bool m_onPut;

	ELMenuState m_menustate;


public:
	ELUI();

	ELGameState pauseUpdate(const Point &cursorpos);

	bool getPaused()const {return m_paused;}

	ELPlayState update(const ElisMagicType &magicType);

	ElisMagicType getChoicedMagic() const {return m_choicedMagic;}
	void resetChoicedMagic() {m_choicedMagic = ElisMagicType::Normal;}

	void draw(const ElisMagicType &magicType, int life, int MP) const;

	void bossDraw(const ElisMagicType &magicType, int life, int maxlife) const;

	void pauseDraw(const Point &cursorpos);

	ELPlayState editMagicUpdate(const Point &cursorpos);

	void editMagicDraw(const Point &cursorpos);

	bool updateGameover(const Point &cursorpos);

	void gameoverDraw(const Point &cursorpos);

	ELMenuState getMenuState() const {return m_menustate;}
	void resetMenu() {m_menustate = ELMenuState::Base;}

	//////////////////////////////コンフィグ
	void changeKey(bool isRightHand)
	{
		if(isRightHand)
		{
			m_keyRight = Input::MouseR;
		}
		else
		{
			m_keyRight = Input::MouseR;
		}
	}
};