# pragma once
# include <Siv3D.hpp>
# include "ELCommon.h"
# include "ELMap.h"
# include "ELObjectInfo.h"


struct ELItemInfo
{
	ELItemInfo(const ELItemType &_itemType, const Rect &_setRect, bool _erase)
	{
		itemType = _itemType;

		setRect = _setRect;

		erase = _erase;
		framecount = 0;
	}

	ELItemType itemType;

	Rect setRect;

	bool erase;//trueÇæÇ∆éûä‘åoâﬂÇ≈è¡ñ≈

	int framecount;
};

class ELItem
{
private:
	bool incamera(const Point& pos, const Point& camerapos)const;

	std::vector<ELItemInfo> m_items;

	int m_index;
public:
	ELItem();

	void loadItem(const String& name,const String& stageName);

	bool update(const Rect& playerRect, const ELMap& map, const ELObjectInfo& object, const Point &camerapos, const Rect& cursorRect);

	void draw(const Point &camerapos) const;

	void makeItem(const ELItemType &itemType, const Point &setpos, bool erase);

	ELItemType getItemType() const;

	void erase(int num);

	int getIndex() const;

	void clearItem();
};