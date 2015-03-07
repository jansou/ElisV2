# pragma once
# include <Siv3D.hpp>
# include "../ELCommon.h"

class ELObject
{
public:
	virtual ~ELObject(){}

	virtual void update() = 0;

	virtual void draw(const Point& camerapos) const = 0;

	virtual Point getPos() const = 0; 

	virtual Rect getRect() const = 0;

	virtual bool isTouchable(const Point& head,const Point& foot) = 0;

	virtual bool attackUpdate(const ELShootType& shootType) = 0;

	virtual bool getErased() const = 0;

	virtual Point objectEffect(const Point &head, const Point &foot) = 0;
};