#include "stdafx.h"
#include "StartScreen.h"

void StartScreen::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("START"), XMFLOAT2{ 500.f,300.f }, XMFLOAT4{ Colors::Red });

	//TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("Graphics Programming 2/2022/Hemant Gurung - 2DAE06 "), XMFLOAT2{ 10.f,670.f }, XMFLOAT4{ Colors::White});

	auto Daelogo = new GameObject();
	auto spriteDae = new SpriteComponent(L"Textures/GP2_Exam2022_MainMenu_Template.png");
	Daelogo->AddComponent(spriteDae);
	AddChild(Daelogo);

	// play button
	auto playButton = new GameObject();
	m_pPlaySprite = new SpriteComponent(L"Textures/play.png");
	playButton->AddComponent(m_pPlaySprite);
	playButton->GetTransform()->Translate(500.f, 300.f, 0.f);
	AddChild(playButton);

	//m_SceneContext.pInput->AddInputAction(InputAction(UIinput::uibuttonSelect, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A));
	InputAction up = InputAction(int(UIinput::uibuttondown), InputState::pressed, -1, -1, XINPUT_GAMEPAD_X);
	GetSceneContext().pInput->AddInputAction(up);

	//START 
	InputAction start = InputAction(int(UIinput::uibuttonSelect), InputState::pressed, -1, -1, XINPUT_GAMEPAD_A);
	GetSceneContext().pInput->AddInputAction(start);
}

void StartScreen::Update()
{
	if(m_SceneContext.pInput->IsActionTriggered(UIinput::uibuttondown))
	{
		SceneManager::Get()->SetActiveGameScene(L"ControlsScreen");
		//if()
	}
	if (m_SceneContext.pInput->IsActionTriggered(UIinput::uibuttonSelect))
	{
		SceneManager::Get()->SetActiveGameScene(L"CharacterScene");
		//if()
	}
	
}

void StartScreen::OnGUI()
{
	GameScene::OnGUI();
}
