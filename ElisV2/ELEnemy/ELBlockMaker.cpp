# include "ELBlockMaker.h"


ELBlockMaker::ELBlockMaker
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
		m_erased(false)
{
	
	TextureAsset::Register(L"texELBlockMaker_stay",
		L"data/Elis/Enemy/BlockMaker/blockmaker_stay.png");
	TextureAsset::Register(L"texELBlockMaker_damaged",
		L"data/Elis/Enemy/BlockMaker/blockmaker_damaged.png");
	TextureAsset::Register(L"texELBlockMaker_attack",
		L"data/Elis/Enemy/BlockMaker/blockmaker_attack.png");
	/*
	TextureAsset::Register(L"texELBlockMaker_dead",
		L"data/Elis/Enemy/BlockMaker/BlockMaker_dead.png");
		*/
	
	SoundAsset::Register(L"sndELBlockMaker_fire",
		L"data/Elis/SE/Enemy/airsnake_fire.mp3");
		
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 4;
	const int kMaxJunpH =20;
	const int kInterval = 70;
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

void ELBlockMaker::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	if(m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if(m_state == ELEnemyState::Dead)
	{

		//SoundAsset(L"sndEnemyDefeat").play();

		if(m_frameCount == kAnimeTiming*6-1)
		{
			int a = Random(1,10);
			if(a >= 1 && a <= 2)
				item.makeItem(ELItemType::LifeUpM,m_hitRect.center,true);
			if(a >= 6 && a <= 8)
				item.makeItem(ELItemType::MagicUpM,m_hitRect.center,true);

			object.addObject(ELObjectType::block, m_pos);

			m_erased = true;

			return;
		}

		m_frameCount+=1;

		return;
	}


	if(playerpos.x-m_pos.x>0)
	{
		m_face = ELEnemyFace::EnemyRight;
	}
	else
	{
		m_face = ELEnemyFace::EnemyLeft;
	}

	if(m_state == ELEnemyState::Attacking)
	{
		if(m_frameCount > kAnimeTiming*6)
		{
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
		else if(m_frameCount == kAnimeTiming*3)
		{
			Point head = Point(m_pos.x-64,m_pos.y-128);

			head.y += kHead.y;
			head.x += kHead.x;
			if(m_face == ELEnemyFace::EnemyRight)
			head.x += 128-kHead.x*2;

			attack.makeShoot(ELShooter::Enemy,ELShootType::Firebreath,head,playerpos);
			
			if(SoundAsset(L"sndELBlockMaker_fire").isPlaying)	SoundAsset(L"sndELBlockMaker_fire").stop();
			
			SoundAsset(L"sndELBlockMaker_fire").play();
		}
	}
	else if(m_state == ELEnemyState::Staying && m_frameCount>30)
	{
		m_state = ELEnemyState::Attacking;
		m_frameCount = 0;
	}
	else if(m_state != ELEnemyState::Damaged || m_frameCount>30)
	{
		IdentifyState(map,object);
	}

	if(std::abs(m_pos.x-playerpos.x)<kDistance)
	{
		if(m_face == ELEnemyFace::EnemyLeft)
		{
			m_pos.x = Min(m_pos.x+kSpeed,map.getMapSize().x-64);
			//MoveXAxis(map,(kSpeed),camerapos,object);
		}
		else
		{
			m_pos.x = Max(m_pos.x-kSpeed,64);
			//MoveXAxis(map,-(kSpeed),camerapos,object);
		}
	}

	if(m_pos.y != playerpos.y)
	{
		const int ydist = 50;//Ç∏ÇÍï™

		int yspeed = std::min(std::abs(m_pos.y-(playerpos.y+ydist)),kSpeed);

		if(m_pos.y > playerpos.y+ydist)//â∫ç~
		{
			m_pos.y = Max(m_pos.y-yspeed,ydist);
			//MoveYAxis(map,-(yspeed),camerapos,object);
		}
		else if(m_pos.y < playerpos.y+ydist)//è„è∏
		{
			m_pos.y = Min(m_pos.y + yspeed,map.getMapSize().y - ydist);
			//MoveYAxis(map,(yspeed),camerapos,object);
		}
	}

	m_frameCount+=1;

	if(m_oldState != m_state)
	{
		m_frameCount = 0;
	}

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELBlockMaker::IdentifyState(const ELMap& map, ELObjectInfo& object)
{
	m_state = ELEnemyState::Staying;
	return;
}

void ELBlockMaker::MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
{
	m_oldState = m_state;

	const int dir = (0<=speed ? 1 : -1);

	for (int i = 0; i<std::abs(speed); ++i)
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

void ELBlockMaker::MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
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

	for (int i = 1; i <= std::abs(speed); ++i)
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


bool ELBlockMaker::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

void ELBlockMaker::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int kw = 128;
	const int kh = 128;

	switch(m_state)
	{
	case ELEnemyState::Staying:
		{	
			TextureAsset(L"texELBlockMaker_stay")
			(
				kw*m_face,
				kh*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBlockMaker_stay").height/kh)),
				kw,
				kh
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Attacking:
		{	
			TextureAsset(L"texELBlockMaker_attack")
			(
				kw*m_face,
				kh*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELBlockMaker_attack").height/kh)),
				kw,
				kh
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELEnemyState::Damaged:
		{
			TextureAsset(L"texELBlockMaker_damaged")
				(kw*m_face,
				0,
				kw,
				kh).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	case ELEnemyState::Dead:
		{
			TextureAsset(L"texELBlockMaker_damaged")
			(kw*m_face, 0, kw, kh).draw(m_pos-camerapos-modifyDrawPos,Alpha(155));
			break;
		}
	default:
		break;
	}

	//Point head = Point(m_pos.x-64,m_pos.y-128);
	/*
			if(m_face == ELEnemyFace::EnemyRight)
				head += Point(128-kHead.x,kHead.y);
			else if(m_face == ELEnemyFace::EnemyLeft)
				head += kHead;
		*/
	/*
	head.y += kHead.y;
	head.x += kHead.x;
	if(m_face == ELEnemyFace::EnemyRight)
		head.x += 128-kHead.x*2;
			Circle(head-camerapos,10).draw(Palette::Red);
			*/
	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
}

ELEnemySize ELBlockMaker::getEnemySize() const
{
	return m_size;
}

Point ELBlockMaker::getPos() const
{
	return m_pos;
}

bool ELBlockMaker::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELBlockMaker::getDamage() const
{
	return kDamage;
}

Rect ELBlockMaker::getHitRect() const
{
	return m_hitRect;
}

void ELBlockMaker::giveDamage(int damage)
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

Point ELBlockMaker::getStartPos() const
{
	return m_startPos;
}

bool ELBlockMaker::isErased() const
{
	return m_erased;
}

bool ELBlockMaker::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}