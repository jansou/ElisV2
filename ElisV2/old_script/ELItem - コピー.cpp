# include "ELItem.h"

ELItem::ELItem()
{
	TextureAsset::Register(L"texELLifeUpS",L"data/Elis/Item/lifeUpS.png");
	TextureAsset::Register(L"texELLifeUpM",L"data/Elis/Item/lifeUpM.png");
	TextureAsset::Register(L"texELLifeUpL",L"data/Elis/Item/lifeUpL.png");

	TextureAsset::Register(L"texELMagicUpS",L"data/Elis/Item/magicUpS.png");
	TextureAsset::Register(L"texELMagicUpM",L"data/Elis/Item/magicUpM.png");
	TextureAsset::Register(L"texELMagicUpL",L"data/Elis/Item/magicUpL.png");
}

namespace
{
	const int kXoffset = 300;
	const int kYoffset = 300;

	Rect getRect(const ELItemType &itemType,const Point &pos)
	{
		switch (itemType)
		{
		case ELItemType::LifeUpS:
			return Rect(pos.x-16+9, pos.y-32+18,14,13);
			break;

		case ELItemType::LifeUpM:
			return Rect(pos.x-16+6, pos.y-32+11,20,20);
			break;

		case ELItemType::LifeUpL:
			return Rect(pos.x-16+0, pos.y-32+8,32,23);
			break;

		case ELItemType::MagicUpS:
			return Rect(pos.x-16+10, pos.y-32+10,12,20);
			break;

		case ELItemType::MagicUpM:
			return Rect(pos.x-16+6, pos.y-32+2,20,28);
			break;

		case ELItemType::MagicUpL:
			return Rect(pos.x-16+5, pos.y-32+0,22,32);
			break;
		default:
			return Rect(0,0,0,0);
			break;
		}
	}

	int keraseTime = 400;
	int kpreEraseTime = 300;
};

void ELItem::loadItem(const String& name,const String& stageName)
{
	const CSVReader csv(L"data/Elis/Item/" +name +L"/"+ stageName +L".csv");
	
	if(!csv) return;

	for(int i=0;;++i)
	{
		String SName = csv.get<String>(i,0).lower();

		if(SName.isEmpty)
		{
			break;
		}
		
		if(SName==L"lifeups")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::LifeUpS,p);
			
			m_items.emplace_back(ELItemType::LifeUpS,r,false);
		}
		else if(SName==L"lifeupm")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::LifeUpM,p);

			m_items.emplace_back(ELItemType::LifeUpM,r,false);
		}
		else if(SName==L"lifeupl")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::LifeUpL,p);

			m_items.emplace_back(ELItemType::LifeUpL,r,false);
		}
		else if(SName==L"magicups")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::MagicUpS,p);
			
			m_items.emplace_back(ELItemType::MagicUpS,r,false);
		}
		else if(SName==L"magicupm")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::MagicUpM,p);

			m_items.emplace_back(ELItemType::MagicUpM,r,false);
		}
		else if(SName==L"magicupl")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			Rect r = getRect(ELItemType::MagicUpL,p);

			m_items.emplace_back(ELItemType::MagicUpL,r,false);
		}
	}
}

bool ELItem::incamera(const Point& pos, const Point& camerapos)const
{
	return pos.x > camerapos.x - kXoffset
			&& pos.x < camerapos.x + 1280 +kXoffset
			&& pos.y > camerapos.y - kYoffset
			&& pos.y <camerapos.y + 720 + kYoffset;
}

void ELItem::makeItem(const ELItemType &itemType, const Point &setpos, bool erase)
{
	m_items.emplace_back(itemType,getRect(itemType, setpos),erase);
}

bool ELItem::update(const Rect& playerRect, const ELMap &map, const ELObjectInfo &object, const Point &camerapos, const Rect& cursorRect)
{
	for(size_t i = 0;i<m_items.size();++i)
	{
		if(incamera(m_items[i].setRect.pos,camerapos))
			m_items[i].framecount += 1;

		if(m_items[i].erase && m_items[i].framecount > keraseTime)
			erase(i);
	}

	for(size_t i = 0;i<m_items.size();++i)
	{
		if(incamera(m_items[i].setRect.pos,camerapos))
		{
			if (playerRect.intersects(m_items[i].setRect) || Rect(cursorRect.pos + camerapos,cursorRect.size).intersects(m_items[i].setRect))
			{
				m_index = i;
				return true;
			}

			m_items[i].framecount += 1;
		}
	}

	return false;
}

void ELItem::draw(const Point &camerapos) const
{
	for(const auto &item : m_items)
	{
		if(incamera(item.setRect.pos,camerapos))
		{
			int a = 255;

			if(item.erase && item.framecount > kpreEraseTime)
				a -= 100;

			switch (item.itemType)
			{
			case ELItemType::LifeUpS:
				TextureAsset(L"texELLifeUpS").drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			case ELItemType::LifeUpM:
				TextureAsset(L"texELLifeUpM").drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			case ELItemType::LifeUpL:
				TextureAsset(L"texELLifeUpL")(0,32*((item.framecount/8)%6),32,32).drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			case ELItemType::MagicUpS:
				TextureAsset(L"texELMagicUpS").drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			case ELItemType::MagicUpM:
				TextureAsset(L"texELMagicUpM").drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			case ELItemType::MagicUpL:
				TextureAsset(L"texELMagicUpL")(0,32*((item.framecount/8)%6),22,32).drawAt(item.setRect.center-camerapos,Alpha(a));
				break;
			default:
				break;
			}
		}
	}
}

ELItemType ELItem::getItemType() const
{
	return m_items[m_index].itemType;
}

void ELItem::erase(int num)
{
	m_items.erase(m_items.begin()+num);
}

int ELItem::getIndex() const
{
	return m_index;
}

void ELItem::clearItem()
{
	m_items.clear();
}