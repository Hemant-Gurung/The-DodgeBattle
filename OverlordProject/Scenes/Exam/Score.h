#pragma once
//#include "C:\_GRAPHICS_PROG_2\ExamProjcet\OverlordEngine_x64\OverlordEngine\Scenegraph\GameObject.h"
class Score :
    public GameObject
{
public:
	Score();


	~Score()
	{
		delete m_pFont;
		delete m_pFont2;
	}
	Score(const Score& other) = delete;
	Score(Score&& other) noexcept = delete;
	Score& operator=(const Score& other) = delete;
	Score& operator=(Score&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	//score position and value
	void SetTextPosition(XMFLOAT2);
	void SetText(std::string score);
private:

	SpriteFont* m_pFont{};
	SpriteFont* m_pFont2{};
	std::string m_Text{};
	std::string m_Score;
	XMFLOAT2 m_TextPosition{};
	XMFLOAT4 m_TextColor{ 0.f,0.f,0.f,1.f };


	
};



