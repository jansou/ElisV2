# include "ELRedcap.h"


ELRedcap::ELRedcap
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
		m_jumpH(0),
		m_speed(0),
		m_oldState(ELEnemyState::Jumping),
		m_shooted(true),
		m_erased(false)
{
	TextureAsset::Register(L"texELRedcap_stay",
		L"data/Elis/Enemy/Redcap/redcap_stay.png");
	TextureAsset::Register(L"texELRedcap_damaged",
		L"data/Elis/Enemy/Redcap/redcap_damaged.png");
	TextureAsset::Register(L"texELRedcap_dead",
		L"data/Elis/Enemy/Redcap/redcap_dead.png");
	TextureAsset::Register(L"texELRedcap_attack",
		L"data/Elis/Enemy/Redcap/redcap_attack.png");
	TextureAsset::Register(L"texELRedcap_attackArm",
		L"data/Elis/Enemy/Redcap/redcap_attackArm.png");
	SoundAsset::Register(L"sndELredcap_shoot",L"data/Elis/SE/Enemy/redcap_shoot.mp3");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =20;
	const int kInterval = 90;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 2;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELRedcap::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{

	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();
	m_oldState = m_state;

	m_playerpos = Vec2(playerpos.x,playerpos.y-37);

	if(m_state == ELEnemyState::Dead)
	{
		if(m_frameCount == kAnimeTiming*5-1)
		{
			int a = Random(1,10);
			
			if(a >= 1 && a <= 3)
				item.makeItem(ELItemType::LifeUpS,m_hitRect.center,true);
			else if(a >= 4 && a <= 6)
				item.makeItem(ELItemType::MagicUpS,m_hitRect.center,true);

			m_erased = true;

			return;
		}

		m_frameCount+=1;

		return;
	}


	if(playerpos.x-m_pos.x>0)
	{
		m_speed = kSpeed;

		m_face = ELEnemyFace::EnemyRight;
	}
	else
	{
		m_speed -= kSpeed;
		
		m_face = ELEnemyFace::EnemyLeft;
	}

	if(m_state == ELEnemyState::Damaged && m_frameCount<30)
	{
		gravity(map, object, camerapos);
	}
	else
	{
		if(m_state == ELEnemyState::Staying 
		&& m_frameCount > kInterval)
		{
			m_shooted = false;
			m_frameCount = 0;
			m_state = ELEnemyState::Attacking;
		}

		if(m_state == ELEnemyState::Attacking && !m_shooted)
		{
			m_shooted = true;
		}

		if(m_state == ELEnemyState::Attacking && m_frameCount == 40)	
		{
			if(SoundAsset(L"sndELredcap_shoot").isPlaying)
				SoundAsset(L"sndELredcap_shoot").stop();

			SoundAsset(L"sndELredcap_shoot").play();

			attack.makeShoot(ELShooter::Enemy,ELShootType::Arrow,Point(m_pos.x, m_pos.y-28),Point(playerpos.x,playerpos.y-37));
		}

		gravity(map, object, camerapos);

		IdentifyState(map, object, camerapos);
	}

	

	if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}

	m_frameCount+=1;

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELRedcap::gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	for(int i=0; i<kGravity;++i)
	{
		//LOGFMT_ERR(map.isFloor(enemyPos.x,enemyPos.y+i));
		if(object.isTouchable(Point(m_pos.x,m_pos.y+i-m_hitArea.size.y),Point(m_pos.x,m_pos.y+i),camerapos)
			|| map.isFloor(m_pos.x,m_pos.y+i)) 
		{
			m_pos.y += i;
			return;
		}
	}

	m_pos.y +=kGravity;
	return;
}

void ELRedcap::IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	if(m_state == ELEnemyState::Attacking && m_frameCount < kAnimeTiming*6)
	{
		return;
	}
	m_state = ELEnemyState::Staying;
	return;
}

void ELRedcap::MoveXAxis(const ELMap& map, int speed, const Point &camerapos)
{
	m_oldState = m_state;

	const int dir = (0<=speed ? 1 : -1);

	for(int i=0;i<std::abs(speed);++i)
	{
		for(int j=0;j<=kRiseOffset;++j)
		{
			const Point foot = m_pos;
			const Point head = m_pos - Point(0,/*kHeight*/m_hitArea.size.y);
			if(!map.isFloor(foot.x+dir,foot.y-j-1) 
				&& !intersectBody(
				Point(foot.x+dir,foot.y-j-1),
				Point(head.x+dir,head.y-j-1),
				map))
			{
				m_pos.moveBy(Point(dir,-j));
				break;
			}
		}	
	}
}

bool ELRedcap::intersectBody(const Point& foot,const Point& head, const ELMap& map)//’Êí°‚Æg‘Ì‚ÌÚG”»’è
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

void ELRedcap::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELRedcap_stay")
			(
				64*m_face,
				64*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELRedcap_stay").height/64)),
				64,
				64
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Attacking:
		{
			Vec2 v2(m_pos.x, m_pos.y-28);
			Vec2 direc = Vec2(Vec2((m_playerpos.x),m_playerpos.y) - v2).normalize();

			int face = -1;
			if(m_face == ELEnemyFace::EnemyRight)	face = 1;


			TextureAsset(L"texELRedcap_attackArm")
			(
				64*m_face,
				64*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELRedcap_attackArm").height/64)),
				64,
				64
				).rotate(Atan2(face*direc.y,face*direc.x)).draw(m_pos-camerapos-modifyDrawPos);

			TextureAsset(L"texELRedcap_attack")
			(
				64*m_face,
				64*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELRedcap_attack").height/64)),
				64,
				64
			).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELRedcap_damaged")
				(64*m_face,
				0,
				64,
				64).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELRedcap_dead")
			(64*m_face, (m_frameCount/kAnimeTiming)%(height(L"texELRedcap_dead")/64)*64, 64, 64).draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELRedcap::getEnemySize() const
{
	return m_size;
}

Point ELRedcap::getPos() const
{
	return m_pos;
}

bool ELRedcap::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead)
		return false;

	return body.intersects(m_hitRect);
}

int ELRedcap::getDamage() const
{
	return kDamage;
}

Rect ELRedcap::getHitRect() const
{
	return m_hitRect;
}

void ELRedcap::giveDamage(int damage)
{
	m_HP = Max(m_HP-damage,0);

	if(damage >0 && m_state != ELEnemyState::Dead)
	{
		SoundAsset(L"sndELEnemyDamaged").play();
		m_state = ELEnemyState::Damaged;
		m_frameCount = 0;
	}

	if(m_HP == 0)
	{
		m_state = ELEnemyState::Dead;
	}
}

Point ELRedcap::getStartPos() const
{
	return m_startPos;
}

bool ELRedcap::isErased() const
{
	return m_erased;
}

bool ELRedcap::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}