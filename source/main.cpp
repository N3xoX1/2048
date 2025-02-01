#include <switch.h>
#include <sys/stat.h>
#include "draw.hpp"
#include "game.hpp"

u8* g_framebuf;
Framebuffer g_framebuffer;
u32 g_framebufWidth, g_framebufHeight;
PadState g_pad;

bool fileExists(std::string path)
{
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0);
}

void Initgfx()
{
    NWindow* win = nwindowGetDefault();

    g_framebufHeight = 720;
    g_framebufWidth = 1280;
    framebufferCreate(&g_framebuffer, win, g_framebufWidth, g_framebufHeight, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&g_framebuffer);
}

void InitController()
{
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&g_pad);
}

int main()
{
    mkdir("sdmc:/switch", 777);
    mkdir("sdmc:/switch/2048", 777);
    Initgfx();
    InitController();

    Game::init();
    if (fileExists("sdmc:/switch/2048/state"))
    {
        Game::loadState();
    }

    while(appletMainLoop())
    {
        padUpdate(&g_pad);
        u64 kDown = padGetButtonsDown(&g_pad);
        if (kDown & HidNpadButton_Plus)
        {
            break;
        }

        Game::scanInput();
        Game::show();
    }

    Game::saveState();
    framebufferClose(&g_framebuffer);
    return 0;
}