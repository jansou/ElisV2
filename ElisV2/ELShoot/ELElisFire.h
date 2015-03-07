# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "ELShoot.h"
# include "../ELCommon.h"

class ELElisFire : public ELShoot
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	ELShooter m_shooter;
	Vec2 m_beginpos;
	Vec2 m_endpos;
	Point m_pos;
	Rect m_shootRect;
	Rect m_hitRect;
	ELShootType m_shootType;
	bool intersectBody(const Point& foot,const Point& head, const ELMap& map, const ELFloorType& floorType);
	int m_damege;
	bool m_collided;
	bool m_erased;

	Line m_line;

	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);
	Vec2 direction()const
	{
		return Vec2(m_endpos - m_beginpos).normalize();
	}
public:
	ELElisFire(const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos);

	///////////////////���L
	void update(const ELMap& map);

	void draw(const Point& camerapos)const;

	Point getPos()const;
	
	bool isHit(const Rect &body);

	ELShooter getShooter() const;

	int getDamage() const;

	bool isErased() const;

	ELShootType getShootType() const;

	void giveErase();
	//////////////////////////
};