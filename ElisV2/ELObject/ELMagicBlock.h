# pragma once
# include <Siv3D.hpp>
# include "ELObject.h"
# include "../ELCommon.h"

class ELMagicBlock : public ELObject
{
private:
	int m_frameCount;
	Point m_setPos;
	String m_name;
	bool m_collided;
	Rect m_hitRect;

	static bool intersectBody(const Rect& body);

	bool m_hit;

	bool m_Animed;
	bool m_broke;
	bool m_erased;
public:
	ELMagicBlock(const Point& setPos);
	///////////////////‹¤—L
	void update();

	void draw(const Point& camerapos)const;

	bool isTouchable(const Point& head,const Point& foot);
	
	Rect getRect() const;

	Point getPos()const;

	bool attackUpdate(const ELShootType& shootType);

	bool getErased() const;

	Point objectEffect(const Point &head, const Point &foot){return Point(0,0);}
	//////////////////////////
};