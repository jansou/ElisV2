# include "ELBoneman.h"


ELBoneman::ELBoneman
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
		m_oldState(ELEnemyState::Staying),
		m_erased(false),
		m_damaged(false)
{
	TextureAsset::Register(L"texELBoneman_stay",
		L"data/Elis/Enemy/Boneman/boneman_stay.png");
	TextureAsset::Register(L"texELBoneman_move",
		L"data/Elis/Enemy/Boneman/boneman_move.png");
	TextureAsset::Register(L"texELBoneman_dead",
		L"data/Elis/Enemy/Boneman/boneman_dead.png");
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 5;
	const int kMaxJunpH =20;
	const int kInterval = 70;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 4;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}
};

void ELBoneman::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{

		//SoundAsset(L"sndEnemyDefeat").play();

		if(m_frameCount == kAnimeTiming*4-1)
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

	if(m_damaged && m_frameCount -30 >m_oldFrameCount)
		m_damaged = false;

	if(playerpos.x-m_pos.x>0)
	{
		m_speed = Min(kSpeed,std::abs(m_pos.x-playerpos.x));

		m_face = ELEnemyFace::EnemyRight;
	}
	else
	{
		m_speed = -Min(kSpeed,std::abs(m_pos.x-playerpos.x));
		
		m_face = ELEnemyFace::EnemyLeft;
	}

	const int offsetY = 10;

	if(playerpos.y < m_pos.y+offsetY && playerpos.y > m_pos.y-offsetY)
	{
		MoveXAxis(map,m_speed,camerapos,object);

		m_state = ELEnemyState::Moving;
	}
	else
	{
		IdentifyState(map,object);
	}

	gravity(map,object,camerapos);

	m_frameCount+=1;

	/*if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}*/

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELBoneman::gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
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

void ELBoneman::IdentifyState(const ELMap& map, ELObjectInfo& object)
{
	m_state = ELEnemyState::Staying;
	return;
}

void ELBoneman::MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
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

void ELBoneman::MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
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
				//LOGFMT_ERR("aaaaaaa");
				m_pos.y += i*dir;
				return;
			}
		}
	}

	m_pos.y += speed;
}


bool ELBoneman::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

void ELBoneman::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int w = 128;
	const int h = 128;

	int a = 255;
	if(m_damaged || m_state == ELEnemyState::Dead)	a-=100;

	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELBoneman_stay")
			(
				w*m_face,
				h*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBoneman_stay").height/h)),
				w,
				h
				).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));

			break;
		}
	case ELEnemyState::Moving:
		{
			TextureAsset(L"texELBoneman_move")
				(
				w*m_face,
				h*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBoneman_move").height/h)),
				w,
				h
				).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELBoneman_dead")
				(w*m_face, h*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBoneman_dead").height/h)), w, h)
				.draw(m_pos-camerapos-modifyDrawPos,Alpha(a-50));
			break;
		}
	default:
		break;
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELBoneman::getEnemySize() const
{
	return m_size;
}

Point ELBoneman::getPos() const
{
	return m_pos;
}

bool ELBoneman::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELBoneman::getDamage() const
{
	return kDamage;
}

Rect ELBoneman::getHitRect() const
{
	return m_hitRect;
}

void ELBoneman::giveDamage(int damage)
{
	if(damage >0 && !m_damaged)
	{
		m_HP = Max(m_HP-damage,0);

		SoundAsset(L"sndELEnemyDamaged").play();
		m_damaged = true;
		m_oldFrameCount = m_frameCount;
	}

	if(m_HP == 0)
	{
		m_state = ELEnemyState::Dead;
		m_frameCount = 0;
	}
}

Point ELBoneman::getStartPos() const
{
	return m_startPos;
}

bool ELBoneman::isErased() const
{
	return m_erased;
}

bool ELBoneman::isDamaged() const
{
	return m_state == ELEnemyState::Dead;
}