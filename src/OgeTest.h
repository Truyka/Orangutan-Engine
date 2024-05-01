#ifndef __INCLUDE_OGETEST__
#define __INCLUDE_OGETEST__

#include "OgeGame.h"
#include "Scene.h"
#include "OgeEntity.h"
#include "Config.h"

class TestGame : public oge::Game
{
public:
    TestGame();
};

class MainScene : public oge::Scene
{
public:
    OGE_SPECIFY_CLASS_NAME("MainState")
    MainScene();

    void onDraw(const float interp) override;
};


#endif /* __INCLUDE_OGETEST__ */
