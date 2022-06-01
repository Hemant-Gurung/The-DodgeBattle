#pragma once
//#include "C:\_GRAPHICS_PROG_2\_OVERLORD\OverlordEngine_x64\OverlordEngine\Scenegraph\GameScene.h"

class ComponentTestScene :public GameScene
{
public:
    ComponentTestScene() :GameScene(L"ComponentTestScent"){};
 /*   CLASSNAME();
    ~CLASSNAME();
    CLASSNAME()(const CLASSNAME& other) = delete;
    CLASSNAME()(CLASSNAME&& other) = delete;
    CLASSNAME()
    operator=(CLASSNAME& other) = delete;
    CLASSNAME()
    operator=(CLASSNAME&& other) = delete;*/

private:
    void Initialize() override;
};

