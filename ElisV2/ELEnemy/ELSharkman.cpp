# include "ELSharkman.h"


ELSharkman::ELSharkman
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
		m_erased(false)
{
	TextureAsset::Register(L"texELSharkman_stay",
		L"data/Elis/Enemy/Sharkman/sharkman_stay.png");
	TextureAsset::Register(L"texELSharkman_damaged",
		L"data/Elis/Enemy/Sharkman/sharkman_damaged.png");
	TextureAsset::Register(L"texELSharkman_dead",
		L"data/Elis/Enemy/Sharkman/sharkman_dead.png");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =20;
	const int kInterval = 70;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 5;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELSharkman::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{

		//SoundAsset(L"sndEnemyDefeat").play();

		if(m_frameCount == kAnimeTiming*6-1)
		{
			int a = Random(1,10);
			if(a >= 1 && a <= 5)
				item.makeItem(ELItemType::LifeUpM,m_hitRect.center,true);
			if(a >= 6 && a <= 10)
				item.makeItem(ELItemType::MagicUpM,m_hitRect.center,true);

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

	gravity(map,object,camerapos);

	if(m_state != ELEnemyState::Damaged || m_frameCount>30)
	{
		IdentifyState(map,object);
	}

	m_frameCount+=1;

	if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELSharkman::gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	for(int i=0; i<kGravity;++i)
	{
		//LOGFMT_ERR(map.isFloor(enemyPos.x,enemyPos.y+i));
		if(object.isTouchable(Point(m_pos.x,m_pos.y+i-m_hitArea.size.y),Point(m_pos.x,m_pos.y+i),camerapos)
			|| map.isFloor(m_pos.x,m_pos.y+i) )
		{
			m_pos.y += i;
			return;
		}
	}

	m_pos.y +=kGravity;
	return;
}

void ELSharkman::IdentifyState(const ELMap& map, ELObjectInfo& object)
{
	m_state = ELEnemyState::Staying;
	return;
}

void ELSharkman::MoveXAxis(const ELMap& map, int speed, const Point &camerapos)
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

bool ELSharkman::intersectBody(const Point& foot,const Point& head, const ELMap& map)//’Êí°‚Æg‘Ì‚ÌÚG”»’è
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

void ELSharkman::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELSharkman_stay")
			(
				256*m_face,
				256*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELSharkman_stay").height/256)),
				256,
				256
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELSharkman_damaged")
				(256*m_face,
				0,
				256,
				256).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELSharkman_dead")
			(256*m_face, (m_frameCount/kAnimeTiming)%(height(L"texELSharkman_dead")/256)*256, 256, 256).draw(m_pos-camerapos-modifyDrawPos);
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELSharkman::getEnemySize() const
{
	return m_size;
}

Point ELSharkman::getPos() const
{
	return m_pos;
}

bool ELSharkman::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELSharkman::getDamage() const
{
	return kDamage;
}

Rect ELSharkman::getHitRect() const
{
	return m_hitRect;
}

void ELSharkman::giveDamage(int damage)
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
		m_state = ELEnemyState::Dead;
	}
}

Point ELSharkman::getStartPos() const
{
	return m_startPos;
}

bool ELSharkman::isErased() const
{
	return m_erased;
}

bool ELSharkman::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}