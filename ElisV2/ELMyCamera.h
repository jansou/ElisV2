# pragma once
# include<Siv3D.hpp>
# include "ELCommon.h"

class ELMyCamera
{
private:
	Point m_camerapos;

	int m_framecount;

	Point m_burstPos;

	ELCameraGraphicEffect m_graphicEffect;

	bool m_flashed;
public:
	ELMyCamera();

	void update(const Point& playerPos,const Point& mapSize);

	Point getCameraPos();

	void cameraEffect(const ELCameraEffect& cameraEffect);

	bool setBurst(const Point &burstpos);

	void cameraGraphicEffect();

	ELCameraGraphicEffect getGraphicEffect() const
	{return m_graphicEffect;}
};