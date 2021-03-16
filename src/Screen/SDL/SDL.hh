#include "../Screen.hh"
#include "SDL_render.h"
#include "SDL_video.h"
#include <cstdint>
#include <vector>

class ScreenSDL : public Screen {
public:
    ScreenSDL();
    ~ScreenSDL();

    void Clear();
    bool isOpen() const;
    bool Poll();
    void Draw();
    void UpdatePixel(uint8_t x, uint8_t y, bool set);
private:
    void    SetBackgroundColor();
    void    SetForegroundColor();
    SDL_Window*     _window;
    SDL_Renderer*   _renderer;
    SDL_Texture*    _texture;
    bool            _open = false;
    std::vector<SDL_Rect> _rects;
};