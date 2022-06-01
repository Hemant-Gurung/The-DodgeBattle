#include "stdafx.h"
#include "Score.h"

Score::Score()
	:m_Text("LIVES"),
	m_Score("5")
{

}

void Score::Initialize(const SceneContext& /*scene_context*/)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
}

void Score::Update(const SceneContext& /*scene_context*/)
{
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text+": "+m_Score), m_TextPosition, m_TextColor);
}


void Score::SetText(std::string score)
{
	m_Score = score;
	if(m_Score<"0")
	{
		m_Score = "0";
	}
}

void Score::SetTextPosition(XMFLOAT2 position)
{
	m_TextPosition = position;
}

