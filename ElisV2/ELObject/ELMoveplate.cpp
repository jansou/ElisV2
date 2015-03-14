# include "ELMoveplate.h"

ELMoveplate::ELMoveplate(const Point& setPos1, const Point &setPos2):
	m_setPos1(setPos1),
	m_setPos2(setPos2),
	m_pos(setPos1),
	m_startpos(setPos1),
	m_endpos(setPos2),
	m_erased(false),
	m_frameCount(0)
{
	//0,0,128,12
	m_hitRect = Rect(m_pos.x-64,m_pos.y-6,128,12);

	

	TextureAsset::Register(L"texELmoveplate", L"data/Elis/Object/movePlate/movePlate.png");
}

namespace
{
	const bool kTouchable = true;
	const int kAnimeTiming = 8;
	const int kSpeed = 2;
};

void ELMoveplate::update()
{
	if(m_pos == m_setPos1)
	{
		m_startpos = m_setPos1;
		m_endpos = m_setPos2;

	}
	else if(m_pos == m_setPos2)
	{
		m_startpos = m_setPos2;
		m_endpos = m_setPos1;

	}

	const Vec2 dir = direction(m_endpos,m_startpos);

	m_pos += Point(int(dir.x*kSpeed),int(dir.y*kSpeed));

	m_hitRect = Rect(m_pos.x-64,m_pos.y-6,128,12);
}

void ELMoveplate::draw(const Point& camerapos)const
{
	TextureAsset(L"texELmoveplate").draw(m_hitRect.pos-camerapos);
	//m_hitRect.drawFrame(10,10,Palette::Aliceblue);
}

bool ELMoveplate::intersectBody(const Rect& body)
{
	return false;
}

Point ELMoveplate::getPos() const
{
	return m_pos;
}

Rect ELMoveplate::getRect() const
{
	return m_hitRect;
}

bool ELMoveplate::isTouchable(const Point& head,const Point& foot) 
{
	//return Line(foot,head).intersects(m_hitRect);
	int n =std::abs(foot.y-head.y);

	return Line(foot, head).intersects(m_hitRect);

	/*
	for(int i=0;i<=n;++i)
	{
		if(Point(foot.x,foot.y - i).intersects(m_hitRect))
		{
			return true;	
		}
	}
	*/
	return false;
}

bool ELMoveplate::attackUpdate(const ELShootType& shootType)//Á–Å‚·‚é‚Æ‚«‚Ítrue
{
	return true;
}

bool ELMoveplate::getErased() const
{
	return m_erased;
}
Point ELMoveplate::objectEffect(const Point &head, const Point &foot)
{
	const Vec2 dir = direction(m_endpos,m_startpos);

	return Point(int(dir.x*kSpeed),int(dir.y*kSpeed));

}