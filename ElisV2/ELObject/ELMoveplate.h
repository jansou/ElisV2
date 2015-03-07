# pragma once
# include <Siv3D.hpp>
# include "ELObject.h"
# include "../ELCommon.h"


class ELMoveplate : public ELObject
{
private:
	int m_frameCount;
	Point m_setPos1;
	Point m_setPos2;
	Point m_pos;

	Vec2 m_startpos;
	Vec2 m_endpos;

	String m_name;
	bool m_collided;
	Rect m_hitRect;

	Point m_Lpos;
	Point m_Rpos;

	static bool intersectBody(const Rect& body);

	bool m_hit;

	bool m_broke;
	bool m_erased;

	int m_face;//0Ç»ÇÁç∂,1Ç»ÇÁâEÇ…å¸Ç©Ç§

	Vec2 direction(const Vec2 pos1, const Vec2 &pos2)const
	{
		return Vec2(pos1 - pos2).normalize();
	}

public:
	ELMoveplate(const Point& setPos1, const Point &setPos2);
	///////////////////ã§óL
	void update();

	void draw(const Point& camerapos)const;

	bool isTouchable(const Point& head,const Point& foot);
	
	Rect getRect() const;

	Point getPos()const;

	bool attackUpdate(const ELShootType& shootType);

	bool getErased() const;

	Point objectEffect(const Point &head, const Point &foot);
	//////////////////////////
};