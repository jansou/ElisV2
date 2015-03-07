# include "ELElisWind.h"


ELElisWind::ELElisWind(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_collided(false),//è’ìÀîªíË
	m_erased(false),
	m_frameCount(0),
	m_burned(false),
	m_rad(0.0)
{
	
	TextureAsset::Register(L"texElisWind",
		L"data/Elis/Effect/Elis/elisWind.png");
	TextureAsset::Register(L"texElisWind_burn",
		L"data/Elis/Effect/Elis/elisWind_burn.png");
	TextureAsset::Register(L"texElisWind_erase",
		L"data/Elis/Effect/Elis/elisWind_erase.png");	
}

namespace
{
	const double kSpeed = 5.0;

	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kx = 11;
	const int ky = 11;
	const int kw = 106;
	const int kh = 106;

	const int kAnimeTiming = 8;
	const int kEraseTime = 40;

	const int kLittleR = 10;
	const int kBigR = 53;
	const double kRadSpeed = 7.5;

	const int kLittleDamage = 1;
	const int kBigDamage = 6;

	const int kBurnTime = kAnimeTiming*10;
};

void ELElisWind::update(const ELMap& map)
{
	if(m_collided && !m_burned)
	{
		/*if(m_frameCount<kEraseTime-1)
		{
			m_frameCount += 1;
		}
		else*/
		{
			m_erased = true;
		}
		return;
	}
	else if(m_burned && m_collided && m_frameCount == kAnimeTiming*3-2)
	{
		m_erased = true;
	}
	else if(m_burned && m_frameCount > kBurnTime)
	{
		m_collided = true;
		m_frameCount = 0;
	}

	if(!m_collided && !m_burned)
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

	if(!m_burned && m_pos.distanceFrom(m_endpos) < 5)
	{
		m_burned = true;
		m_frameCount = 0;
	}

	m_hitCir = Circle(m_pos, (m_burned) ? kBigR : kLittleR);

	m_frameCount +=1;

	m_rad += kRadSpeed;

	return;
}

void ELElisWind::draw(const Point& camerapos)const
{
	//LOGFMT_ERR("shoot!!!!!");
	
	if(m_collided && m_burned)
	{
		TextureAsset(L"texElisWind_erase")
			(
			0
			,(m_frameCount/kAnimeTiming)%(height(L"texElisWind_erase")/128)*128-1
			,128,128
			)
			.rotateAt(64,64,-Radians(m_rad))
			.drawAt(m_pos-camerapos);
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}
	

	if(!m_burned)
		TextureAsset(L"texElisWind")
		.rotateAt(64,64,-Radians(m_rad))
			.drawAt(m_pos-camerapos);
	else
	{
		TextureAsset(L"texElisWind_burn")
			(0
			,(kAnimeTiming*2>m_frameCount) ? (m_frameCount/kAnimeTiming)%(height(L"texElisWind_burn")/128)*128-1 : 128*2
			,128,128)
			.rotateAt(64,64,-Radians(m_rad))
			.drawAt(m_pos-camerapos);
	}

	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELElisWind::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELElisWind::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects(m_hitCir))
		{
			if(!m_burned)
			{
				m_collided = true;
				m_frameCount = 0;
			}

			return true;
		}
	}

	return false;
}

ELShooter ELElisWind::getShooter() const
{
	return m_shooter;
}

int ELElisWind::getDamage() const
{
	return (m_burned) ? kBigDamage : kLittleDamage;
}

bool ELElisWind::isErased() const
{
	return m_erased;
}

ELShootType ELElisWind::getShootType() const
{
	return m_shootType;
}

void ELElisWind::giveErase()
{
	m_collided = true;
}