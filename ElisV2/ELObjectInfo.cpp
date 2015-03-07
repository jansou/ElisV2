# include "ELObjectInfo.h"

ELObjectInfo::ELObjectInfo()
{
	TextureAsset::Register(L"texELblock",L"data/Elis/Object/block/block.png");
	TextureAsset::Register(L"texELblock_erase",L"data/Elis/Object/block/block_erase.png");
	SoundAsset::Register(L"sndELBlockBreak",L"data/Elis/SE/Object/blockBreak.mp3");

	TextureAsset::Register(L"texELMagicBlock",L"data/Elis/Object/magicBlock/magicBlock.png");

	TextureAsset::Register(L"texELMagicBlock_Anime",L"data/Elis/Object/magicBlock/magicBlock_Anime.png");
}

namespace
{
	const int kXoffset = 300;
	const int kYoffset = 300;
};

void ELObjectInfo::addObject(const ELObjectType &objectType, Point &pos)
{
	switch (objectType)
	{
	case ELObjectType::magicBlock:
		m_objects.emplace_back(std::make_shared<ELMagicBlock>(pos));
		break;
	case ELObjectType::block:
		m_objects.emplace_back(std::make_shared<ELBlock>(pos));
		break;
	default:
		break;
	}
			
}

void ELObjectInfo::update(const Point& camerapos)
{
	for(auto& object : m_objects)
	{
		if(incamera(object->getPos(),camerapos))
			object ->update();
	}

	m_objects.erase(std::remove_if(m_objects.begin(),m_objects.end(),[](std::shared_ptr<ELObject> ptr){return ptr->getErased();}),m_objects.end());
}

bool ELObjectInfo::loadObjectInfo(const String& mapName, const String& stageName)
{
	const CSVReader csv(L"data/Elis/Object/" +mapName +L"/"+ stageName +L".csv");
	
	if(!csv) return false;
	
	for(int i=0;;++i)
	{
		String SName = csv.get<String>(i,0).lower();

		if(SName.isEmpty)
		{
			break;
		}
		else if(SName==L"block")
		{
			const Point p = Point(csv.get<int>(i,1),csv.get<int>(i,2));

			m_objects.emplace_back(std::make_shared<ELBlock>(p));
		}
		else if(SName==L"moveplate")
		{
			const Point p1 = Point(csv.get<int>(i,1),csv.get<int>(i,2));
			const Point p2 = Point(csv.get<int>(i,3),csv.get<int>(i,4));

			m_objects.emplace_back(std::make_shared<ELMoveplate>(p1,p2));
		}

	}

	return true;
}

void ELObjectInfo::draw(const Point& camerapos) const
{
	
	for(const auto &object : m_objects)
	{
		if(incamera(object->getPos(), camerapos))
			object->draw(camerapos);
	}
	
}

bool ELObjectInfo::incamera(const Point& pos, const Point& camerapos) const
{
	return pos.x > camerapos.x - kXoffset
			&& pos.x < camerapos.x + 1280 +kXoffset
			&& pos.y > camerapos.y - kYoffset
			&& pos.y <camerapos.y + 720 + kYoffset;
}

bool ELObjectInfo::isTouchable(const Point& head,const Point& foot, const Point &camerapos)
{
	bool b = false;

	for(const auto& object : m_objects)
	{
		if(incamera(object->getPos(), camerapos) && object->isTouchable(head,foot))
		{
			b = true;
		}
	}

	return b;
}

void ELObjectInfo::clearObjects()
{
	m_objects.clear();
}

void ELObjectInfo::attackUpdate(ELAttack& attack, const Point& camerapos)
{
	for(auto& object : m_objects)
	{
		if(incamera(object->getPos(),camerapos))
		{
			int index = attack.objectHit(object->getRect());
			if(index != -1)
			{
				if(object->attackUpdate(attack.getShootType(index)))//Õ“Ë‚Å–‚–@‚ª‘Å‚¿Á‚³‚ê‚é‚Æ‚«‚Ítrue
				{
					attack.giveErase(index);
				}
			}
		}
	}
}

Point ELObjectInfo::objectEffect(const Point &head ,const Point &foot ,const Point &camerapos)
{
	Point p(0,0);

	for(const auto& object : m_objects)
	{
		if(incamera(object->getPos(), camerapos) && object->isTouchable(head,foot))
		{
			p += object->objectEffect(head,foot);
		}
	}

	return p;
}
