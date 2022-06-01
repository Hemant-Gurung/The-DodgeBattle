#include "stdafx.h"
#include "ControlsScreen.h"



void ControlsScreen::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("CONTROLS"), XMFLOAT2{ 500.f,300.f }, XMFLOAT4{ Colors::Red });

	auto Controls = new GameObject();
	auto spriteLogo = new SpriteComponent(L"Textures/controls.png");
	Controls->AddComponent(spriteLogo);
	AddChild(Controls);

	InputAction up = InputAction(int(UIinput::uibuttondown), InputState::pressed, -1, -1, XINPUT_GAMEPAD_B);
	GetSceneContext().pInput->AddInputAction(up);
}

void ControlsScreen::Update()
{
	GameScene::Update();
	if (m_SceneContext.pInput->IsActionTriggered(UIinput::uibuttondown))
	{
		SceneManager::Get()->SetActiveGameScene(L"StartScreen");
		//if()
	}
}
