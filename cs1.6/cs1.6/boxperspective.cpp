#include "pch.h"
#include "boxperspective.hpp"
#include "cs1.6Dlg.h"

uint32_t g_num = 0;
game::WINDOWSINFO wi = game::WINDOWSINFO(0, 0, 0, 0, 0, 0);
std::vector<Player> players;
float selfMatrix[4][4];
std::unique_ptr<std::vector<std::vector<bone_Position>>> pbone_positions = std::make_unique<std::vector<std::vector<bone_Position>>>();


namespace game {

    void GetWindowsInfo(wchar_t* windowsname, pWindowsInfo pwi)
    {
        HWND hWnd = FindWindowW(NULL, windowsname);
        if (hWnd == NULL) {
            DebugPrint(__FUNCTIONW__, L"Target window not found!\n");
            return;
        }

        LONG_PTR style = GetWindowLongPtrW(hWnd, GWL_STYLE);
        if (!(style & WS_BORDER)) {
            //std::cout << "Window has border." << std::endl;
            pwi->win_x_pos = 0;
            pwi->win_y_pos = 0;
            pwi->win_width = GetSystemMetrics(SM_CXSCREEN);
            pwi->win_height = GetSystemMetrics(SM_CYSCREEN);
        }
        else {
            // std::cout << "Window does not have border." << std::endl;

            POINT clientPoint = { 0, 0 };
            ClientToScreen(hWnd, &clientPoint);

            RECT clientRect;
            if (GetClientRect(hWnd, &clientRect)) {
                pwi->win_x_pos = clientPoint.x;
                pwi->win_y_pos = clientPoint.y;
                pwi->win_width = clientRect.right;
                pwi->win_height = clientRect.bottom;
            }
            else {
                DebugPrint(__FUNCTIONW__, L"Failed to get window rect!\n");
            }
        }
    }

    void readSelfMatrix()
    {
        std::vector<uint32_t> selfMatrixOffset = { 0x01820100 };
        uint32_t base = selfMatrixOffset[0];
        for (int i = 0; i < 4; i++)
        {
            for (uint32_t j = 0; j < 4; j++)
            {
                selfMatrixOffset[0] = base + ((i * 0x4) + j) * 0x4;
                g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&selfMatrix[i][j], selfMatrixOffset);
            }
        }
    }

    void readBonePosition(uint32_t player_number)
    {
        float x, y, z;
        std::vector<uint32_t> boneOffset = { 0xF06BB20 };
        pbone_positions->resize(player_number);
        for (uint32_t i = 0; i < player_number; i++)
        {
            for (uint32_t j = 0; j < 30; j++)
            {
                g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&x, boneOffset);
                boneOffset[1] += 4;
                g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&y, boneOffset);
                boneOffset[1] += 4;
                g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&z, boneOffset);
                (*pbone_positions)[i].emplace_back(x, y, z, 0.0f, 0.0f);
                boneOffset[1] += 0x18;
            }
        }

        for (uint32_t i = 0; i < pbone_positions->size(); i++)
        {
            for (uint32_t j = 0; j < (*pbone_positions)[i].size(); j++)
            {
                float ndc[3] = { 0 };
                float clipCoords[4] = { 0 };
                float clipCoords_yh = 0, ndc_yh = 0, yh_pos = 0;
                float clipCoords_yl = 0, ndc_yl = 0, yl_pos = 0;
                wi.game_width = 1920;
                wi.game_height = 1080;
                clipCoords[0] = (*pbone_positions)[i][j].x * selfMatrix[0][0] + (*pbone_positions)[i][j].y * selfMatrix[1][0] + (*pbone_positions)[i][j].z * selfMatrix[2][0] + selfMatrix[3][0];
                clipCoords[1] = (*pbone_positions)[i][j].x * selfMatrix[0][1] + (*pbone_positions)[i][j].y * selfMatrix[1][1] + (*pbone_positions)[i][j].z * selfMatrix[2][1] + selfMatrix[3][1];
                clipCoords[2] = (*pbone_positions)[i][j].x * selfMatrix[0][2] + (*pbone_positions)[i][j].y * selfMatrix[1][2] + (*pbone_positions)[i][j].z * selfMatrix[2][2] + selfMatrix[3][2];
                clipCoords[3] = (*pbone_positions)[i][j].x * selfMatrix[0][3] + (*pbone_positions)[i][j].y * selfMatrix[1][3] + (*pbone_positions)[i][j].z * selfMatrix[2][3] + selfMatrix[3][3];
                ndc[0] = clipCoords[0] / clipCoords[3];
                ndc[1] = clipCoords[1] / clipCoords[3];
                ndc[2] = clipCoords[2] / clipCoords[3];
                if (clipCoords[3] < 0.1f)
                    continue;
                ndc[0] = clipCoords[0] / clipCoords[3];
                ndc[1] = clipCoords[1] / clipCoords[3];
                ndc[2] = clipCoords[2] / clipCoords[3];
                (*pbone_positions)[i][j].x_pos = (wi.win_width / 2 * ndc[0]) + (ndc[0] + wi.win_width / 2);
                (*pbone_positions)[i][j].y_pos = -(wi.win_height / 2 * ndc[1]) + (ndc[1] + wi.win_height / 2);
            }
        }
    }
    void drawBone(SDL_Renderer* render)
    {
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        for (uint32_t i = 0; i < pbone_positions->size(); i++)
        {
            for (uint32_t j = 1; j < (*pbone_positions)[i].size(); j++)
            {
                SDL_RenderDrawLine(render, (int)(*pbone_positions)[i][j].x_pos, (int)(*pbone_positions)[i][j].y_pos, (int)(*pbone_positions)[i][j - 1].x_pos, (int)(*pbone_positions)[i][j - 1].y_pos);
            }
        }
    }

    void readPlayerCoordination()
    {
        float x_temp = 0, y_temp = 0, z_temp = 0;
        float blood = 0;
        uint32_t team = 0;
        uint32_t bot_number = 0;
        float liveOrDie = 0; // 
        std::vector<uint32_t> bot_numberOffset = { 0x005F73AC, 0xD0, 0x70, 0x110, 0x9C };
        g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory<uint32_t>(&bot_number, bot_numberOffset);
        std::vector<uint32_t> coordinationOffset = { 0x11069BC , 0x7c, 0x4, 0x8 };
        std::vector<uint32_t> teamOffset = { 0x11069BC, 0x7c, 0x1c8 };
        std::vector<uint32_t> liveOrDieOffset = { 0x11069BC, 0x7c, 0x4, 0x16c };
        std::vector<uint32_t> bloodOffset = { 0x11069BC, 0x7c, 0x4, 0x160 };
        players.clear();
        bot_number = 9;
        for (uint32_t i = 0; i < bot_number + 1; i++)
        {
            bloodOffset[3] = 0x160 + i * 0x324;
            teamOffset[1] = 0x7c + i * 0x324;
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&blood, bloodOffset);
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory<uint32_t>(&team, teamOffset);
            liveOrDieOffset[3] = 0x16c + i * 0x324;
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&liveOrDie, liveOrDieOffset);
            coordinationOffset[3] = 0x8 + i * 0x324;
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&x_temp, coordinationOffset);
            coordinationOffset[3] += 0x4;
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&y_temp, coordinationOffset);
            coordinationOffset[3] += 0x4;
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory(&z_temp, coordinationOffset);
            if ((uint32_t)liveOrDie > 1 || i == 0)
                players.push_back(Player(x_temp, y_temp, z_temp, (uint32_t)blood, team, 0, 0, 0, 0));
        }

        for (uint32_t i = 1; i < players.size(); i++)
        {

            float ndc[3] = { 0 };
            float clipCoords[4] = { 0 };
            float clipCoords_yh = 0, ndc_yh = 0, yh_pos = 0;
            float clipCoords_yl = 0, ndc_yl = 0, yl_pos = 0;
            wi.game_width = 1920;
            wi.game_height = 1080;
            clipCoords[0] = players[i].x * selfMatrix[0][0] + players[i].y * selfMatrix[1][0] + players[i].z * selfMatrix[2][0] + selfMatrix[3][0];
            clipCoords[1] = players[i].x * selfMatrix[0][1] + players[i].y * selfMatrix[1][1] + players[i].z * selfMatrix[2][1] + selfMatrix[3][1];
            clipCoords[2] = players[i].x * selfMatrix[0][2] + players[i].y * selfMatrix[1][2] + players[i].z * selfMatrix[2][2] + selfMatrix[3][2];
            clipCoords[3] = players[i].x * selfMatrix[0][3] + players[i].y * selfMatrix[1][3] + players[i].z * selfMatrix[2][3] + selfMatrix[3][3];
            clipCoords_yh = players[i].x * selfMatrix[0][1] + players[i].y * selfMatrix[1][1] + (players[i].z + 25) * selfMatrix[2][1] + selfMatrix[3][1];
            clipCoords_yl = players[i].x * selfMatrix[0][1] + players[i].y * selfMatrix[1][1] + (players[i].z - 35) * selfMatrix[2][1] + selfMatrix[3][1];
            ndc[0] = clipCoords[0] / clipCoords[3];
            ndc[1] = clipCoords[1] / clipCoords[3];
            ndc[2] = clipCoords[2] / clipCoords[3];
            ndc_yh = clipCoords_yh / clipCoords[3];
            ndc_yl = clipCoords_yl / clipCoords[3];
            if (clipCoords[3] < 0.1f)
                continue;
            players[i].x_pos = (wi.win_width / 2 * ndc[0]) + (ndc[0] + wi.win_width / 2);
            players[i].y_pos = -(wi.win_height / 2 * ((ndc_yh + ndc_yl) / 2)) + (((ndc_yh + ndc_yl) / 2) + wi.win_height / 2);
            yh_pos = -(wi.win_height / 2 * ndc_yh) + (ndc_yh + wi.win_height / 2);
            yl_pos = -(wi.win_height / 2 * ndc_yl) + (ndc_yl + wi.win_height / 2);
            players[i].rectangle_height = yl_pos - yh_pos;
            players[i].rectangle_width = players[i].rectangle_height * 0.5f;
        }


    }

    void drawWindowBox(SDL_Renderer* render)
    {
        SDL_Rect rect = { 0, 0, (int)wi.win_width, (int)wi.win_height };
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderDrawRect(render, &rect);
    }

    void drawBox(SDL_Renderer* render, TTF_Font* font)
    {
        char bloodStr[5];
        SDL_Color color = { 0 };
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        drawWindowBox(render);
        if (players.size() == 0)
            return;
        for (int i = 1; i < players.size(); i++)
        {
            auto player = players[i];
            if (player.team == players[0].team)
            {
                color = { 0, 255, 0, 255 };
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
            }
            else if (player.team != players[0].team)
            {
                color = { 255, 0, 0, 255 };
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
            }
            if (boxShowFlag == 0 || player.team != players[0].team)
            {
                SDL_Rect rect = { (int32_t)(player.x_pos - player.rectangle_width / 2),
                              (int32_t)(player.y_pos - player.rectangle_height / 2),
                              (int32_t)player.rectangle_width,
                              (int32_t)player.rectangle_height };
                SDL_RenderDrawRect(render, &rect);
                memset(bloodStr, 0x00, 5);
                sprintf_s(bloodStr, "%d", player.blood);
                SDL_Surface* surface = TTF_RenderText_Blended(font, bloodStr, color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
                SDL_FreeSurface(surface);
                SDL_Rect rectFont = rect;
                rectFont.w = 20;
                rectFont.h = 18;
                rectFont.y -= rectFont.w;
                SDL_RenderCopy(render, texture, NULL, &rectFont);

            }
        }
        if (bone_flag == 1)
        {
            readBonePosition((uint32_t)players.size());
            drawBone(render);
        }
        SDL_RenderPresent(render);
    }

    void boxperspective_thread()
    {

        // 初始化 SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::stringstream ss;
            ss << "SDL_Init Error: " << SDL_GetError() << std::endl;
            OutputDebugStringA(ss.str().c_str());
            return;
        }
        if (TTF_Init() == -1) {
            std::stringstream ss;
            ss << "TTF_Init Error: " << TTF_GetError() << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }

        GetWindowsInfo(L"Counter-Strike", &wi);

        // 创建一个窗口
        SDL_Window* window = SDL_CreateWindow("Transparent Window",
            wi.win_x_pos,
            wi.win_y_pos,
            wi.win_width, wi.win_height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
        if (window == NULL) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }

        // 获取窗口的 Win32 句柄
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(window, &wmInfo);
        HWND hwnd = wmInfo.info.win.window;

        // 设置窗口样式，使其支持透明度
        LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

        // 设置窗口透明度为完全透明
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

        // 创建一个渲染器
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL) {
            SDL_DestroyWindow(window);
            printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }

        SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

        TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\msyh.ttc", 24);
        if (font == NULL) {
            std::stringstream ss;
            ss << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            OutputDebugStringA(ss.str().c_str());
        }

        // 设置渲染器透明度
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Event e;
        int quit = 0;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = 1;
                }
            }
            readSelfMatrix();
            readPlayerCoordination();
            drawBox(renderer, font);

        }

        // 清理
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return;
    }
}
