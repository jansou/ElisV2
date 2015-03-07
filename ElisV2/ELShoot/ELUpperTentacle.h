# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "ELShoot.h"
# include "../ELCommon.h"

class ELUpperTentacle : public ELShoot
{
private:
	int m_frameCount;
	int m_oldFrameCount;
	ELShooter m_shooter;
	Vec2 m_beginpos;
	Vec2 m_endpos;
	Vec2 m_pos;
	Rect m_shootRect;
	Rect m_hitRect;
	ELShootType m_shootType;

	int m_damege;
	bool m_collided;
	bool m_erased;
	bool m_staying;

	static bool intersectBody(const Point& foot,const Point& head, const ELMap& map);
	Vec2 direction()const
	{
		return Vec2(m_endpos - m_beginpos).normalize();
	}

	Vec2 m_nodes[10];

	bool m_preActed;

	Vec2 m_targetPos;
	Vec2 m_preTargetPos;
	bool m_rising;

public:
	ELUpperTentacle(const ELShootType& shootType,
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