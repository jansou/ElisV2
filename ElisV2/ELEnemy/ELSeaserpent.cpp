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
m_pos(Point(640,140)),
m_startPos(Point(640, 140)),
m_state(ELEnemyState::Staying),
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
m_attackType(ELSeaserpentAttack::Staying),
m_oldAttackType(ELSeaserpentAttack::null),
m_hitRect(990,170,80,70),
m_maxHP(HP),
m_head_updown(true)
{
	TextureAsset::Register(L"texELSeaserpent_head_stay",L"data/Elis/Enemy/Seaserpent/head_stay.png");
	TextureAsset::Register(L"texELSeaserpent_body_stay",L"data/Elis/Enemy/Seaserpent/body_stay.png");
	TextureAsset::Register(L"texELSeaserpent_tail_stay",L"data/Elis/Enemy/Seaserpent/tail_stay.png");
	
	ELSeaserpentTentacle item;
	m_tentacles.push_back(item);
	m_tentacles.push_back(item);

	m_tentacles[0].setPos(Point(950, 740));
	m_tentacles[1].setPos(Point(/*1230*/330, 740));

	TextureAsset::Register(L"texELSeaserpent_seaser_head", L"data/Elis/Enemy/Seaserpent/seaser_head.png");
	TextureAsset::Register(L"texELSeaserpent_seaser_body", L"data/Elis/Enemy/Seaserpent/seaser_body.png");
	TextureAsset::Register(L"texELSeaserpent_seaser_attack_head", L"data/Elis/Enemy/Seaserpent/seaser_head_attack.png");
	TextureAsset::Register(L"texELSeaserpent_seaser_body_front", L"data/Elis/Enemy/Seaserpent/seaser_body_front.png");
	

}

namespace
{
	Point kBeginFrontNode(1180,740);
	Point kBeginBackNode(1020,740);

	const int kAnimeTiming = 8;
	const int kGravity = 10;
	const int kSpeed = 3;

	const int kHeadSpeed = 1;
	const int kHeadShakeRange = 25;
	const int kBiteSpeed = 10;

	const int kMaxJunpH =23;
	const int kInterval = 80;
	const int kDrop = 1;
	const int kRiseOffset = 3;
	const int kDamage = 3;//ê⁄êGéûÇÃÉ_ÉÅÅ[ÉW
	const int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kAttackTime = kAnimeTiming*8;
	const int kShotTime = kAnimeTiming*3;
	const int kDamageTime = kAnimeTiming*4;

	const int kReadyTime = 30;

	//êGéËÇÃäJénínì_
	const Point kBeginNodes(1230,552);

	int kNodeSize=10;
};

void ELSeaserpent::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	
	if (m_state == ELEnemyState::Staying)
	{
		//////éüÇÃçUåÇÇåàÇﬂÇÈ
		if (m_frameCount > 120 && m_attackType == ELSeaserpentAttack::Staying)
		{
			if (m_oldAttackType == ELSeaserpentAttack::null
				|| m_oldAttackType == ELSeaserpentAttack::UpperTentacle)
			{
				m_attackType = ELSeaserpentAttack::Bite;
				m_oldAttackType = ELSeaserpentAttack::Bite;
				m_state = ELEnemyState::Moving;
				m_targetPlayerPos = playerpos;

				//LOG_ERROR(L"!!!");
			}
			else if (m_oldAttackType == ELSeaserpentAttack::Bite)
			{
				m_attackType = ELSeaserpentAttack::UpperTentacle;
				m_oldAttackType = ELSeaserpentAttack::UpperTentacle;

				//LOG_ERROR(L"???");
			}
			else
			{
				m_attackType = ELSeaserpentAttack::Bite;
				m_oldAttackType = ELSeaserpentAttack::Bite;
				m_state = ELEnemyState::Moving;
				m_targetPlayerPos = playerpos;
			}

			m_frameCount = 0;
		}
	}

	if (m_attackType == ELSeaserpentAttack::Bite)
	{
		if (m_state == ELEnemyState::Moving)
		{
			if (playerpos.x > m_pos.x)
			{
				m_face = ELEnemyFace::EnemyRight;
			}
			else
			{
				m_face = ELEnemyFace::EnemyLeft;
			}

			if (m_face == ELEnemyFace::EnemyLeft)
			{
				//m_pos.x = Max(m_pos.x - kSpeed, m_targetPlayerPos.x);
				m_pos.x = Max(m_pos.x - kSpeed, playerpos.x);
			}
			else if (m_face == ELEnemyFace::EnemyRight)
			{
				//m_pos.x = Min(m_pos.x + kSpeed, m_targetPlayerPos.x);
				m_pos.x = Min(m_pos.x + kSpeed, playerpos.x);
			}


			if (m_head_updown)//ì™Ç†Ç∞ÇÈ
			{
				m_pos.y = Max(m_startPos.y, m_pos.y - kHeadSpeed);

				if (m_startPos.y >= m_pos.y)
				{
					m_head_updown = false;
				}
			}
			else//ì™â∫Ç∞ÇÈ
			{
				m_pos.y = Min(m_startPos.y + kHeadShakeRange, m_pos.y + kHeadSpeed);

				if (m_startPos.y + kHeadShakeRange <= m_pos.y)
				{
					m_head_updown = true;
				}
			}

			//if (m_pos.x == m_targetPlayerPos.x)
			if (/*m_pos.x == playerpos.x
				||*/ m_frameCount > 200)
			{
				m_state = ELEnemyState::Attacking;
				m_frameCount = 0;

				m_head_updown = false;
			}
		}
		else if (m_state == ELEnemyState::Attacking)
		{
			if (!m_head_updown)
			{
				m_pos.y = Min(m_pos.y + kBiteSpeed, m_startPos.y + 450);

				if (m_startPos.y + 450 == m_pos.y)
				{
					m_head_updown = true;
				}
			}
			else
			{
				m_pos.y = Max(m_pos.y - kBiteSpeed, m_startPos.y);

				if (m_startPos.y == m_pos.y)
				{
					m_head_updown = false;
					m_state = ELEnemyState::Staying;
					m_frameCount = 0;
					m_attackType = ELSeaserpentAttack::Staying;
				}
			}
			////////////////////////////////
		}
	}
	
	if (m_attackType == ELSeaserpentAttack::Staying)
	{
		if (m_tentacles.size() < 3 && m_HP < m_maxHP * 2 / 3)
		{
			//ÇRñ{ñ⁄ê∂Ç‚Ç∑
			ELSeaserpentTentacle item;
			m_tentacles.push_back(item);
			m_tentacles[2].setPos(Point(1050, 780));
		}
		else if (m_tentacles.size() < 4 && m_HP < m_maxHP / 3)
		{
			//ÇSñ{ñ⁄ê∂Ç‚Ç∑
			ELSeaserpentTentacle item;
			m_tentacles.push_back(item);
			m_tentacles[3].setPos(Point(230, 780));
		}
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

	//êGéËÇ÷ÇÃçUåÇéwóﬂ///////////////////////////////////////////////
	if (m_attackType == ELSeaserpentAttack::UpperTentacle)
	{
		if (m_frameCount == 30*0)
		{
			m_tentacles[0].setState(ELEnemyState::Moving);
		}
		if (m_frameCount == 30*1)
		{
			m_tentacles[1].setState(ELEnemyState::Moving);
		}
		if (m_tentacles.size()>2 &&  m_frameCount == 30 * 2)
		{
			m_tentacles[2].setState(ELEnemyState::Moving);
		}
		if (m_tentacles.size()>3 && m_frameCount == 30 * 3)
		{
			m_tentacles[3].setState(ELEnemyState::Moving);
		}

		bool flag = true;
		for (size_t i = 0; i < m_tentacles.size(); ++i)
		{
			flag *= (m_tentacles[i].getEnemyState() != ELEnemyState::Moving);
		}
		//LOG_ERROR(flag);

		if (flag)
		{
			
			m_attackType = ELSeaserpentAttack::Staying;
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
	}
	/////////////////////////////////////
	for (ELSeaserpentTentacle &t : m_tentacles)
	{
		t.update(map, playerpos, object, item, attack, camerapos);
	}

	m_hitRect=Rect(m_pos - Point(60, 100), 120, 200);

	++m_frameCount;
}

void ELSeaserpent::Attack_UpperTentacle(ELAttack& attack, const Point& playerpos)
{
	static int framecount = 0;
	
	if(framecount == 0)
	{
		attack.makeShoot(ELShooter::Enemy,ELShootType::UpperTentacle,Point(playerpos.x,ELWindowHeight),Point(0,0));
	}

	++framecount;

	if(framecount>60)
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

bool ELSeaserpent::intersectBody(const Point& foot,const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
{
	for (int i = 1; i < foot.y - head.y; ++i)
	{
		if (map.isFloor(foot.x, foot.y - i) && map.getFloorType(foot.x, foot.y - i) == ELFloorType::NormalFloor)
		{
			return true;
		}
	}
	return false;
}

void ELSeaserpent::draw(const Point& camerapos, const Point& modifyDrawPos)const
{
	const int a = 255 -100*m_invisibled;

	if (m_state == ELEnemyState::Attacking)
	{
		TextureAsset(L"texELSeaserpent_seaser_body").draw(m_pos.x - 170 / 2, m_startPos.y + 100);
		TextureAsset(L"texELSeaserpent_seaser_body_front").resize(170, m_pos.y - m_startPos.y).draw(m_pos.x - 170 / 2, m_startPos.y);
		TextureAsset(L"texELSeaserpent_seaser_attack_head").draw(m_pos - Point(180, 260) / 2);
	}
	else
	{
		TextureAsset(L"texELSeaserpent_seaser_body").draw(m_pos.x - 170 / 2, m_startPos.y + 100);
		TextureAsset(L"texELSeaserpent_seaser_head").draw(m_pos - Point(180, 260) / 2);
	}
	/*
	drawTail_stay();

	drawBody_stay();

	drawHead_stay();
	*/
	for (const ELSeaserpentTentacle& t : m_tentacles){
		t.draw(camerapos, modifyDrawPos);
	}
	

	m_hitRect.drawFrame(10, 0, Palette::Black);

}

void ELSeaserpent::drawHead_stay() const
{
	static int framecount = 0;
	static int offset=0;

	if(framecount % kAnimeTiming == 0)
	{
		if(framecount == kAnimeTiming*3 || framecount == kAnimeTiming*5)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*11 || framecount == kAnimeTiming*13)
		{
			offset += 1; 
		}
	}

	int a = 255 - 100 * m_invisibled;

	TextureAsset(L"texELSeaserpent_head_stay").draw(949,60+offset,Alpha(a));

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
		if(framecount == kAnimeTiming*2 || framecount == kAnimeTiming*4)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*10 || framecount == kAnimeTiming*12)
		{
			offset += 1; 
		}
	}

	TextureAsset(L"texELSeaserpent_body_stay").draw(1057,377+offset);

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
		if(framecount == kAnimeTiming*6 || framecount == kAnimeTiming*8)
		{
			offset -= 1; 
		}
		else if(framecount == kAnimeTiming*14 || framecount == kAnimeTiming*16)
		{
			offset += 1; 
		}
	}
	
	TextureAsset(L"texELSeaserpent_tail_stay").draw(947,426+offset);

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
	if (m_state == ELEnemyState::Dead || m_invisibled){
		return false;
	}
	//LOGFMT_ERR();
	
	return body.intersects(m_hitRect);
}

int ELSeaserpent::getDamage() const
{
	return kDamage;
}

Rect ELSeaserpent::getHitRect() const//íeÇ…ÇΩÇ¢ÇµÇƒÇÃìñÇΩÇËîªíËÇ…égÇ§
{
	return m_hitRect;
}

void ELSeaserpent::giveDamage(int damage)//Ç±ÇÃìGÇ…É_ÉÅÅ[ÉWÇó^Ç¶ÇÈ
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

Point ELSeaserpent::getStartPos() const//èâä˙à íuÇÃéÊìæ
{
	return m_startPos;
}

bool ELSeaserpent::isErased() const//è¡ñ≈ÇµÇƒÇ‡ÇÊÇ¢Ç©
{
	return m_erased;
}

bool ELSeaserpent::isDamaged() const//É_ÉÅÅ[ÉWÇêHÇÁÇ¡ÇƒÇ¢ÇÈÇ©
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
	TextureAsset::Register(L"texELTentacle_fin", L"data/Elis/Enemy/Tentacle/fin.png");
}

void ELSeaserpentTentacle::update(const ELMap& map, const Point& playerpos, ELObjectInfo& object, ELItem &item, ELAttack &attack, const Point &camerapos)
{
	const int kSpeed = 10;
	const int kHigh = 400;

	if (m_state == ELEnemyState::Dead && m_oldState != ELEnemyState::Dead){
		SoundAsset(L"sndELEnemyDefeat").play();
	}

	m_oldState = m_state;

	if (m_state == ELEnemyState::Dead)
	{
		m_pos.y = Min(m_pos.y + kSpeed / 2, m_startPos.y);

		//ïúäà
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
		//è„è∏
		m_pos.y = Max(m_pos.y - kSpeed, m_startPos.y - kHigh);

		//íBÇµÇΩÇÁ
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
			m_state = ELEnemyState::Attacking;
			m_frameCount = 0;
		}
		m_playerpos = playerpos;
	}
	else if (m_state == ELEnemyState::Attacking)
	{
		if (m_frameCount > 30)
		{
			//ïXÇîÚÇŒÇ∑
			SoundAsset(L"sndElisIce").play();
			attack.makeShoot(ELShooter::Enemy, ELShootType::LongIce, m_pos, Point(m_playerpos.x, m_playerpos.y));
			m_state = ELEnemyState::Staying;
			m_frameCount = 0;
		}
	}
	else if (m_state == ELEnemyState::Moving)
	{
		//â∫ç~
		m_pos.y = Min(m_pos.y + kSpeed / 2, m_startPos.y);

		if (m_frameCount == 50)
		{
			//ë≈Çøè„Ç∞ïX
			attack.makeShoot(ELShooter::Enemy,ELShootType::UpperTentacle,Point(playerpos.x, ELWindowHeight),Point(0, 0));
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

bool ELSeaserpentTentacle::intersectBody(const Point& foot, const Point& head, const ELMap& map)//í èÌè∞Ç∆êgëÃÇÃê⁄êGîªíË
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

	const auto drawTentacle = [&](){
		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64), Atan2(direction(m_pos, m_playerpos).y, direction(m_pos, m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64));
	};
	const auto drawTentacleAlpha = [&](){
		TextureAsset(L"texELTentacle_fin")
			.rotateAt(Point(0, 64), Atan2(direction(m_pos, -m_playerpos).y, direction(m_pos, -m_playerpos).x))
			.draw(m_pos - camerapos - Point(0, 64),Alpha(155));
	};

	switch (m_state)
	{
	case ELEnemyState::Ready:
	{
		drawTentacle();
		break;
	}
	case ELEnemyState::Staying:
	{
		//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));
		drawTentacle();
		break;
	}
	case ELEnemyState::Moving:
	{
		//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));
		drawTentacle();
		break;
	}
	case ELEnemyState::Attacking:
	{
		//TextureAsset(L"texELTentacle_fin").draw(m_pos - camerapos - Point(64, 128));
		drawTentacle();
		break;
	}
	case ELEnemyState::Damaged:
	{
		drawTentacleAlpha();
		break;
	}
	case ELEnemyState::Dead:
	{
		drawTentacleAlpha();
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
	if (m_state == ELEnemyState::Dead || m_state == ELEnemyState::Damaged){
		return false;
	}

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