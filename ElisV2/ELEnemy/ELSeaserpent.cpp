# include "ELSeaserpent.h"


ELSeaserpent::ELSeaserpent
(
const ELEnemyName& Name,
const ELEnemySize& Size,
const Rect& hitRect,
int HP,
const Point& Pos
) :
m_name(Name),
m_size(Size),
m_hitArea(hitRect),
m_HP(HP),
m_pos(Pos),//Point(640,140)),
m_startPos(Pos),//(Point(640, 140)),
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
m_damageCount(0),
m_attackType(ELSeaserpentAttack::UpperTentacle),
m_hitRect(990,170,80,70),
m_maxHP(HP)
{
	TextureAsset::Register(L"texELSeaserpent_head_stay",L"data/Elis/Enemy/Seaserpent/head_stay.png");
	TextureAsset::Register(L"texELSeaserpent_body_stay",L"data/Elis/Enemy/Seaserpent/body_stay.png");
	TextureAsset::Register(L"texELSeaserpent_tail_stay",L"data/Elis/Enemy/Seaserpent/tail_stay.png");
	
	
	
	/*
	m_tentacleFrontNodes[10];//前
	m_tentacleBackNodes[10];//後

	for(size_t i=0;i<10;++i)
	{
		m_tentacleFrontNodes[i] = kBeginFrontNode;//
		m_tentacleBackNodes[i] = kBeginBackNode;//
	}
	*/
	ELSeaserpentTentacle item;
	m_tentacles.push_back(item);
	m_tentacles.push_back(item);

	m_tentacles[0].setPos(Point(950, 740));
	m_tentacles[1].setPos(Point(1230, 740));
}

namespace
{
	Point kBeginFrontNode(1180,740);
	Point kBeginBackNode(1020,740);

	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;
	const int kMaxJunpH =23;
	const int kInterval = 80;
	const int kDrop = 1;
	const int kRiseOffset = 3;
	const int kDamage = 3;//接触時のダメージ
	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kAttackTime = kAnimeTiming*8;
	const int kShotTime = kAnimeTiming*3;
	const int kDamageTime = kAnimeTiming*4;

	const int kReadyTime = 30;

	//触手の開始地点
	const Point kBeginNodes(1230,552);

	int kNodeSize=10;
};

void ELSeaserpent::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	//tentacle, 950, 720,
	//tentacle, 1230, 720,

	//tentacle1
	//tentacle1update(map, playerpos, object, item, attack, camerapos);

	if (m_tentacles.size()<3 && m_HP < m_maxHP/3*2)
	{
		ELSeaserpentTentacle item;
		m_tentacles.push_back(item);
		m_tentacles[2].setPos(Point(1050,780));
	}
	else if (m_tentacles.size()<4 && m_HP < m_maxHP/3)
	{
		ELSeaserpentTentacle item;
		m_tentacles.push_back(item);
		m_tentacles[3].setPos(Point(830, 760));
	}

	if (m_invisibled)
	{
		++m_damageCount;

		if (m_damageCount > 30)
		{
			m_invisibled = false;
			m_damageCount = 0;
		}
	}

	if (m_frameCount%300==0)
	{
		int ran = Random(m_tentacles.size() - 1);
		m_tentacles[ran].setState(ELEnemyState::Moving);

		if (m_tentacles.size() == 4)
		{
			ran = Random(m_tentacles.size() - 1);
			m_tentacles[ran].setState(ELEnemyState::Moving);
		}
	}

	for (size_t i = 0; i<m_tentacles.size(); ++i)
	{
		m_tentacles[i].update(map, playerpos, object, item, attack, camerapos);
	}

	switch (m_attackType)
	{
	case ELSeaserpentAttack::UpperTentacle:
		//Attack_UpperTentacle(attack, playerpos);
		break;
	case ELSeaserpentAttack::IceShoot:
		break;
	default:
		break;
	}
	
	//m_hitRect = Rect();

	++m_frameCount;
}


void updateFrontTentacle()
{

}

void updateBackTentacle()
{

}

void ELSeaserpent::Attack_UpperTentacle(ELAttack& attack, const Point& playerpos)
{
	static int framecount = 0;
	
	if(framecount == 0)
	{
		attack.makeShoot(
			ELShooter::Enemy,
			ELShootType::UpperTentacle,
			Point(playerpos.x,ELWindowHeight),
			Point(0,0)
			);
	}

	++framecount;

	if(framecount>120)
	{
		framecount = 0;
	}
}

void ELSeaserpent::attackProcess(const Point& playerpos)
{
	
}


void ELSeaserpent::IdentifyState(const ELMap& map, ELObjectInfo& object, const Point &camerapos)
{
	
}

bool ELSeaserpent::intersectBody(const Point& foot,const Point& head, const ELMap& map)//通常床と身体の接触判定
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

void ELSeaserpent::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	//drawBackTentacle(camerapos);

	const int a = 255 -100*m_invisibled;

	drawTail_stay();

	drawBody_stay();

	drawHead_stay();

	/*
	switch(m_state)
	{
	case ELEnemyState::Standby:
		{	
			
		}
	case ELEnemyState::Ready:
		{	
			const int w = 128;
			const int h = 128;

			const double w2 = 1.0 + m_frameCount/10.0;
			const double h2 = 1.0 + m_frameCount/10.0;

			break;
		}
	case ELEnemyState::Staying:
		{	
			
			break;
		}
	case ELEnemyState::Attacking:
		{
			
			break;
		}
	case ELEnemyState::Dead:
		{
			
			break;
		}
	default:
		break;
	}
	*/
	//drawFrontTentacle(camerapos);

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);

	for (size_t i = 0; i < m_tentacles.size(); ++i)
	{
		m_tentacles[i].draw(camerapos, modifyDrawPos);
	}

	//m_hitRect.drawFrame(10, 0, Palette::Black);
}

void ELSeaserpent::drawHead_stay() const
{
	static int framecount = 0;
	static int offset=0;


	if(framecount % kAnimeTiming == 0)
	{
		if(framecount == kAnimeTiming*3
			|| framecount == kAnimeTiming*5)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*11
			|| framecount == kAnimeTiming*13)
		{
			offset += 1; 
		}
	}

	int a = 255 - 100 * m_invisibled;

	TextureAsset(L"texELSeaserpent_head_stay")
				.draw(949,60+offset,Alpha(a));

	++framecount;

	if(framecount == kAnimeTiming*16 + 1)
	{
		framecount = 0;
	}
}

void ELSeaserpent::drawBody_stay() const
{
	static int framecount = 0;
	static int offset=0;


	if(framecount % kAnimeTiming*2 == 0)
	{
		if(framecount == kAnimeTiming*2
			|| framecount == kAnimeTiming*4)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*10
			|| framecount == kAnimeTiming*12)
		{
			offset += 1; 
		}
	}

	TextureAsset(L"texELSeaserpent_body_stay")
		.draw(1057,377+offset);

	++framecount;

	if(framecount == kAnimeTiming*16 + 1)
	{
		framecount = 0;
	}
}


void ELSeaserpent::drawTail_stay() const
{
	static int framecount = 0;
	static int offset=0;


	if(framecount % kAnimeTiming*2 == 0)
	{
		if(framecount == kAnimeTiming*6
			|| framecount == kAnimeTiming*8)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*14
			|| framecount == kAnimeTiming*16)
		{
			offset += 1; 
		}
	}

	
	TextureAsset(L"texELSeaserpent_tail_stay")
			.draw(947,426+offset);

	++framecount;

	if(framecount == kAnimeTiming*16 + 1)
	{
		framecount = 0;
	}
}

////////////////////////////////////////////////////////
ELEnemySize ELSeaserpent::getEnemySize() const
{
	return m_size;
}

Rect ELSeaserpent::getDrawRect() const
{
	return m_drawRect;
}

Point ELSeaserpent::getPos() const
{
	return m_pos;
}

bool ELSeaserpent::isHit(const Rect &body) const
{
	if(m_state == ELEnemyState::Dead || m_invisibled)
		return false;

	//LOGFMT_ERR();
	
	return body.intersects(m_hitRect);
}

int ELSeaserpent::getDamage() const
{
	return kDamage;
}

Rect ELSeaserpent::getHitRect() const//弾にたいしての当たり判定に使う
{
	return m_hitRect;
}

void ELSeaserpent::giveDamage(int damage)//この敵にダメージを与える
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

Point ELSeaserpent::getStartPos() const//初期位置の取得
{
	return m_startPos;
}

bool ELSeaserpent::isErased() const//消滅してもよいか
{
	return m_erased;
}

bool ELSeaserpent::isDamaged() const//ダメージを食らっているか
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}

///////////////////////////////
bool ELSeaserpent::ready()
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
////////////////////////////////////////////////////////////////////////////////

ELSeaserpentTentacle::ELSeaserpentTentacle
(/*
const ELEnemyName& Name,
const ELEnemySize& Size,
const Rect& hitRect,
int HP,
const Point& Pos*/
) :
m_name(ELEnemyName::tantacle),
m_size(ELEnemySize::M),
m_hitArea(Rect(0, 49, 30, 30)),
m_HP(6),
m_pos(Point(950, 720)),
m_startPos(Point(950, 720)),
m_state(ELEnemyState::Ready),
m_face(ELEnemyFace::EnemyLeft),
m_frameCount(0),
m_oldFrameCount(0),
m_jumpH(0),
m_speed(0),
m_oldState(ELEnemyState::Jumping),
m_erased(false),
m_MaxHp(6)
{
	TextureAsset::Register(L"texELTentacle_fin",
		L"data/Elis/Enemy/Tentacle/fin.png");
}

void ELSeaserpentTentacle::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	const int kSpeed = 10;
	const int kHigh = 400;

	if (m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead)SoundAsset(L"sndELEnemyDefeat").play();

	m_oldState = m_state;

	if (m_state == ELEnemyState::Dead)
	{
		m_pos.y = Min(m_pos.y + kSpeed / 2, m_startPos.y);

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
		m_pos.y = Max(m_pos.y - kSpeed, m_startPos.y - kHigh);

		if (m_pos.y == m_startPos.y - kHigh)
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
			attack.makeShoot(ELShooter::Enemy, ELShootType::LongIce, m_pos, Point(m_playerpos.x, m_playerpos.y));
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
	}
	else if (m_state == ELEnemyState::Moving)
	{
		m_pos.y = Min(m_pos.y + kSpeed / 2, m_startPos.y);

		if (m_frameCount == 50)
		{
			attack.makeShoot(
				ELShooter::Enemy,
				ELShootType::UpperTentacle,
				Point(playerpos.x, ELWindowHeight),
				Point(0, 0)
				);
		}

		if (m_frameCount>300)
		{
			m_state = ELEnemyState::Ready;
			m_frameCount = 0;
		}

	}

	m_frameCount += 1;

	m_hitRect = Rect(m_pos.x - m_hitArea.size.x / 2, m_pos.y - m_hitArea.size.y, m_hitArea.size);
}

bool ELSeaserpentTentacle::intersectBody(const Point& foot, const Point& head, const ELMap& map)//通常床と身体の接触判定
{
	for (int i = 1; i<foot.y - head.y; ++i)
	{
		if (map.isFloor(foot.x, foot.y - i) && map.getFloorType(foot.x, foot.y - i) == ELFloorType::NormalFloor)
		{
			return true;
		}
	}

	return false;
}

void ELSeaserpentTentacle::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int r = 10;
	int yoffset = (m_startPos.y - m_pos.y) / 10;

	for (int i = 0; i<10; ++i)
	{
		Point pos(m_startPos.x, m_startPos.y - yoffset*i);

		Circle cir(pos - camerapos, r);
		cir.draw(Color(67, 92, 117));
		cir.drawFrame(0.5, 0.5, Palette::Black);
	}


	switch (m_state)
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
			.rotateAt(Point(0, 64),
			Atan2(direction(m_pos, m_playerpos).y
			, direction(m_pos, m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64));

		break;
	}
	case ELEnemyState::Moving:
	{
		//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));

		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64),
			Atan2(direction(m_pos, m_playerpos).y
			, direction(m_pos, m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64));

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
			.draw(m_pos - camerapos - Point(0, 64), Alpha(155));
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
		int ring = 60 - m_frameCount;
		Circle(m_pos - camerapos, ring).drawFrame(1, 1, Palette::Blue);
	}

	//Rect(m_hitRect.pos-camerapos,m_hitRect.size).drawFrame(10,10,Palette::Black);
	//Circle(m_pos - camerapos, 10).draw(Palette::Red);
}

ELEnemySize ELSeaserpentTentacle::getEnemySize() const
{
	return m_size;
}

Point ELSeaserpentTentacle::getPos() const
{
	return m_pos;
}

bool ELSeaserpentTentacle::isHit(const Rect &body) const
{
	if (m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged)
		return false;

	return body.intersects(m_hitRect);
}

int ELSeaserpentTentacle::getDamage() const
{
	return kDamage;
}

Rect ELSeaserpentTentacle::getHitRect() const
{
	return m_hitRect;
}

void ELSeaserpentTentacle::giveDamage(int damage)
{
	if (damage >0 && m_state != ELEnemyState::Dead && m_state != ELEnemyState::Damaged)
	{
		m_HP = Max(m_HP - damage, 0);

		SoundAsset(L"sndELEnemyDamaged").play();
		m_state = ELEnemyState::Damaged;
		m_frameCount = 0;
	}

	if (m_HP == 0)
	{
		m_state = ELEnemyState::Dead;
	}
}

Point ELSeaserpentTentacle::getStartPos() const
{
	return m_startPos;
}

bool ELSeaserpentTentacle::isErased() const
{
	return m_erased;
}

bool ELSeaserpentTentacle::isDamaged() const
{
	return m_state == ELEnemyState::Damaged || m_state == ELEnemyState::Dead;
}

void ELSeaserpentTentacle::setPos(const Point& pos)
{
	m_pos = pos;
	m_startPos = pos;
}