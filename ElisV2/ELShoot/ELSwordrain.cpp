# include "ELSwordrain.h"


ELSwordrain::ELSwordrain(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_damege(4),//É_ÉÅÅ[ÉWó 
	m_collided(false),//è’ìÀîªíË
	m_erased(false),
	m_frameCount(0)
{
	//TextureAsset::Register(L"texELsword",L"data/Elis/Effect/Darkbrave/sword.png");
}

namespace
{
	const int kSpeed = 16;

	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kwidth=170;
	const int kheight=300;

	const int kx = 86;
	const int ky = 47;
	const int kw = 9;
	const int kh = 233;
	
	const int kAnimeTiming = 8;
	const int kEraseTime = 40;
};

void ELSwordrain::update(const ELMap& map)
{
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
		if(m_frameCount > kAnimeTiming*8)
		{
			for(int i=1;i<=kSpeed;++i)
			{
				if(map.isFloor(int(m_pos.x),int(m_pos.y + i))
					&& map.getFloorType(int(m_pos.x),int(m_pos.y + i)) == ELFloorType::NormalFloor)
				{
					m_pos.y += i;
					m_collided = true;
					m_frameCount = 0;

					break;
				}
				else if(i==kSpeed)
				{
					m_pos.y += kSpeed;
				}
			}
		}
	}
	m_hitRect = Rect(int(m_pos.x - kwidth/2 + kx), int(m_pos.y - kheight/2 + ky), kw,kh);

	m_frameCount +=1;

	return;
}

void ELSwordrain::draw(const Point& camerapos)const
{
	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	if(m_collided)
	{
		TextureAsset(L"texELsword")(0,kheight*6,kwidth,kheight)
			.drawAt(m_hitRect.center-camerapos,Alpha(155));
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}

	if(m_frameCount > kAnimeTiming*6)
	{
		TextureAsset(L"texELsword")(0,6*kheight,kwidth,kheight)
		.drawAt(m_hitRect.center-camerapos);
		return;
	}

	TextureAsset(L"texELsword")(0,(m_frameCount/kAnimeTiming)%8*kheight,kwidth,kheight)
		.drawAt(m_hitRect.center-camerapos);


	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//LOGFMT_ERR((m_frameCount/kAnimeTiming)%8);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELSwordrain::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELSwordrain::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects( 
			m_hitRect))
		{
			//m_collided = true;
			//m_frameCount = 0;
			return true;
		}
	}

	return false;
}

ELShooter ELSwordrain::getShooter() const
{
	return m_shooter;
}

int ELSwordrain::getDamage() const
{
	return m_damege;
}

bool ELSwordrain::isErased() const
{
	return m_erased;
}

ELShootType ELSwordrain::getShootType() const
{
	return m_shootType;
}

void ELSwordrain::giveErase()
{
	m_collided = true;
}