#pragma once
class ControlsScreen : public GameScene
{
	enum UIinput
	{
		uibuttonSelect,
		uibuttonup,
		uibuttondown
	};
public:
	ControlsScreen() :GameScene(L"ControlsScreen") {};
	~ControlsScreen() = default;
	ControlsScreen(const ControlsScreen& other) = delete;
	ControlsScreen(ControlsScreen&& other) noexcept = delete;
	ControlsScreen& operator=(const ControlsScreen& other) = delete;
	ControlsScreen& operator=(ControlsScreen&& other) noexcept = delete;	


	void Initialize() override;
	void Update() override;
private:
	SpriteComponent* m_pPlaySprite;
	SpriteFont* m_pFont;
};

