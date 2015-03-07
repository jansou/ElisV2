# include "ELAttack.h"

ELAttack::ELAttack()
{
	//エリスの魔法用
	TextureAsset::Register(L"texElisShot1",
		L"data/Elis/Effect/Elis/elisShot1.png");
	TextureAsset::Register(L"texElisShot1_erase",
		L"data/Elis/Effect/Elis/elisShot1_erase.png");

	TextureAsset::Register(L"texElisShot2",
		L"data/Elis/Effect/Elis/elisShot2.png");
	TextureAsset::Register(L"texElisShot2_erase",
		L"data/Elis/Effect/Elis/elisShot2_erase.png");

	TextureAsset::Register(L"texElisShot3",
		L"data/Elis/Effect/Elis/elisShot3.png");
	TextureAsset::Register(L"texElisShot3_erase",
		L"data/Elis/Effect/Elis/elisShot3_erase.png");

	TextureAsset::Register(L"texElisIce",
		L"data/Elis/Effect/Elis/elisIce.png");
	TextureAsset::Register(L"texElisIce_erase",
		L"data/Elis/Effect/Elis/elisIce_erase.png");
		//
	TextureAsset::Register(L"texELsword",L"data/Elis/Effect/Darkbrave/sword.png");
}

void ELAttack::makeShoot(
		const ELShooter& shooter,
		const ELShootType& shootType,
		const Point& beginpos,
		const Point& endpos)
{
	switch(shootType)
	{
	case ELShootType::ElisNormalShot1:
		{
		m_shoots.emplace_back(
			std::make_shared<ELElisShoot1>(
			shootType,
			shooter,
			beginpos,
			endpos));
		break;
		}
	case ELShootType::ElisNormalShot2:
		{
		m_shoots.emplace_back(
			std::make_shared<ELElisShoot2>(
			shootType,
			shooter,
			beginpos,
			endpos));
		break;
		}
	case ELShootType::ElisNormalShot3:
		{
		m_shoots.emplace_back(
			std::make_shared<ELElisShoot3>(
			shootType,
			shooter,
			beginpos,
			endpos));
		break;
		}
	case ELShootType::ElisIce:
		{
			m_shoots.emplace_back(
			std::make_shared<ELElisIce>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::ElisFire:
		{
			m_shoots.emplace_back(
				std::make_shared<ELElisFire>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::ElisWind:
		{
			m_shoots.emplace_back(
				std::make_shared<ELElisWind>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::Arrow:
		{
			m_shoots.emplace_back(
			std::make_shared<ELArrow>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::CurveAxe:
		{
			m_shoots.emplace_back(
			std::make_shared<ELCurveAxe>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::SwordRain:
		{
			m_shoots.emplace_back(
				std::make_shared<ELSwordrain>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::Firebreath:
		{
			m_shoots.emplace_back(
				std::make_shared<ELFirebreath>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::Rockfall:
		{
			m_shoots.emplace_back(
				std::make_shared<ELRockfall>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::UpperTentacle:
		{
			m_shoots.emplace_back(
				std::make_shared<ELUpperTentacle>(
			shootType,
			shooter,
			beginpos,
			endpos));
			break;
		}
	case ELShootType::LongIce:
	{
		m_shoots.emplace_back(
			std::make_shared<ELLongIce>(
			shootType,
			shooter,
			beginpos,
			endpos));
		break;
	}
	default:
		break;
	}
}

namespace
{
	const int kXoffset = 200;
	const int kYoffset = 200;
};

void ELAttack::update(const ELMap& map, const Point& camerapos)
{
	for(size_t i =0; i<m_shoots.size();++i)
	{
		if(m_shoots[i]->isErased() || !incamera(m_shoots[i]->getPos(),camerapos))
		{
			erase(i);
		}
	}

	for(auto& shoot:m_shoots)
	{
		shoot->update(map);
	}
}

void ELAttack::erase(int index)
{
	m_shoots.erase(m_shoots.begin()+index);
}

void ELAttack::giveErase(int index)
{
	m_shoots[index]->giveErase();
}

void ELAttack::draw(const Point& camerapos)const
{
	for(const auto& shoot : m_shoots)
	{
		shoot->draw(camerapos);
	}
}


bool ELAttack::incamera(const Point& pos, const Point& camerapos)const
{
	return pos.x > camerapos.x - kXoffset
			&& pos.x < camerapos.x + 1280 +kXoffset
			&& pos.y > camerapos.y - kYoffset
			&& pos.y <camerapos.y + 720 + kYoffset;
}

int ELAttack::getDamage(const Rect &body, const ELShooter &shooter) const
{
	int damage = 0;

	for(const auto& shoot : m_shoots)
	{
		if(shooter != shoot->getShooter() && shoot->isHit(body))
		{
			damage = shoot->getDamage();
			return damage;
		}
	}

	return damage;
}

int ELAttack::objectHit(const Rect& body)
{
	for(size_t i=0;i<m_shoots.size();++i)
	{
		if(m_shoots[i]->isHit(body))
		{
			return i;
		}
	}
	
	return -1;
}

ELShootType ELAttack::getShootType(int index) const
{
	return m_shoots[index]->getShootType();
}