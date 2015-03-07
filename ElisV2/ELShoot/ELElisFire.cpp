# include "ELElisFire.h"


ELElisFire::ELElisFire(
	const ELShootType& shootType,
		const ELShooter& shooter,
		const Vec2& beginpos,
		const Vec2& endpos):
	m_shootType(shootType),
	m_shooter(shooter),
	m_beginpos(beginpos),
	m_endpos(endpos),
	m_pos(Point(int(beginpos.x), int(beginpos.y))),
	m_damege(2),//ダメージ量
	m_collided(false),//衝突判定
	m_erased(false),
	m_frameCount(0)
{
	TextureAsset::Register(L"texELFire",
		L"data/Elis/Effect/Elis/elisFire.png");
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
	//86,91,98,76
	
	const int kx = 86;
	const int ky = 91;
	const int kw = 98;
	const int kh = 76;

	const int kAnimeTiming = 4;
	const int kEraseTime = 40;
};

void ELElisFire::update(const ELMap& map)
{

	if(m_frameCount == 12)
	{
		m_erased = true;
		return;
	}
	/*
	if(m_frameCount==60-2)
	{
		m_erased = true;
		return;
	}
	
	if(m_collided)
	{
		m_hitRect = Rect(int(m_pos.x - 256/2 + kx), int(m_pos.y - 256/2 + ky), kw,kh);

		m_frameCount +=1;

		return;
	}

	Vec2 dir = direction();

	const Point foot = Point(m_pos.x,m_pos.y+37);
	const Point head = Point(m_pos.x,m_pos.y-37);

	for(int i=0;i<=kSpeed;++i)
	{
		if(intersectBody(Point(int(foot.x+dir.x*i),int(foot.y+dir.y*i)),
			Point(int(head.x+dir.x*i),int(head.y+dir.y*i)),map,ELFloorType::NormalFloor)
			//||object.isTouchable(Point(int(head.x+dir.x*i),int(head.y+dir.y*i)),Point(int(foot.x+dir.x*i),int(foot.y+dir.y*i)),camerapos)
			)
		{
			m_pos = Point(int(m_pos.x + dir.x*(i-1)),int(m_pos.y + dir.y*(i-1)));

			break;
		}
		else if(i==kSpeed)
		{
			m_pos = Point(int(m_pos.x + dir.x*kSpeed),int(m_pos.y + dir.y*kSpeed));
		}
	}

	//44,54,40,74

	//m_hitRect = Rect(int(m_pos.x - 256/2 + kx), int(m_pos.y - 256/2 + ky), kw,kh);
	m_hitRect = Rect(int(m_pos.x - 20), int(m_pos.y - 37), 40,74);

	//m_line = Line(Vec2(int(m_pos.x), int(m_pos.y - 37)),Vec2(int(m_pos.x), int(m_pos.y + 37)));//
	*/
	m_hitRect = Rect(int(m_pos.x - 100), int(m_pos.y - 37), 200,74);
	m_frameCount +=1;
	
	return;
}

bool ELElisFire::intersectBody(const Point& foot,const Point& head, const ELMap& map, const ELFloorType& floorType)//通常床と身体の接触判定
{

	for(int i=1;i<foot.y-head.y;++i)
	{
		if(map.isFloor(foot.x,foot.y-i) && map.getFloorType(foot.x,foot.y-i)==floorType)
		{
			return true;
		}
	}

	return false;
}

void ELElisFire::draw(const Point& camerapos)const
{
	//LOGFMT_ERR("shoot!!!!!");

	//Rect(m_hitRect.pos.x-camerapos.x,m_hitRect.pos.y-camerapos.y,m_hitRect.size.x,m_hitRect.size.y).draw(Palette::Aliceblue);
	
	/*TextureAsset(L"texELFire")(0,(m_frameCount/kAnimeTiming)%(height(L"texELFire")/256)*256,256,256).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos,Alpha(200));
				*/

	TextureAsset(L"texELFire")(0,(m_frameCount/kAnimeTiming)%(height(L"texELFire")/256)*256,256,256).
			rotate(
				Atan2(direction().y,direction().x)
				).drawAt(m_hitRect.center-camerapos,Alpha(150));

	//Circle(m_pos.x-camerapos.x,m_pos.y-camerapos.y,10).draw(Palette::Black);
	//Circle(m_endpos.x-camerapos.x,m_endpos.y-camerapos.y,10).draw(Palette::Black);

	//Resource::Font(L"font").draw(Format()+direction(),0,40,Palette::Blue);

	//m_hitRect.drawFrame(1,1,Palette::Blue);
}

Point ELElisFire::getPos() const
{
	return Point(int(m_pos.x),int(m_pos.y));
}

bool ELElisFire::isHit(const Rect& body)
{
	if(!m_collided)
	{
		

		/*if(body.intersects( 
			m_hitRect.rotatedAt(
				m_hitRect.center.x,
				m_hitRect.center.y,
				Atan2(direction().y,direction().x)
				)))*/
		if(body.intersects(m_hitRect))
		{
			//if(body.intersects(m_hitRect.center))
				//m_collided = true;
			
			return true;
		}
	}


	return false;
}

ELShooter ELElisFire::getShooter() const
{
	return m_shooter;
}

int ELElisFire::getDamage() const
{
	return m_damege;
}

bool ELElisFire::isErased() const
{
	return m_erased;
}

ELShootType ELElisFire::getShootType() const
{
	return m_shootType;
}

void ELElisFire::giveErase()
{
	m_collided = true;
}