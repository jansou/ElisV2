# pragma once
# include <Siv3D.hpp>
# include "ELObject\ELObject.h"
# include "ELObject\ELBlock.h"
# include "ELObject\ELMagicBlock.h"
# include "ELObject\ELMoveplate.h"


# include "ELCommon.h"
# include "ELAttack.h"


class ELObjectInfo
{
private:
	bool incamera(const Point& pos, const Point& camerapos) const;

	std::vector<std::shared_ptr<ELObject>> m_objects;

public:
	ELObjectInfo();
	
	bool loadObjectInfo(const String& mapName, const String& stageName);

	void update(const Point& camerapos);

	void draw(const Point& camerapos)const;

	bool isTouchable(const Point& head,const Point& foot, const Point &camerapos);

	void clearObjects();

	void attackUpdate(ELAttack& attack, const Point& camerapos);

	void addObject(const ELObjectType &objectType, Point &pos);

	Point objectEffect(const Point &head ,const Point &foot ,const Point &camerapos);
};