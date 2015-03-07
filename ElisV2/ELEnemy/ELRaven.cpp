# include "ELRaven.h"


ELRaven::ELRaven
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
		m_state(ELEnemyState::Staying),
		m_face(ELEnemyFace::EnemyLeft),
		m_frameCount(0),
		m_oldFrameCount(0),
		m_oldState(ELEnemyState::Jumping),
		m_erased(false),
		m_attacked(false)
{
	TextureAsset::Register(L"texELRaven_stay",
		L"data/Elis/Enemy/Raven/raven_stay.png");
	TextureAsset::Register(L"texELRaven_damaged",
		L"data/Elis/Enemy/Raven/raven_damaged.png");
	TextureAsset::Register(L"texELRaven_attack",
		L"data/Elis/Enemy/Raven/raven_attack.png");
	TextureAsset::Register(L"texELRaven_dead",
		L"data/Elis/Enemy/Raven/raven_dead.png");

	SoundAsset::Register(L"sndELravenCry",L"data/Elis/SE/Enemy/ravenCry.mp3");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kAttackSpeed = 5;
	
	const int kInterval = 80;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 2;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const Point kHead(15,55);

	const int kDistance = 300;
};

void ELRaven::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{

		if(m_frameCount == kAnimeTiming*5-1)
		{
			int a = Random(1,10);
			if(a >= 1 && a <= 2)
				item.makeItem(ELItemType::LifeUpM,m_hitRect.center,true);
			if(a >= 6 && a <= 8)
				item.makeItem(ELItemType::MagicUpM,m_hitRect.center,true);

			m_erased = true;

			return;
		}

		m_frameCount+=1;

		return;
	}

	/////çsìÆ
	if(m_state == ELEnemyState::Attacking)
	{
		if(m_pos.y >= m_tarPos.y && m_tarPos.x == m_pos.x)
		{
			m_state = ELEnemyState::Staying;
			m_attacked = true;
			m_frameCount = 0;
		}
	}
	else if(m_state == ELEnemyState::Staying && m_frameCount>kInterval && std::abs(playerpos.x - m_pos.x)<500)
	{
		m_state = ELEnemyState::Attacking;
		m_tarPos = playerpos;
		m_attacked = false;
		m_frameCount = 0;
		if(SoundAsset(L"sndELravenCry").isPlaying)	SoundAsset(L"sndELravenCry").stop();

		SoundAsset(L"sndELravenCry").play();

	}
	else if(m_state == ELEnemyState::Staying && (m_pos.y == m_startPos.y || m_frameCount >60 ))
	{
		m_attacked = false;
	}
	else if(m_state != ELEnemyState::Damaged || m_frameCount>30)
	{
		m_state = ELEnemyState::Staying;
	}

	////äÁÇÃå¸Ç´
	if(m_state == ELEnemyState::Staying && !m_attacked)
	{
		if(playerpos.x-m_pos.x>0)
		{
			m_face = ELEnemyFace::EnemyRight;
		}
		else
		{
			m_face = ELEnemyFace::EnemyLeft;
		}
	}
	/////////

	////////ç∂âEÇÃà⁄ìÆ

	
	const int xspeed = std::min(std::abs(m_pos.x - ((m_state == ELEnemyState::Attacking) ? m_tarPos.x : playerpos.x))
		, (m_state == ELEnemyState::Attacking) ? kAttackSpeed : kSpeed);

	if(m_face == ELEnemyFace::EnemyLeft)
	{
		m_pos.x = Max(m_pos.x-xspeed,0);
	}
	else
	{
		m_pos.x = Min(m_pos.x+xspeed,map.getMapSize().x);
	}
	/////////

	/////////è„â∫ÇÃà⁄ìÆ
	if(m_state == ELEnemyState::Attacking)
	{
		const int yspeed = Min(std::abs(m_pos.y-(m_tarPos.y))
			,(m_state == ELEnemyState::Attacking) ? kAttackSpeed : kSpeed);

		if(m_pos.y >= m_tarPos.y)//è„è∏
		{
			m_pos.y = Max(m_pos.y-yspeed,0);
		}
		else if(m_pos.y < m_tarPos.y)//â∫ç~
		{
			m_pos.y = Min(m_pos.y + yspeed,map.getMapSize().y);
		}
	}
	else
	{
		m_pos.y = Max(m_pos.y-kSpeed,m_startPos.y);
	}
	//////////////

	m_frameCount+=1;

	if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELRaven::MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
{
	m_oldState = m_state;

	const int dir = (0<=speed ? 1 : -1);

	for(int i=0;i<std::abs(speed);++i)
	{
		for(int j=0;j<=kRiseOffset;++j)
		{
			const Point foot = m_pos;
			const Point head = m_pos - Point(0,m_hitArea.size.y);
			if(!map.isFloor(foot.x+dir,foot.y-j-1) 
				&& !intersectBody(
				Point(foot.x+dir,foot.y-j-1),
				Point(head.x+dir,head.y-j-1),
				map))
			{
				if(!object.isTouchable(Point(head.x+dir,head.y-j-1),Point(foot.x+dir,foot.y-j-1),camerapos))
				{
					m_pos.moveBy(Point(dir,-j));
					break;
				}
			}
		}	
	}
}

void ELRaven::MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
{
	const int dir = (0<=speed ? 1 : -1);

	const Point head = Point(m_pos.x,m_pos.y - m_hitArea.size.y);

	const Point foot = Point(m_pos.x,m_pos.y);

	
	if(dir > 0 && (map.isFloor(foot.x,foot.y) || object.isTouchable(Point(head.x,head.y),Point(foot.x,foot.y),camerapos)))
	{
		if(map.getFloorType(foot.x,foot.y) == ELFloorType::NormalFloor)//è∞Ç»ÇÁ
		{
			return;
		}
	}

	for(int i=1;i<=std::abs(speed);++i)
	{
		

		if(map.isFloor(head.x,head.y+i*dir) || map.isFloor(foot.x,foot.y+i*dir) 
			|| object.isTouchable(Point(head.x,head.y+i*dir),Point(foot.x,foot.y+i*dir),camerapos))
		{
			if(map.getFloorType(head.x,head.y+i*dir) == ELFloorType::NormalFloor
				|| map.getFloorType(foot.x,foot.y+i*dir) == ELFloorType::NormalFloor)//è∞Ç»ÇÁ
			{
				m_pos.y += i*dir;
				return;
			}
		}
	}

	m_pos.y += speed;
}


bool ELRaven::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

void ELRaven::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int kw = 128;
	const int kh = 128;

	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELRaven_stay")
			(
				kw*m_face,
				kh*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELRaven_stay").height/kh)),
				kw,
				kh
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Attacking:
		{	
			TextureAsset(L"texELRaven_attack")
			(
				kw*m_face,
				0,
				kw,
				kh
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELRaven_damaged")
				(kw*m_face,
				0,
				kw,
				kh).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELRaven_dead")
			(kw*m_face
			, kh*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELRaven_dead").height/kh))
			, kw
			, kh).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	default:
		break;
	}
}

ELEnemySize ELRaven::getEnemySize() const
{
	return m_size;
}

Point ELRaven::getPos() const
{
	return m_pos;
}

bool ELRaven::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELRaven::getDamage() const
{
	return kDamage;
}

Rect ELRaven::getHitRect() const
{
	return m_hitRect;
}

void ELRaven::giveDamage(int damage)
{
	if(damage >0 && m_state != ELEnemyState::Dead && m_state != ELEnemyState::Damaged)
	{
		m_HP = Max(m_HP-damage,0);

		SoundAsset(L"sndELEnemyDamaged").play();
		m_state = ELEnemyState::Damaged;
		m_frameCount = 0;
	}

	if(m_HP == 0)
	{
		if(SoundAsset(L"sndELravenCry").isPlaying)	SoundAsset(L"sndELravenCry").stop();

		SoundAsset(L"sndELravenCry").play();


		m_state = ELEnemyState::Dead;
	}
}

Point ELRaven::getStartPos() const
{
	return m_startPos;
}

bool ELRaven::isErased() const
{
	return m_erased;
}

bool ELRaven::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}