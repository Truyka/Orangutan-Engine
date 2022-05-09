#ifndef __OGETEST_H__
#define __OGETEST_H__

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


#endif // __OGETEST_H__