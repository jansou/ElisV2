# include "ELArrow.h"


ELArrow::ELArrow(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(beginpos),
	m_damege(3),//É_ÉÅÅ[ÉWó 
	m_collided(false),//è’ìÀîªíË
	m_erased(false)
{
	TextureAsset::Register(L"texELArrow",
		L"data/Elis/Effect/Enemy/Arrow.png");
}

namespace
{
	const double kSpeed = 15.0;

	inline int width(const String &name)
	{
		return TextureAsset(name).width;
	}

	inline int height(const String &name)
	{
		return TextureAsset(name).height;
	}

	const int kx = 10;
	const int ky = 30;
	const int kw = 43;
	const int kh = 3;

	const int kAnimeTiming = 8;
	const int kEraseTime = 40;
};
void ELArrow::update(const ELMap& map)
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
	m_hitRect = Rect(int(m_pos.x - 64/2 + kx), int(m_pos.y - 64/2 + ky), kw,kh);

	m_frameCount +=1;

	return;
}

void ELArrow::draw(const Point& camerapos)const
{
	//LOGFMT_ERR("shoot!!!!!");

	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	if(m_collided)
	{
		TextureAsset(L"texELArrow")(kx,ky,kw,kh).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos,Alpha(255-150));
		//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
		return;
	}

	TextureAsset(L"texELArrow")(kx,ky,kw,kh).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos);


	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELArrow::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELArrow::isHit(const Rect& body)
{
	if(!m_collided)
	{
		if(body.intersects( 
			m_hitRect.rotatedAt(
				m_hitRect.center.x,
				m_hitRect.center.y,
				Atan2(direction().y,direction().x)
				)))
		{
			m_collided = true;
			m_frameCount = 0;
			return true;
		}
	}

	return false;
}

ELShooter ELArrow::getShooter() const
{
	return m_shooter;
}

int ELArrow::getDamage() const
{
	return m_damege;
}

bool ELArrow::isErased() const
{
	return m_erased;
}

ELShootType ELArrow::getShootType() const
{
	return m_shootType;
}

void ELArrow::giveErase()
{
	m_collided = true;
}