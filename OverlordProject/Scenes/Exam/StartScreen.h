#pragma once
class StartScreen: public GameScene
{
	enum UIinput
	{
		uibuttonSelect,
		uibuttonup,
		uibuttondown
	};

public:
	StartScreen():GameScene(L"StartScreen"){}
	~StartScreen() override = default;
	StartScreen(const StartScreen& other) = delete;
	StartScreen(StartScreen&& other) noexcept = delete;
	StartScreen& operator=(const StartScreen& other) = delete;
	StartScreen& operator=(StartScreen&& other) noexcept = delete;	

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

	
private:
	SpriteFont* m_pFont;
	SpriteComponent* m_pPlaySprite;
	int m_ActiveButton = 0;

};

