# include "ELBluecap.h"


ELBluecap::ELBluecap
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
	TextureAsset::Register(L"texELBluecap_stay",
		L"data/Elis/Enemy/Bluecap/Bluecap_stay.png");
	TextureAsset::Register(L"texELBluecap_damaged",
		L"data/Elis/Enemy/Bluecap/Bluecap_damaged.png");
	TextureAsset::Register(L"texELBluecap_dead",
		L"data/Elis/Enemy/Bluecap/Bluecap_dead.png");
	TextureAsset::Register(L"texELBluecap_attack",
		L"data/Elis/Enemy/Bluecap/Bluecap_attack.png");
	TextureAsset::Register(L"texELBluecap_attackArm",
		L"data/Elis/Enemy/Bluecap/Bluecap_attackArm.png");
	SoundAsset::Register(L"sndELBluecap_shoot",L"data/Elis/SE/Enemy/bluecap_shoot.mp3");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =20;
	const int kInterval = 60;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 5;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELBluecap::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
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

		if(m_state == ELEnemyState::Attacking && m_frameCount == kAnimeTiming*3)	
		{
			if(SoundAsset(L"sndELBluecap_shoot").isPlaying)
				SoundAsset(L"sndELBluecap_shoot").stop();

			SoundAsset(L"sndELBluecap_shoot").play();

			int a = Random(3,5);
			//int b = Random(4,5);
			//attack.makeShoot(ELShooter::Enemy,ELShootType::CurveAxe,Point(m_pos.x, m_pos.y-28),Point(((m_face==ELEnemyFace::EnemyRight) ? 1 : -1)*6,3));
			//attack.makeShoot(ELShooter::Enemy,ELShootType::CurveAxe,Point(m_pos.x, m_pos.y-28),Point(((m_face==ELEnemyFace::EnemyRight) ? 1 : -1)*5,4));
			//attack.makeShoot(ELShooter::Enemy,ELShootType::CurveAxe,Point(m_pos.x, m_pos.y-28),Point(((m_face==ELEnemyFace::EnemyRight) ? 1 : -1)*4,5));

			attack.makeShoot(ELShooter::Enemy,ELShootType::CurveAxe,Point(m_pos.x, m_pos.y-28),Point(((m_face==ELEnemyFace::EnemyRight) ? 1 : -1)*a,4));
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

void ELBluecap::gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
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

void ELBluecap::IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	if(m_state == ELEnemyState::Attacking && m_frameCount < kAnimeTiming*6)
	{
		return;
	}
	m_state = ELEnemyState::Staying;
	return;
}

bool ELBluecap::intersectBody(const Point& foot,const Point& head, const ELMap& map)//’Êí°‚Æg‘Ì‚ÌÚG”»’è
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

void ELBluecap::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELBluecap_stay")
			(
				64*m_face,
				64*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBluecap_stay").height/64)),
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
			if(m_face == ELEnemyFace::EnemyLeft)	face = 1;
		
			TextureAsset(L"texELBluecap_attack")
			(
				64*m_face,
				64*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBluecap_attack").height/64)),
				64,
				64
			).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELBluecap_damaged")
				(64*m_face,
				0,
				64,
				64).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELBluecap_dead")
			(64*m_face, (m_frameCount/kAnimeTiming)%(height(L"texELBluecap_dead")/64)*64, 64, 64).draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELBluecap::getEnemySize() const
{
	return m_size;
}

Point ELBluecap::getPos() const
{
	return m_pos;
}

bool ELBluecap::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead)
		return false;

	return body.intersects(m_hitRect);
}

int ELBluecap::getDamage() const
{
	return kDamage;
}

Rect ELBluecap::getHitRect() const
{
	return m_hitRect;
}

void ELBluecap::giveDamage(int damage)
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

Point ELBluecap::getStartPos() const
{
	return m_startPos;
}

bool ELBluecap::isErased() const
{
	return m_erased;
}

bool ELBluecap::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}