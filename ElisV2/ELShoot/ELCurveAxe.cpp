# include "ELCurveAxe.h"


ELCurveAxe::ELCurveAxe(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Point& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_Speed(endpos),
	m_pos(beginpos),
	m_damege(3),//É_ÉÅÅ[ÉWó 
	m_collided(false),//è’ìÀîªíË
	m_erased(false),
	m_frameCount(0),
	m_rad(0.0)
{
	TextureAsset::Register(L"texELAxe",L"data/Elis/Effect/Enemy/Axe.png");
}

namespace
{
	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kx = 0;
	const int ky = 0;
	const int kw = 12;
	const int kh = 26;

	const double kvx = 15.0;
	const double kvy = 40.0;
	const double kg = 10.0;
	const int ksecond = 6;

	const int kAnimeTiming = 8;
	const int kEraseTime = 40;

	const double kRadSpeed = 7.5;
};

void ELCurveAxe::update(const ELMap& map)
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
		bool flag = false;

		int dirx = (m_Speed.x>0) ? 1 : -1;
		int diry = (m_Speed.y>0) ? 1 : -1;


		for(int i=1;i<=std::abs(m_Speed.x);++i)
		{
			for(int j=1;j<=std::abs(m_Speed.y);++j)
			{
				if(map.isFloor(int(m_pos.x + i*dirx),int(m_pos.y - j*diry))
					&& map.getFloorType(int(m_pos.x + i*dirx),int(m_pos.y - j*diry)) == ELFloorType::NormalFloor)
				{
					m_pos += Point(i*dirx,-j*diry);
					m_collided = true;
					m_frameCount = 0;

					flag = true;

					break;
				}
				
				if(j==std::abs(m_Speed.y))
				{
					m_pos.y -= m_Speed.y;
					break;
				}
			}

			if(flag) break;

			if(i==std::abs(m_Speed.x))
			{
				m_pos.x += m_Speed.x;
				break;
			}
		}
	}
	m_hitRect = Rect(int(m_pos.x - kw/2 + kx), int(m_pos.y - kh/2 + ky), kw,kh);

	m_frameCount +=1;

	if(m_frameCount%5 == 0)
		m_Speed.y = Max(m_Speed.y-1,-2);

	m_rad += kRadSpeed;

	return;
}

void ELCurveAxe::draw(const Point& camerapos)const
{
	bool face = m_Speed.x > 0;

	if(m_collided)
	{
		TextureAsset(L"texELAxe")(kw*face,ky,kw,kh)
			.rotateAt(kw/2,kh/2,((face) ? 1 :-1) * Radians(m_rad))
			.drawAt(m_hitRect.center-camerapos,Alpha(255-150));
	}
	else
	{
		TextureAsset(L"texELAxe")(kw*face,ky,kw,kh)
			.rotateAt(kw/2,kh/2,((face) ? 1 :-1) * Radians(m_rad))
			.drawAt(m_hitRect.center-camerapos);
	}
}

Point ELCurveAxe::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELCurveAxe::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects( 
			m_hitRect/*.rotatedAt(kw/2,kh/2,-Radians(m_rad))*/))
		{
			m_collided = true;
			m_frameCount = 0;
			return true;
		}
	}
	
	return false;
}

ELShooter ELCurveAxe::getShooter() const
{
	return m_shooter;
}

int ELCurveAxe::getDamage() const
{
	return m_damege;
}

bool ELCurveAxe::isErased() const
{
	return m_erased;
}

ELShootType ELCurveAxe::getShootType() const
{
	return m_shootType;
}

void ELCurveAxe::giveErase()
{
	m_collided = true;
}