# include "ELSlimeG.h"


ELSlimeG::ELSlimeG
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
		m_state(ELEnemyState::Jumping),
		m_face(ELEnemyFace::EnemyLeft),
		m_frameCount(0),
		m_oldFrameCount(0),
		m_jumpH(0),
		m_speed(0),
		m_oldState(ELEnemyState::Jumping),
		m_erased(false),
		m_invisibled(false)
{
	TextureAsset::Register(L"texELSlimeG_stay",
		L"data/Elis/Enemy/SlimeG/slime_stay.png");
	TextureAsset::Register(L"texELSlimeG_jump",
		L"data/Elis/Enemy/SlimeG/slime_jump.png");
	TextureAsset::Register(L"texELSlimeG_damaged",
		L"data/Elis/Enemy/SlimeG/slime_damaged.png");
	TextureAsset::Register(L"texELSlimeG_dead",
		L"data/Elis/Enemy/SlimeG/slime_dead.png");
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
	const int kDamage = 2;//ê⁄êGéûÇÃÉ_ÉÅÅ[ÉW
	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELSlimeG::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();
	
	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{
		if(m_frameCount == kAnimeTiming*3-1)
		{
			int a = Random(1,10);
			if(a == 2 || a == 1)
				item.makeItem(ELItemType::LifeUpS,m_hitRect.center,true);
			else if(a == 3 || a == 4)
				item.makeItem(ELItemType::MagicUpS,m_hitRect.center,true);

			m_erased = true;
			return;
		}

		m_frameCount+=1;
		return;
	}

	if(m_state == ELEnemyState::Damaged && m_frameCount<30)
	{
		gravity(map, object,camerapos);
	}
	else
	{
		m_invisibled = false;

		if(m_state == ELEnemyState::Staying 
		&& m_frameCount > kInterval)
		{
			attackProcess(playerpos);
			m_oldFrameCount = m_frameCount;
			m_state = ELEnemyState::Attacking;
		}

		if(m_state == ELEnemyState::Attacking)
		{
			MoveXAxis(map, m_speed, object, camerapos);
			jump(map, m_jumpH, object, camerapos);
			m_jumpH = Max(m_jumpH-kDrop,0);
		}
	
		gravity(map, object, camerapos);

		IdentifyState(map, object, camerapos);
	}
	m_frameCount+=1;

	if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2,m_pos.y - m_hitArea.size.y,m_hitArea.size);
}

void ELSlimeG::attackProcess(const Point& playerpos)
{
	m_jumpH = kMaxJunpH;

	if(playerpos.x-m_pos.x>0)
	{
		m_speed = kSpeed;

		m_face = ELEnemyFace::EnemyRight;
		
		return;
	}
	else
	{
		m_speed -= kSpeed;
		
		m_face = ELEnemyFace::EnemyLeft;

		return;
	}
}

void ELSlimeG::gravity(const ELMap& map, ELObjectInfo& object, const Point& camerapos)
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

void ELSlimeG::IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	if(map.isFloor(m_pos.x,m_pos.y) 
		|| object.isTouchable(Point(m_pos.x,m_pos.y-m_hitArea.size.y),Point(m_pos.x,m_pos.y),camerapos))
	{
		m_state = ELEnemyState::Staying;
		m_jumpH = 0;
		m_speed = 0;
		return;
	}

	if(m_state != ELEnemyState::Attacking)
	{
		m_state = ELEnemyState::Jumping;	
		return;
	}
	
	return;
}

void ELSlimeG::MoveXAxis(const ELMap& map, int speed, ELObjectInfo& object, const Point &camerapos)
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
				&& !intersectBody(Point(foot.x+dir,foot.y-j-1),Point(head.x+dir,head.y-j-1),map))
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

void ELSlimeG::jump(const ELMap& map, int jumpH, ELObjectInfo& object, const Point &camerapos)
{
	for(int i=1;i<jumpH;++i)
	{
		if((
			map.isFloor(m_pos.x,m_pos.y-m_hitArea.size.y-i) 
			&& map.getFloorType(m_pos.x,m_pos.y-m_hitArea.size.y-i) == ELFloorType::NormalFloor
			)
			|| object.isTouchable(Point(m_pos.x,m_pos.y-i),Point(m_pos.x,m_pos.y-m_hitRect.size.y-i),camerapos))
		{
			m_jumpH = i-1;
			break;
		}
	}
	m_pos.y -= m_jumpH;
}

bool ELSlimeG::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

void ELSlimeG::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			if(m_oldState == ELEnemyState::Jumping)
			{
				TextureAsset(L"texELSlimeG_jump")
				(128*m_face, 128*1,	128, 128).draw(m_pos-camerapos-modifyDrawPos);
			}
			TextureAsset(L"texELSlimeG_stay")
			(128*m_face, 128*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELSlimeG_stay").height/128)), 128, 128).
			draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	case ELEnemyState::Jumping:
		{
			if(m_oldState == ELEnemyState::Staying)
			{
				TextureAsset(L"texELSlimeG_jump")
				(128*m_face, 128*0,	128, 128).draw(m_pos-camerapos-modifyDrawPos);
			}
			else
			{
				TextureAsset(L"texELSlimeG_jump")
				(128*m_face, 128*2, 128, 128).draw(m_pos-camerapos-modifyDrawPos);
			}
		break;
		}
	case ELEnemyState::Attacking:
		{
			TextureAsset(L"texELSlimeG_jump")
			(128*m_face, 128*2, 128, 128).draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELSlimeG_damaged")
			(128*m_face, 0, 128, 128).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELSlimeG_dead")
			(128*m_face, (m_frameCount/kAnimeTiming)%(height(L"texELSlimeG_dead")/128)*128, 128, 128).draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELSlimeG::getEnemySize() const
{
	return m_size;
}

Rect ELSlimeG::getDrawRect() const
{
	return m_drawRect;
}

Point ELSlimeG::getPos() const
{
	return m_pos;
}

bool ELSlimeG::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_invisibled)
		return false;

	//LOGFMT_ERR();
	return body.intersects(m_hitRect);
}

int ELSlimeG::getDamage() const
{
	return kDamage;
}

Rect ELSlimeG::getHitRect() const
{
	return m_hitRect;
}

void ELSlimeG::giveDamage(int damage)
{
	if(damage >0 && m_state != ELEnemyState::Dead && m_state != ELEnemyState::Damaged)
	{
		m_HP = Max(m_HP-damage,0);

		SoundAsset(L"sndELEnemyDamaged").play();
		m_state = ELEnemyState::Damaged;
		m_invisibled = true;
		m_frameCount = 0;
	}

	if(m_HP==0)
	{
		m_state = ELEnemyState::Dead;
	}
}

Point ELSlimeG::getStartPos() const
{
	return m_startPos;
}

bool ELSlimeG::isErased() const
{
	return m_erased;
}

bool ELSlimeG::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}