# include "ELDoor.h"

ELDoor::ELDoor(const Point& setPos, const String& destinationName, const Point& destunationPoint, const String& texName):
m_setPos(setPos),
m_name(destinationName),
m_destunationPoint(destunationPoint)
{
	m_texDoor = Texture(L"data/Elis/Door/" + texName);
	m_drawRect = Rect(m_setPos.x-m_texDoor.width/2,m_setPos.y-m_texDoor.height,m_texDoor.width,m_texDoor.height);
	m_hitRect = Rect(m_setPos.x-64,m_setPos.y-128,128,128);
}

namespace
{

};

bool ELDoor::update(const Rect& elisRect, const ElisState& elisState)
{
	if( (Input::KeyW.clicked || Input::KeyUp.clicked) && m_hitRect.intersects(elisRect) && elisState==ElisState::Staying)
	{
		return true;
	}

	return false;
	
}

void ELDoor::draw(const Point& camerapos)const
{
	m_texDoor.draw(m_drawRect.pos-camerapos);
}


Point ELDoor::getPos() const
{
	return m_setPos;
}

Rect ELDoor::getRect() const
{
	return m_hitRect;
}

String ELDoor::getStageName() const
{
	return m_name;
}

Point ELDoor::getDestunationPoint() const
{
	return m_destunationPoint;
}