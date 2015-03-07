# include "ELRockfall.h"


ELRockfall::ELRockfall(
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
	TextureAsset::Register(L"texELrockfall",L"data/Elis/Effect/Bigslime/rock.png");
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

	const int kwidth=128;
	const int kheight=128;

	const int kx = 34;
	const int ky = 16;
	const int kw = 59;
	const int kh = 112;
	
	const int kAnimeTiming = 8;
	const int kEraseTime = kAnimeTiming*3;
};

void ELRockfall::update(const ELMap& map)
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
			const int y = int(m_pos.y) + 64;

			for(int i=1;i<=kSpeed;++i)
			{
				if(map.isFloor(int(m_pos.x),int(y + i))
					&& map.getFloorType(int(m_pos.x),int(y + i)) == ELFloorType::NormalFloor)
				{
					m_pos.y += i;
					m_collided = true;
					m_frameCount = 0;
					playEraseSound();
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

void ELRockfall::draw(const Point& camerapos)const
{
	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	if(m_collided)
	{
		TextureAsset(L"texELrockfall")
			(0,(m_frameCount/kAnimeTiming)%8*kheight +1
			,kwidth,kheight)
			.drawAt(m_hitRect.center-camerapos);
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}
	
	TextureAsset(L"texELrockfall")
		(0,0
		,kwidth,kheight)
		.drawAt(m_hitRect.center-camerapos);


	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//LOGFMT_ERR((m_frameCount/kAnimeTiming)%8);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELRockfall::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELRockfall::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects( 
			m_hitRect))
		{
			return true;
		}
	}

	return false;
}

ELShooter ELRockfall::getShooter() const
{
	return m_shooter;
}

int ELRockfall::getDamage() const
{
	return m_damege;
}

bool ELRockfall::isErased() const
{
	return m_erased;
}

ELShootType ELRockfall::getShootType() const
{
	return m_shootType;
}

void ELRockfall::giveErase()
{
	m_collided = true;
}