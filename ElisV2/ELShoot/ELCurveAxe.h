# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "ELShoot.h"
# include "../ELCommon.h"


class ELCurveAxe : public ELShoot
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	ELShooter m_shooter;
	Vec2 m_beginpos;
	Vec2 m_pos;
	Point m_Speed;
	Rect m_shootRect;
	Rect m_hitRect;
	ELShootType m_shootType;
	double m_rad;

	int m_damege;
	bool m_collided;
	bool m_erased;

	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);

public:
	ELCurveAxe(const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Point& endpos);

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