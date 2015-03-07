# include "ELTentacle.h"


ELTentacle::ELTentacle
	(
	const ELEnemyName& Name,
	const ELEnemySize& Size,
	const Rect& hitRect,
	int HP,
	const Point& Pos
	):
		m_name(Name),
		m_size(Size),
		m_hitArea(hitRect),
		m_HP(HP),
		m_pos(Pos),
		m_startPos(Pos),
		m_state(ELEnemyState::Ready),
		m_face(ELEnemyFace::EnemyLeft),
		m_frameCount(0),
		m_oldFrameCount(0),
		m_jumpH(0),
		m_speed(0),
		m_oldState(ELEnemyState::Jumping),
		m_erased(false),
		m_MaxHp(HP)
{
	TextureAsset::Register(L"texELTentacle_fin",
		L"data/Elis/Enemy/Tentacle/fin.png");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 10;
	const int kMaxJunpH =20;
	const int kInterval = 70;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 5;

	const int kHigh = 400;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELTentacle::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{
		m_pos.y = Min(m_pos.y + kSpeed/2, m_startPos.y);

		if (m_frameCount>300)
		{
			m_HP = m_MaxHp;
			m_state = ELEnemyState::Ready;
			m_frameCount = 0;
		}

	}
	else if (m_state == ELEnemyState::Damaged && m_frameCount>30)
	{
		m_state = ELEnemyState::Staying;
	}
	else if (m_state == ELEnemyState::Ready)
	{
		m_pos.y = Max(m_pos.y-kSpeed,m_startPos.y-kHigh);

		if (m_pos.y==m_startPos.y-kHigh)
		{
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
		m_playerpos = playerpos;
	}
	else if (m_state == ELEnemyState::Staying)
	{
		
		if (m_frameCount > 120)
		{
			//attack.makeShoot(ELShooter::Enemy, ELShootType::ElisIce, m_pos, playerpos);

			m_state = ELEnemyState::Attacking;
			m_frameCount = 0;
		}
		m_playerpos = playerpos;
	}
	else if (m_state == ELEnemyState::Attacking)
	{
		if (m_frameCount > 30)
		{
			SoundAsset(L"sndElisIce").play();
			attack.makeShoot(ELShooter::Enemy, ELShootType::LongIce, m_pos, Point(m_playerpos.x,m_playerpos.y));
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
	}

	m_frameCount+=1;

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x / 2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

bool ELTentacle::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
{
	for(int i=1;i<foot.y-head.y;++i)
	{
		if(map.isFloor(foot.x,foot.y-i) && map.getFloorType(foot.x,foot.y-i)==ELFloorType::NormalFloor)
		{
			return true;
		}
	}

	return false;
}

void ELTentacle::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int r = 10;
	int yoffset = (m_startPos.y - m_pos.y) / 10;

	for (int i = 0; i<10; ++i)
	{
		Point pos(m_startPos.x, m_startPos.y - yoffset*i);

		Circle cir(pos-camerapos, r);
		cir.draw(Color(67, 92, 117));
		cir.drawFrame(0.5, 0.5, Palette::Black);
	}


	switch(m_state)
	{
	case ELEnemyState::Ready:
		{
			//TextureAsset(L"texELTentacle_fin").draw(m_pos-camerapos-Point(64,128));
		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64),
			Atan2(direction(m_pos, m_playerpos).y
			, direction(m_pos, m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64));
		
		break;
		}
	case ELEnemyState::Staying:
		{		
		//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));
		
		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0,64),
			Atan2(direction(m_pos, m_playerpos).y
			, direction(m_pos, m_playerpos).x))
			.draw(m_pos - camerapos - Point(0,64));
			
			break;
		}
	case ELEnemyState::Attacking:
		{
			//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));

			TextureAsset(L"texELTentacle_fin")
				.rotateAt(Point(0, 64),
				Atan2(direction(m_pos, m_playerpos).y
				, direction(m_pos, m_playerpos).x))
				.draw(m_pos - camerapos - Point(0, 64));

			break;
		}
	case ELEnemyState::Damaged:
		{
		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64),
			Atan2(direction(m_pos, -m_playerpos).y
			, direction(m_pos, -m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64),Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64),
			Atan2(direction(m_pos, -m_playerpos).y
			, direction(m_pos, -m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64), Alpha(155));
			break;
		}
	default:
		break;
	}

	if (m_state == ELEnemyState::Attacking)
	{
		int ring=60-m_frameCount;
		Circle(m_pos-camerapos, ring).drawFrame(1, 1, Palette::Blue);
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
	//Circle(m_pos - camerapos, 10).draw(Palette::Red);
}

ELEnemySize ELTentacle::getEnemySize() const
{
	return m_size;
}

Point ELTentacle::getPos() const
{
	return m_pos;
}

bool ELTentacle::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELTentacle::getDamage() const
{
	return kDamage;
}

Rect ELTentacle::getHitRect() const
{
	return m_hitRect;
}

void ELTentacle::giveDamage(int damage)
{
	if(damage >0 && m_state != ELEnemyState::Dead && m_state != ELEnemyState::Damaged)
	{
		m_HP = Max(m_HP-damage,0);

		SoundAsset(L"sndELEnemyDamaged").play();
		m_state = ELEnemyState::Damaged;
		m_frameCount = 0;
	}

	if(m_HP <= 0)
	{
		m_state = ELEnemyState::Dead;
	}
}

Point ELTentacle::getStartPos() const
{
	return m_startPos;
}

bool ELTentacle::isErased() const
{
	return m_erased;
}

bool ELTentacle::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}