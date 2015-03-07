# include "ELMyCursor.h"


ELMyCursor::ELMyCursor()
{
	
}

bool ELMyCursor::init()
{
	TextureAsset::Register(L"ELcursor",L"data/Elis/Cursor/cursor.png");

	m_offSet = Point(TextureAsset(L"ELcursor").width/2,TextureAsset(L"ELcursor").height/2);

	return !!TextureAsset(L"ELcursor");
}

void ELMyCursor::update()
{
	m_cursorPos = Mouse::Pos();


}

void ELMyCursor::draw()
{
	TextureAsset(L"ELcursor").draw(m_cursorPos-m_offSet);
}

Point ELMyCursor::getCursorPos() const
{
	return m_cursorPos;
}