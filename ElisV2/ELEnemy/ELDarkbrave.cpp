# include "ELDarkbrave.h"


ELDarkbrave::ELDarkbrave
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
		m_state(ELDarkbraveState::NormalStaying),
		m_attackMode(AttackMode::null),
		m_face(ELEnemyFace::EnemyLeft),
		m_frameCount(0),
		m_oldFrameCount(0),
		m_jumpH(0),
		m_speed(0),
		m_oldState(ELDarkbraveState::Jumping),
		m_erased(false),
		m_damaged(false),
		m_warped(false),
		m_damageFrame(0),
		m_choiceCount(0),
		m_oldWarpto(0)
{
	TextureAsset::Register(L"texELDarkbrave_stay",
		L"data/Elis/Enemy/Darkbrave/darkbrave_stay.png");
	TextureAsset::Register(L"texELDarkbrave_stay2",
		L"data/Elis/Enemy/Darkbrave/darkbrave_stay2.png");
	
	TextureAsset::Register(L"texELDarkbrave_dead",
		L"data/Elis/Enemy/Darkbrave/darkbrave_dead.png");

	TextureAsset::Register(L"texELDarkbrave_ready",
		L"data/Elis/Enemy/Darkbrave/darkbrave_ready.png");
	TextureAsset::Register(L"texELDarkbrave_float",
		L"data/Elis/Enemy/Darkbrave/darkbrave_float.png");

	TextureAsset::Register(L"texELDarkbrave_swordrain_a",
		L"data/Elis/Enemy/Darkbrave/swordrain_a.png");
	TextureAsset::Register(L"texELDarkbrave_swordrain_b",
		L"data/Elis/Enemy/Darkbrave/swordrain_b.png");

	TextureAsset::Register(L"texELDarkbrave_warp",
		L"data/Elis/Enemy/Darkbrave/warp.png");

	TextureAsset::Register(L"texELDarkbrave_dash",
		L"data/Elis/Enemy/Darkbrave/darkbrave_dash.png");

	TextureAsset::Register(L"texELDarkbrave_strike",
		L"data/Elis/Enemy/Darkbrave/darkbrave_strike.png");

	SoundAsset::Register(L"sndELsword",L"data/Elis/SE/Darkbrave/sword.mp3");
	SoundAsset::Register(L"sndELwarp",L"data/Elis/SE/Darkbrave/warp.mp3");
	SoundAsset::Register(L"sndELtransform",L"data/Elis/SE/Darkbrave/transform.mp3");
	
}

namespace
{
	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =20;
	const int kDrop = 1;

	const int kRiseOffset = 3;

	const int kDamage = 5;

	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kReadyTime = kAnimeTiming * 10;

	const int kx = 128;
	const int ky = 128;

	const int kFloatHeight = 250;

	const int kXspeed = 4;
	const int kYspeed = 3;

	const int kDistance = 250;

	const int kInterval = 120;

	const int kSwordpos[4] = {420,780,1240,1600};
	const int kSwordSpace = 100;

	const int kChoiceSkillSize = 4;
	const int kChoiceSkill[kChoiceSkillSize] = {1,1,1,2};
};

void ELDarkbrave::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	const Point head = Point(m_pos.x,m_pos.y - m_hitArea.size.y);
	const Point foot = Point(m_pos.x,m_pos.y);
	object.isTouchable(Point(head.x,head.y),Point(foot.x,foot.y),camerapos);

	if(m_state == ELDarkbraveState::Dead && m_oldState != ELDarkbraveState::Dead)SoundAsset(L"sndELEnemyDefeat").play();
	
	m_oldState = m_state;

	if(m_state == ELDarkbraveState::Dead)
	{
		/*
		//LOGFMT_ERR(m_frameCount);
		if(m_frameCount == kAnimeTiming*13-1)
		{
			m_erased = true;

			return;
		}

		m_frameCount+=1;
		*/
		return;
	}
	
	if(m_state != ELDarkbraveState::Dash && m_state != ELDarkbraveState::Strike)
	{
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
	}

	if(m_attackMode == AttackMode::SwordRain)
	{
		SwordRainProcess(map, camerapos, object, attack);
	}
	else if(m_attackMode == AttackMode::Strike)
	{
		StrikeProcess(map, camerapos, object, attack, playerpos);
	}
	else if(m_attackMode == AttackMode::null)
	{
		if(m_state == ELDarkbraveState::Floating)
		{
			FloatingProcess(map,camerapos,object, playerpos);
		}
		else if(m_state == ELDarkbraveState::BattleStaying || m_state == ELDarkbraveState::NormalStaying)
		{
			gravity(map,object,camerapos);
		}
	}

	if(m_damaged && m_damageFrame>40)
	{
		m_damaged = false;
		m_damageFrame = 0;
	}

	if(m_damaged)	++m_damageFrame;

	m_frameCount+=1;

	if(m_oldState != m_state)
	{
		m_frameCount = 1;
	}

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x/2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

void ELDarkbrave::StrikeProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, ELAttack &attack, const Point &playerpos)
{
	if(!m_warped)
	{
		if(m_state != ELDarkbraveState::Warp)
		{
			m_state = ELDarkbraveState::Warp;
			m_frameCount = 1;
		}

		if(m_frameCount > kAnimeTiming * 4)
		{
			m_state = ELDarkbraveState::Dash;
			m_warped = true;
			return;
		}

		if(m_frameCount == kAnimeTiming * 2)
		{
			int a =Random(1,2);
			switch(a)
			{
				case 1:
					Warp(Point(kSwordpos[0],m_startPos.y - kFloatHeight/2));
				break;
				case 2:
					Warp(Point(kSwordpos[3],m_startPos.y - kFloatHeight/2));
				break;
			}
			return;
		}

		return;
	}

	if(m_state == ELDarkbraveState::Strike && m_frameCount == kAnimeTiming*3-1)
	{
		m_warped = false;
		m_state = ELDarkbraveState::Floating;
		m_attackMode = AttackMode::null;
		m_frameCount = 1;
		return;
	}

	/////////////////
	if(m_face == ELEnemyFace::EnemyLeft)
		MoveXAxis(map,-(kSpeed)*2,camerapos,object);
	else
		MoveXAxis(map,(kSpeed)*2,camerapos,object);
	
	const int dir = (0<=playerpos.y - m_pos.y ? 1 : -1);
	const int xdir = (m_face != ELEnemyFace::EnemyLeft ? 1 : -1);

	MoveYAxis(map,2*dir,camerapos,object);

	const Point head = Point(m_pos.x+xdir,m_pos.y - m_hitArea.size.y);
	const Point foot = Point(m_pos.x+xdir,m_pos.y);
	
	if(m_pos.distanceFrom(playerpos) < 20.0
		|| object.isTouchable(Point(head.x,head.y),Point(foot.x,foot.y),camerapos)
		|| intersectBody(Point(foot.x,foot.y),Point(head.x,head.y),map))
	{
		m_state = ELDarkbraveState::Strike;
	}
}

void ELDarkbrave::SwordRainProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, ELAttack &attack)
{
	if(!m_warped)
	{
		if(m_state != ELDarkbraveState::Warp)
		{
			m_state = ELDarkbraveState::Warp;
			m_frameCount = 1;
		}

		if(m_frameCount > kAnimeTiming * 4)
		{
			m_state = ELDarkbraveState::SwordRain;
			m_warped = true;
			return;
		}

		if(m_frameCount == kAnimeTiming * 2)
		{
			int a;

			for(;;)
			{
				a =Random(1,4);

				if(a!=m_oldWarpto)
					break;
			}

			m_oldWarpto = a;

			switch(a)
			{
				case 1:
					Warp(Point(kSwordpos[a-1],m_startPos.y - kFloatHeight));
				break;
				case 2:
				Warp(Point(kSwordpos[a-1],m_startPos.y - kFloatHeight));
				break;
				case 3:
				Warp(Point(kSwordpos[a-1],m_startPos.y - kFloatHeight));
				break;
				case 4:
				Warp(Point(kSwordpos[a-1],m_startPos.y - kFloatHeight));
				break;
			}
			return;
		}

		return;
	}

	if(m_frameCount == kAnimeTiming)
	{
		SoundAsset(L"sndELsword").play();

		const int a = Random(-1,1);

		const int s = a*kSwordSpace/2;

		switch(m_pos.x)
		{
		case 420:
			{
			const int as[14] = {-1,1,2,3,4,5,6,7,8,9,10,11,12,13};

			for(int i=0;i<14;++i)
			{
				attack.makeShoot(ELShooter::Enemy,ELShootType::SwordRain,Point(m_pos.x+kSwordSpace*as[i],m_pos.y+s-200),m_pos);
			}
			
			break;
			}
		case 780:
			{
				const int as[14] = {-5,-4,-3,-2,-1,1,2,3,4,5,6,7,8,9};

				for(int i=0;i<14;++i)
				{
					attack.makeShoot(ELShooter::Enemy,ELShootType::SwordRain,Point(m_pos.x+kSwordSpace*as[i]+s,m_pos.y-200),m_pos);
				}

			break;
			}
		case 1240:
			{
				const int as[14] = {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,1,2,3,4};

				for(int i=0;i<14;++i)
				{
					attack.makeShoot(ELShooter::Enemy,ELShootType::SwordRain,Point(m_pos.x+kSwordSpace*as[i]+s,m_pos.y-200),m_pos);
				}

			break;
			}
		case 1600:
			{
				const int as[14] = {-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,1};

				for(int i=0;i<14;++i)
				{
					attack.makeShoot(ELShooter::Enemy,ELShootType::SwordRain,Point(m_pos.x+kSwordSpace*as[i]+s,m_pos.y-200),m_pos);
				}

			break;
			}
		}
	}

	if(m_frameCount == kAnimeTiming * 15 -1)
	{
		m_state = ELDarkbraveState::Floating;
		m_attackMode = AttackMode::null;
		m_frameCount = 1;
		m_warped = false;
	}
}

void ELDarkbrave::Warp(const Point &tarpos)
{
	SoundAsset(L"sndELwarp").play();
	m_pos = tarpos;
}

void ELDarkbrave::FloatingProcess(const ELMap& map, const Point &camerapos, ELObjectInfo& object, const Point &playerpos)
{
	if(m_pos.y > m_startPos.y-kFloatHeight)
		MoveYAxis(map,-kYspeed,camerapos,object);

	if(std::abs(m_pos.x - playerpos.x) < kDistance)
	{
	if(m_face == ELEnemyFace::EnemyRight)
		MoveXAxis(map,-kXspeed,camerapos,object);
	else
		MoveXAxis(map,kXspeed,camerapos,object);
	}

	if(kInterval < m_frameCount)
	{
		//int a = Random(1,2);
		int a = kChoiceSkill[m_choiceCount];

		++m_choiceCount;
		if(m_choiceCount == kChoiceSkillSize)	m_choiceCount = 0;

		switch(a)
		{
		case 1://ソードレイン
			m_attackMode = AttackMode::SwordRain;
			break;
		case 2://ストライク
			m_attackMode = AttackMode::Strike;
			break;
		}
	}
}

void ELDarkbrave::gravity(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	for(int i=0; i<kGravity;++i)
	{
		
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

void ELDarkbrave::IdentifyState(const ELMap& map, ELObjectInfo& object)
{
	return;
}

void ELDarkbrave::MoveXAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
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

void ELDarkbrave::MoveYAxis(const ELMap& map, int speed, const Point &camerapos, ELObjectInfo& object)
{
	const int dir = (0<=speed ? 1 : -1);

	const Point head = Point(m_pos.x,m_pos.y - m_hitArea.size.y);

	const Point foot = Point(m_pos.x,m_pos.y);

	
	if(dir > 0 && (map.isFloor(foot.x,foot.y) 
		|| object.isTouchable(Point(head.x,head.y),Point(foot.x,foot.y),camerapos)))
	{
		if(map.getFloorType(foot.x,foot.y) == ELFloorType::NormalFloor)//床なら
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
				|| map.getFloorType(foot.x,foot.y+i*dir) == ELFloorType::NormalFloor)//床なら
			{
				//LOGFMT_ERR("aaaaaaa");
				m_pos.y += i*dir;
				return;
			}
		}
	}

	m_pos.y += speed;
}

bool ELDarkbrave::intersectBody(const Point& foot,const Point& head, const ELMap& map)//通常床と身体の接触判定
{
	for(int i=1;i<std::abs(foot.y-head.y);++i)
	{
		if(map.isFloor(foot.x,foot.y-i) && map.getFloorType(foot.x,foot.y-i)==ELFloorType::NormalFloor)
		{
			return true;
		}
	}

	return false;
}

void ELDarkbrave::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	int a = 255;

	if(m_damaged)
		a -= 100;

	switch(m_state)
	{
	case ELDarkbraveState::NormalStaying:
		{	
			TextureAsset(L"texELDarkbrave_stay")
			(
				kx*m_face,
				ky*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELDarkbrave_stay").height/ky)),
				kx,
				ky
				).draw(m_pos-camerapos-modifyDrawPos);

			break;
		}
	case ELDarkbraveState::BattleStaying:
		{	
			TextureAsset(L"texELDarkbrave_stay2")
			(
				kx*m_face,
				ky*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELDarkbrave_stay2").height/ky)),
				kx,
				ky
				).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));

			break;
		}
	case ELDarkbraveState::Dead:
		{
			TextureAsset(L"texELDarkbrave_dead")
			//(256*m_face, (m_frameCount/kAnimeTiming)%(height(L"texELDarkbrave_dead")/256)*256, 256, 256)
			(256*m_face, 0, 256, 256)
			.drawAt(m_pos-camerapos-modifyDrawPos+Point(64,64));
			break;
		}
	case ELDarkbraveState::Ready:
		{
			TextureAsset(L"texELDarkbrave_ready")
			(0, (m_frameCount/kAnimeTiming)%(height(L"texELDarkbrave_ready")/ky)*ky, kx, ky)
			.draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	case ELDarkbraveState::Floating:
		{	
			TextureAsset(L"texELDarkbrave_float")
			(
				kx*m_face,
				ky*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELDarkbrave_float").height/ky)),
				kx,
				ky
				).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));

			break;
		}
	case ELDarkbraveState::Warp:
		{
			if(m_frameCount>kAnimeTiming && m_frameCount < kAnimeTiming+1)
			{
				TextureAsset(L"texELDarkbrave_warp")
					(kx*m_face,128,kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
				break;
			}

			TextureAsset(L"texELDarkbrave_warp")
					(kx*m_face,0,kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	case ELDarkbraveState::SwordRain:
		{	
			if(m_frameCount > kAnimeTiming*7)
			{
				TextureAsset(L"texELDarkbrave_swordrain_b")
			(
				kx*m_face,
				ky*((m_frameCount-kAnimeTiming*7)/kAnimeTiming%(TextureAsset(L"texELDarkbrave_swordrain_b").height/ky)),
				kx,
				ky
				).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));

				break;
			}

			if(m_frameCount>kAnimeTiming)
			{
					TextureAsset(L"texELDarkbrave_swordrain_a")
				(kx*m_face,128,kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			}
			else
				TextureAsset(L"texELDarkbrave_swordrain_a")
				(kx*m_face,0,kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	case ELDarkbraveState::Dash:
		{
			TextureAsset(L"texELDarkbrave_dash")
				(kx*m_face,ky*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELDarkbrave_dash").height/ky)),kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	case ELDarkbraveState::Strike:
		{
			TextureAsset(L"texELDarkbrave_strike")
			(kx*m_face,ky*(m_frameCount/kAnimeTiming%(TextureAsset(L"texELDarkbrave_strike").height/ky)),kx,ky).draw(m_pos-camerapos-modifyDrawPos,Alpha(a));
			break;
		}
	default:
		break;
	}

	//Circle(m_pos.x-camerapos,10).draw(Palette::Red);
	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);

	//LOGFMT_ERR(m_frameCount);
}

ELEnemySize ELDarkbrave::getEnemySize() const
{
	return m_size;
}

Point ELDarkbrave::getPos() const
{
	return m_pos;
}

bool ELDarkbrave::isHit(const Rect &body) const
{
	if(m_state == ELDarkbraveState::Dead || m_state == ELDarkbraveState::Warp)
		return false;

	return body.intersects(m_hitRect);
}

int ELDarkbrave::getDamage() const
{
	return kDamage;
}

Rect ELDarkbrave::getHitRect() const
{
	return m_hitRect;
}

void ELDarkbrave::giveDamage(int damage)
{
	if(damage >0 && m_state != ELDarkbraveState::Dead && !m_damaged)
	{
		SoundAsset(L"sndELEnemyDamaged").play();
		
		m_damaged = true;

		m_HP = Max(m_HP-damage,0);
	}

	if(m_HP == 0)
	{
		m_state = ELDarkbraveState::Dead;
		m_frameCount = 1;
	}
}

Point ELDarkbrave::getStartPos() const
{
	return m_startPos;
}

bool ELDarkbrave::isErased() const
{
	return m_erased;
}

bool ELDarkbrave::isDamaged() const
{
	return m_state == ELDarkbraveState::Damaged || m_state == ELDarkbraveState::Dead;
}

bool ELDarkbrave::ready()
{
	if(m_state != ELDarkbraveState::Ready)
	{
		SoundAsset(L"sndELtransform").play();
		m_frameCount = 0;
		m_state = ELDarkbraveState::Ready;
	}

	if(m_frameCount == kReadyTime-1)
	{
		m_state = ELDarkbraveState::Floating;
		return false;
	}
	return true;
}