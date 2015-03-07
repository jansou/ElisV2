# include "ELBigslime.h"


ELBigslime::ELBigslime
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
		m_state(ELEnemyState::Standby),
		m_face(ELEnemyFace::EnemyLeft),
		m_frameCount(1),
		m_oldFrameCount(0),
		m_jumpH(0),
		m_speed(0),
		m_oldState(ELEnemyState::Standby),
		m_erased(false),
		m_invisibled(false),
		m_cameraEffect(ELCameraEffect::null),
		m_damageCount(0)
{
	TextureAsset::Register(L"texELBigslime_stay",
		L"data/Elis/Enemy/Bigslime/bigslime_stay.png");
	TextureAsset::Register(L"texELBigslime_core",
		L"data/Elis/Enemy/Bigslime/core.png");
	TextureAsset::Register(L"texELBigslime_attack",
		L"data/Elis/Enemy/Bigslime/bigslime_attack.png");
	TextureAsset::Register(L"texELBigslime_standby",
		L"data/Elis/Enemy/Bigslime/bigslime_standby.png");
	TextureAsset::Register(L"texELBigslime_ready",
		L"data/Elis/Enemy/Bigslime/bigslime_ready.png");
	/*
	TextureAsset::Register(L"texELBigslime_dead",
		L"data/Elis/Enemy/Slime/slime_dead.png");*/
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =23;
	const int kInterval = 80;
	const int kDrop = 1;
	const int kRiseOffset = 3;
	const int kDamage = 3;//ê⁄êGéûÇÃÉ_ÉÅÅ[ÉW
	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kShotH = 68+100;

	const int kSlimeW = 512;
	const int kSlimeH = 512;

	const int kCoreW = 92;
	const int kCoreH = 92;
	const int kCoreR = 33;

	const int kAttackTime = kAnimeTiming*8;
	const int kShotTime = kAnimeTiming*3;
	const int kDamageTime = kAnimeTiming*4;

	const int kReadyTime = 30;
};

void ELBigslime::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	m_coreRect = Rect(m_pos-Point(0,kSlimeH/2-128)-Point(kCoreR,kCoreR),kCoreR*2,kCoreR*2);

	if(m_state != ELEnemyState::Ready && m_state != ELEnemyState::Standby)
	{
		if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();
	
		m_oldState = m_state;

		if(m_state == ELEnemyState::Dead)
		{
			m_cameraEffect = ELCameraEffect::null;
			m_frameCount+=1;
			return;
		}

		if(m_state == ELEnemyState::Damaged && m_frameCount<30)
		{
			gravity(map, object,camerapos);
		}
		else
		{
			if(m_state == ELEnemyState::Staying 
			&& m_frameCount > kInterval)
			{
				//attackProcess(playerpos);
				m_oldFrameCount = m_frameCount;
				m_frameCount = 0;
				m_state = ELEnemyState::Attacking;
			}

			if(m_state == ELEnemyState::Attacking)
			{
				m_cameraEffect = ELCameraEffect::LittleShake;
				//jump(map, m_jumpH, object, camerapos);
				//m_jumpH = Max(m_jumpH-kDrop,0);

				if(m_frameCount % (kShotTime) == 0)
				{
					attack.makeShoot(ELShooter::Enemy, ELShootType::Rockfall, Point(Random(1,12)*59+30, kShotH), playerpos);
				}
			}
	
			gravity(map, object, camerapos);

			IdentifyState(map, object, camerapos);
		}
		m_frameCount+=1;

		if(m_oldState != m_state)
		{
			m_frameCount = 0;
		}

		if(m_invisibled)
		{
			++m_damageCount;

			if(m_damageCount == kDamageTime)
			{
				m_invisibled = false;
				m_damageCount = 0;
			}
		}

		m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2,m_pos.y - m_hitArea.size.y,m_hitArea.size);
	}
}

void ELBigslime::attackProcess(const Point& playerpos)
{
	
}

void ELBigslime::gravity(const ELMap& map, ELObjectInfo& object, const Point& camerapos)
{
	for(int i=0; i<kGravity;++i)
	{
		//LOGFMT_ERR(map.isFloor(enemyPos.x,enemyPos.y+i));
		if(object.isTouchable(Point(m_pos.x,m_pos.y-i-m_hitArea.size.y),Point(m_pos.x,m_pos.y-i),camerapos)
			||map.isFloor(m_pos.x,m_pos.y+i) )
		{
			m_pos.y +=i;
			return;
		}
	}

	m_pos.y +=kGravity;
	return;
}

void ELBigslime::IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	if(m_state == ELEnemyState::Attacking && kAttackTime > m_frameCount)
	{
		return;
	}

	if(map.isFloor(m_pos.x,m_pos.y) 
		|| object.isTouchable(Point(m_pos.x,m_pos.y-m_hitArea.size.y),Point(m_pos.x,m_pos.y),camerapos))
	{
		m_state = ELEnemyState::Staying;
		m_jumpH = 0;
		m_speed = 0;
		m_cameraEffect = ELCameraEffect::null;
		return;
	}
	
	return;
}

bool ELBigslime::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

void ELBigslime::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int a = 255 -100*m_invisibled;

	if(m_state != ELEnemyState::Ready && m_state != ELEnemyState::Standby)
	{
		TextureAsset(L"texELBigslime_core")
			(0, kCoreH*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBigslime_core").height/kCoreH)), kCoreW, kCoreH).
			drawAt(m_coreRect.center-camerapos,Alpha(a));

		Circle(m_coreRect.center-camerapos,kCoreR+10).draw(Color(255,255,255,m_invisibled * (m_damageCount%4==0) * 255));
	}

	switch(m_state)
	{
	case ELEnemyState::Standby:
		{	
			const int w = 128;
			const int h = 128;

			TextureAsset(L"texELBigslime_standby")
				(0,h*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBigslime_standby").height/h))
				, w, h).
				drawAt(m_pos-camerapos-Point(0,h/2));
			break;
		}
	case ELEnemyState::Ready:
		{	
			const int w = 128;
			const int h = 128;

			const double w2 = 1.0 + m_frameCount/10.0;
			const double h2 = 1.0 + m_frameCount/10.0;


			TextureAsset(L"texELBigslime_ready")
				(0,0
				, w, h).scale(w2, h2).
				drawAt(m_pos-camerapos-Point(0,int(h+128*(m_frameCount/10.0))/2));
			break;
		}
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELBigslime_stay")
				(0,kSlimeH*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBigslime_stay").height/kSlimeH))
				, kSlimeW, kSlimeH).
				drawAt(m_pos-camerapos-Point(0,kSlimeH/2));
			break;
		}
	case ELEnemyState::Attacking:
		{
			TextureAsset(L"texELBigslime_attack")
			(0,kSlimeH*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBigslime_attack").height/kSlimeH))
			, kSlimeW, kSlimeH)
			.drawAt(m_pos-camerapos-Point(0,kSlimeH/2));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELBigslime_stay")
				(0, 0, kSlimeW, kSlimeH).drawAt(m_pos-camerapos-Point(0,kSlimeH/2));
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELBigslime::getEnemySize() const
{
	return m_size;
}

Rect ELBigslime::getDrawRect() const
{
	return m_drawRect;
}

Point ELBigslime::getPos() const
{
	return m_pos;
}

bool ELBigslime::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_invisibled)
		return false;

	//LOGFMT_ERR();
	return body.intersects(m_hitRect);
}

int ELBigslime::getDamage() const
{
	return kDamage;
}

Rect ELBigslime::getHitRect() const//íeÇ…ÇΩÇ¢ÇµÇƒÇÃìñÇΩÇËîªíËÇ…égÇ§
{
	return m_coreRect;
}

void ELBigslime::giveDamage(int damage)
{
	if(damage >0 && m_state != ELEnemyState::Dead && !m_invisibled)
	{
		m_HP = Max(m_HP-damage,0);

		SoundAsset(L"sndELEnemyDamaged").play();
		//m_state = ELEnemyState::Damaged;
		m_invisibled = true;
		m_damageCount = 0;
	}

	if(m_HP==0)
	{
		m_state = ELEnemyState::Dead;
	}
}

Point ELBigslime::getStartPos() const
{
	return m_startPos;
}

bool ELBigslime::isErased() const
{
	return m_erased;
}

bool ELBigslime::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}

///////////////////////////////
bool ELBigslime::ready()
{
	if(m_state != ELEnemyState::Ready)
	{
		//SoundAsset(L"sndELtransform").play();
		m_frameCount = 0;
		m_state = ELEnemyState::Ready;
	}

	if(m_frameCount == kReadyTime-1)
	{
		m_state = ELEnemyState::Staying;
		return false;
	}
	return true;
}