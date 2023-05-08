#pragma once
#include "BaseScene.h"

class Scene_GameArea:public BaseScene {

    private:
    struct Private_Data;
    Private_Data* sdata;

    public:
    Scene_GameArea();
    ~Scene_GameArea();
    // basic setup of new scene, call before scene create
    void HintString(int type, std::string hint) override;
    void HintFloat(int type, float hint)override;
    void HintInt(int type, int hint)override;

   // load and unload scene
    void OnCreate()override;
    void OnDestroy()override;

    void Start()override;
    void Stop()override;

   // game loop
    void Update()override;
    void Draw()override;
    void LateUpdate()override;
};