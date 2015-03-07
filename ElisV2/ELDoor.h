# pragma once
# include <Siv3D.hpp>
# include "ELCommon.h"

enum class ElisState;

class ELDoor
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	Point m_setPos;
	String m_name;
	Point m_destunationPoint;
	bool m_collided;
	Rect m_hitRect;
	Rect m_drawRect;

	Texture m_texDoor;

	Key m_keyUP;

public:
	ELDoor(const Point& setPos, const String& destinationName, const Point& destunationPoint, const String& texName);

	///////////////////‹¤—L
	bool update(const Rect& elisRect, const ElisState& elisState);

	void draw(const Point& camerapos)const;
	
	Rect getRect() const;

	Point getPos()const;

	String getStageName() const;

	Point getDestunationPoint() const;
	//////////////////////////
};