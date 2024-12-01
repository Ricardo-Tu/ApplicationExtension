#pragma once
#include <Windows.h>
#include "cs1.6.h" 
#include "cs1.6Dlg.h"
#include "context.hpp"
#include "boxperspective.hpp"
#include <vector>

struct Postion {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    Postion(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};
extern std::vector<Postion> myPostion;

namespace game {
    bool InitAllControlsContext();
    bool InitEditControlContext();
    bool InitRadioControlContext();
    void SetMoney();
    bool SetMoneyTimer();
    bool CancelSetMoneyTimer();
    void SetBlood();
    bool SetBloodTimer();
    bool CancelSetBloodTimer();
    void SetArmor();
    bool SetArmorTimer();
    bool CancelSetArmorTimer();
    void SetBullet();
    bool SetBulletTimer();
    bool CancelSetBulletTimer();
    void SetInvincibility();
    void CancelInvincibility();
    void SetBuyEveryWhere();
    void CancelBuyEveryWhere();
    void SetMouseLeftButtonColdDown();
    void CancelMouseLeftButtonColdDown();
    void SetNoBackSeat();
    void CancelNoBackSeat();
    void SavePostion(Postion& position);
    void LoadPostion(Postion position);
    void SetSuperMan();
    void CancelSuperMan();
    void SetNightVision();
    void CancelNightVision();
}
