# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "ELShoot.h"
# include "../ELCommon.h"

class ELElisWind : public ELShoot
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	ELShooter m_shooter;
	Vec2 m_beginpos;
	Vec2 m_endpos;
	Vec2 m_pos;
	Rect m_shootRect;
	Circle m_hitCir;
	ELShootType m_shootType;

	bool m_burned;

	int m_damege;
	bool m_collided;
	bool m_erased;

	double m_rad;

	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);
	Vec2 direction()const
	{
		return Vec2(m_endpos - m_beginpos).normalize();
	}
public:
	ELElisWind(const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos);

	///////////////////‹¤—L
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