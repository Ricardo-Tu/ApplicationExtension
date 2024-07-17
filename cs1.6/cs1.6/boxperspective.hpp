#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>
#include <memory>
#include "tools.hpp"


namespace game {
    typedef struct WINDOWSINFO {
        uint32_t win_x_pos;
        uint32_t win_y_pos;
        uint32_t win_width;
        uint32_t win_height;
        uint32_t game_width;
        uint32_t game_height;
        WINDOWSINFO(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t game_w, uint32_t game_h) :win_x_pos(x), win_y_pos(y), win_width(w), win_height(h), game_width(game_w), game_height(game_h)
        {}
    }WindowsInfo, * pWindowsInfo;

    void GetWindowsInfo(wchar_t* windowsname, pWindowsInfo pwi);
    void boxperspective_thread();

}

extern uint32_t g_num;
extern game::WINDOWSINFO wi;

// team  T = 1  CT = 2
struct Player {
    float x;
    float y;
    float z;
    uint32_t blood;
    uint32_t team;
    float x_pos;
    float y_pos;
    float rectangle_width;
    float rectangle_height;
    Player(float x,
        float y,
        float z,
        uint32_t b,
        uint32_t team,
        float x_pos,
        float y_pos,
        float rectangle_width,
        float rectangle_height)
        :x(x), y(y), z(z), blood(b), team(team), x_pos(x_pos), y_pos(y_pos), rectangle_width(rectangle_width), rectangle_height(rectangle_height)
    {}
};

struct bone_Position {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float x_pos = 0.0f;
    float y_pos = 0.0f;
    bone_Position(float _x, float _y, float _z, float _x_pos, float _y_pos) : x(_x), y(_y), z(_z), x_pos(_x_pos), y_pos(_y_pos) {}
};

extern std::vector<Player> players;
extern float selfMatrix[4][4];
extern uint32_t boxShowFlag;
extern std::unique_ptr<std::vector<std::vector<bone_Position>>> pbone_positions;
