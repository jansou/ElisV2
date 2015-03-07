
# include "ELLongIce.h"


ELLongIce::ELLongIce(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_damege(3),//ƒ_ƒ[ƒW—Ê
	m_collided(false),//Õ“Ë”»’è
	m_erased(false),
	m_frameCount(0)
{
	/*
	TextureAsset::Register(L"texElisIce",
		L"data/Elis/Effect/Elis/elisIce.png");
	TextureAsset::Register(L"texElisIce_erase",
		L"data/Elis/Effect/Elis/elisIce_erase.png");
		*/
}

namespace
{
	const double kSpeed = 10.0;
	
	const int kbreakTime = 10;

	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}
	
	const int kx = 9;
	const int ky = 37;
	const int kw = 111;
	const int kh = 54;

	const int kAnimeTiming = 8;
	const int kEraseTime = 24;

};

void ELLongIce::update(const ELMap& map)
{
	/*
	if(!m_collided && m_frameCount > kbreakTime)
	{
		m_collided = true;
		m_frameCount = 0;
	}
	*/
	if(m_collided)
	{
		if(m_frameCount<kEraseTime-1)
		{
			m_frameCount += 1;
		}
		else
		{
			m_erased = true;
		}
		return;
	}

	if(!m_collided)
	{
		for(int i=1;i<=kSpeed;++i)
		{
			if(map.isFloor(int(m_pos.x + direction().x*i),int(m_pos.y + direction().y*i))
				&& map.getFloorType(int(m_pos.x + direction().x*i),int(m_pos.y + direction().y*i)) == ELFloorType::NormalFloor)
			{
				m_pos += direction()*i;
				m_collided = true;
				m_frameCount = 0;

				break;
			}
			else if(i==kSpeed)
			{
				m_pos += direction()*kSpeed;
			}
		}
	}
	m_hitRect = Rect(int(m_pos.x - 128/2 + kx), int(m_pos.y - 128/2 + ky), kw,kh);

	/*m_hitTriangle = Triangle(m_pos.x - 128/2 +9 ,m_pos.y - 128/2 +37,
		m_pos.x - 128/2 +9 ,m_pos.y - 128/2 +91,
		m_pos.x - 128/2 +120 ,m_pos.y - 128/2 +64);
		*/
	m_frameCount +=1;

	return;
}

void ELLongIce::draw(const Point& camerapos)const
{
	//LOGFMT_ERR("shoot!!!!!");

	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	if(m_collided)
	{
		TextureAsset(L"texElisIce_erase")(0,(m_frameCount/kAnimeTiming)%(height(L"texElisIce_erase")/128)*128,128,128).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos);
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}

	TextureAsset(L"texElisIce")(0,0,128,128).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos);


	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

//	m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELLongIce::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELLongIce::isHit(const Rect& body)
{
	if(m_frameCount < 3)//‘«Œ³‚ª‰ó‚ê‚é‚Ì‚ð–h‚®‚æ‚¤
		return false;

	if(!m_collided)
	{
		if(body.intersects( 
			m_hitRect.rotatedAt(
				m_hitRect.center.x,
				m_hitRect.center.y,
				Atan2(direction().y,direction().x)
				)))
		{
			//m_collided = true;
			//m_frameCount = 0;
			return true;
		}
	}

	return false;
}

ELShooter ELLongIce::getShooter() const
{
	return m_shooter;
}

int ELLongIce::getDamage() const
{
	return m_damege;
}

bool ELLongIce::isErased() const
{
	return m_erased;
}

ELShootType ELLongIce::getShootType() const
{
	return m_shootType;
}

void ELLongIce::giveErase()
{
	m_collided = true;
}