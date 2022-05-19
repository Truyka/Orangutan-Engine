#include "Graphics.h"
#include "components/Sprite.h"

namespace oge
{


std::unique_ptr<Graphics> Graphics::instance_;

const Color Color::red(255, 0, 0);
const Color Color::orange(255, 128, 0);
const Color Color::yellow(255, 255, 0);
const Color Color::lime(128, 255, 0);
const Color Color::green(0, 255, 0);
const Color Color::cyan(0, 255, 255);
const Color Color::teal(0, 128, 128);
const Color Color::blue(0, 0, 255);
const Color Color::purple(128, 0, 255);
const Color Color::pink(255, 0, 255);
const Color Color::black(0, 0, 0);
const Color Color::white(255, 255, 255);

Graphics::Graphics(std::string title, int w, int h) : font_(nullptr)
{
    OGE_LOG_INFO("Initializing game window and renderer");
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        OGE_LOG_ERROR("Couldn't intialize SDL (SDL_Init) in Graphics ctor");
        OGE_LOG_ERROR( SDL_GetError() );
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        OGE_LOG_ERROR("Couldn't intialize SDL_Image (IMG_Init) in Graphics ctor");
        OGE_LOG_ERROR( IMG_GetError() );
    }

    if(TTF_Init() == -1)
    {
        OGE_LOG_ERROR("Couldn't intialize SDL_TTF (IMG_Init) in Graphics ctor");
        OGE_LOG_ERROR( TTF_GetError() );
    }

    createWindow(title, w, h);
    if(window_ == nullptr)
    {
        OGE_LOG_ERROR("Couldn't create a window (SDL_CreateWindow) in Graphics ctor");
        OGE_LOG_ERROR( SDL_GetError() );
    }

    createRenderer();
    if(renderer_ == nullptr)
    {
        OGE_LOG_ERROR("Couldn't create a window (SDL_CreateWindow) in Graphics ctor");
        OGE_LOG_ERROR( SDL_GetError() );
    }
}

Graphics::~Graphics()
{
    if(font_ != nullptr)
    {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }

    size_t count = clearTextureBank();
    OGE_LOG_INFO("Cleared textureBank. Cleared textures count: " << count);

    OGE_LOG_INFO("Quitting the SDL_Image subsystem (IMG_Quit)");
    IMG_Quit();

    OGE_LOG_INFO("Quitting SDL_TTF subsystem (TTF_Quit)");
    TTF_Quit();

    OGE_LOG_INFO("Quitting SDL (SDL_Quit)");
    SDL_Quit();
}

void Graphics::create(std::string title, int w, int h)
{
    if(!instance_)
    {
        instance_ = std::unique_ptr<Graphics>(new Graphics(title, w, h));
    }
    else 
    {
        OGE_LOG_WARNING("Graphics::create() called when an instance is already initialized");
    }
}

Graphics& Graphics::instance()
{
    return *instance_;
}

void Graphics::renderClear(Color color) 
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
}

void Graphics::render(TextureInfo texture, Rect renderDest, Rect textureClip, 
                      const float angle, Vector2f center, Flip flip, Color col)
{   
    SDL_Rect dest = renderDest.toSDLRect();
    SDL_Rect clip = textureClip.toSDLRect();
    SDL_Point point{int(std::round(center.x)), int(std::round(center.y))};

    SDL_Rect* clipPtr = textureClip.isValid() ? &clip : nullptr;
    SDL_Point* pointPtr = (center == Vector2f::null) ? nullptr : &point;

    SDL_RendererFlip flipSDL = SDL_RendererFlip(flip);

    SDL_SetTextureColorMod(texture.texture, col.r, col.g, col.b);
    SDL_RenderCopyEx(renderer_, texture.texture, clipPtr, &dest, angle, pointPtr, flipSDL);
}

void Graphics::render(std::string path, Rect renderDest, Rect textureClip)
{
    TextureInfo texture = loadOrGet(path);

    render(texture, renderDest, textureClip);
}

void Graphics::renderPresent() 
{
    SDL_RenderPresent(renderer_);
}

void Graphics::drawRect(Rect rect, Color color)
{
    SDL_Rect draw = rect.toSDLRect();
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer_, &draw);
}

void Graphics::drawLine(Vector2f v0, Vector2f v1, Color color)
{
    v0.round(); v1.round();
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer_, v0.x, v0.y, v1.x, v1.y);
}

void Graphics::drawLine(Ray ray, Color color)
{
    drawLine(ray.origin, ray.origin + ray.dir, color);
}

bool Graphics::setFont(std::string path, int size)
{
    if(font_ != nullptr)
    {
        TTF_CloseFont(font_);
    }
    font_ = TTF_OpenFont(path.c_str(), size);
    return font_ != nullptr;
}

TextureInfo Graphics::textureFromSurface(SDL_Surface* surface)
{
    if(surface == nullptr)
    {
        OGE_LOG_ERROR(IMG_GetError());
        return TextureInfo();
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    if(texture == nullptr)
    {
        OGE_LOG_ERROR(SDL_GetError());
        return TextureInfo();
    }

    return TextureInfo{texture, surface->w, surface->h};
}

TextureInfo Graphics::createTexture(std::string path)
{
    OGE_LOG_INFO("Loading texture at " << path);

    SDL_Surface* surface = IMG_Load(path.c_str());
    TextureInfo ret = textureFromSurface(surface);

    SDL_FreeSurface(surface);
    return ret;
}

void Graphics::loadText(std::string id, std::string str, Color col, TextQuality q)
{
    removeFromBank(id);

    SDL_Surface* surface;
    switch(q)
    {
    case TextQuality::Normal: 
        surface = TTF_RenderText_Solid(font_, str.c_str(), col.toSDLColor()); 
        break;
    case TextQuality::High:
        surface = TTF_RenderText_Blended(font_, str.c_str(), col.toSDLColor()); 
        break;
    }
         
    textureBank_[id] = textureFromSurface(surface);

    SDL_FreeSurface(surface);
}

void Graphics::loadTextWrap(std::string id, std::string str, Color col, int wrap, TextQuality q)
{
    removeFromBank(id);

    SDL_Surface* surface;
    switch(q)
    {
    case TextQuality::Normal: 
        surface = TTF_RenderText_Solid_Wrapped(font_, str.c_str(), col.toSDLColor(), wrap); 
        break;
    case TextQuality::High:
        surface = TTF_RenderText_Blended_Wrapped(font_, str.c_str(), col.toSDLColor(), wrap); 
        break;
    }
         
    textureBank_[id] = textureFromSurface(surface);

    SDL_FreeSurface(surface);
}

void Graphics::loadTextShaded(std::string id, std::string str, Color col1, Color col2)
{
    removeFromBank(id);

    SDL_Surface* surface = TTF_RenderText_Shaded(font_, str.c_str(), col1.toSDLColor(), col2.toSDLColor());
    textureBank_[id] = textureFromSurface(surface);

    SDL_FreeSurface(surface);
}

void Graphics::loadTextShadedWrap(std::string id, std::string str, Color col1, Color col2, int wrap)
{
    removeFromBank(id);

    SDL_Surface* surface = TTF_RenderText_Shaded_Wrapped(font_, str.c_str(), col1.toSDLColor(), col2.toSDLColor(), wrap);
    textureBank_[id] = textureFromSurface(surface);

    SDL_FreeSurface(surface);
}

TextureInfo Graphics::loadOrGet(std::string path)
{
    auto it = textureBank_.find(path);
    if(it == textureBank_.end())
    {
        TextureInfo temp = createTexture(path);
        textureBank_[path] = temp;
        return temp;
    }
    return it->second;
}

size_t Graphics::clearTextureBank()
{
    size_t count = 0;
    for(auto& pair : textureBank_)
    {
        if(pair.second.texture != nullptr)
        {
            SDL_DestroyTexture(pair.second.texture);
            pair.second.texture = nullptr;
            count++;
        }
    }
    textureBank_.clear();
    return count;
}

TextureInfo* Graphics::getFromBank(std::string path)
{
    auto it = textureBank_.find(path);
    return it == textureBank_.end() ? nullptr : &it->second;
}

bool Graphics::removeFromBank(std::string path)
{
    auto it = textureBank_.find(path);
    if(it != textureBank_.end())
    {
        if(it->second.texture != nullptr)
        {
            SDL_DestroyTexture(it->second.texture);
            it->second.texture = nullptr;
        }
        return true;
    }
    return false;
}

Rect Graphics::getWindowRect()
{
    int x, y, w, h;
    SDL_GetWindowPosition(window_, &x, &y);
    SDL_GetWindowSize(window_, &w, &h); // consider SDL_GetRendererOutputSize() 
    return Rect(x, y, w, h);
}

void Graphics::createWindow(std::string title, int w, int h)
{
    window_ = SDL_CreateWindow(
        title.c_str(), 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w, // window width
        h, // window height
        0  // flags
    );
}

void Graphics::createRenderer()
{
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}


} // namespace oge