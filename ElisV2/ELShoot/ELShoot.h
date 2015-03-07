# pragma once
# include <Siv3D.hpp>
# include "../ELMap.h"
# include "../ELCommon.h"


class ELShoot
{
public:
	virtual ~ELShoot(){}

	virtual void update(const ELMap& map) = 0;

	virtual void draw
		(
		const Point& camerapos
		) const = 0;

	virtual Point getPos() const = 0;

	virtual ELShooter getShooter() const = 0;

	virtual bool isHit(const Rect &body) = 0;

	virtual int getDamage() const = 0;

	virtual bool isErased() const = 0;

	virtual ELShootType getShootType() const = 0;

	virtual void giveErase() = 0;
};
