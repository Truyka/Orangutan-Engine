#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Config.h"
#include "math/OgeMath.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <map>
#include <string>
#include <stdint.h>
#include <math.h>

namespace oge
{


struct TextureInfo
{
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

struct Color
{
    Color(Uint8 _r,Uint8 _g, Uint8 _b, Uint8 _a = SDL_ALPHA_OPAQUE)
        : r(_r), g(_g), b(_b), a(_a)
    {}

    SDL_Color toSDLColor()
    {
        return SDL_Color
        {
            .r = r,
            .g = g,
            .b = b,
            .a = a
        };
    }

    Uint8 r, g, b, a;

    static const Color red;
    static const Color orange;
    static const Color yellow;
    static const Color lime;
    static const Color green;
    static const Color cyan;
    static const Color teal;
    static const Color blue;
    static const Color purple;
    static const Color pink;
    static const Color black;
    static const Color white;
};

enum class TextQuality
{
    Normal,
    High
};

enum class Flip
{
    None = SDL_FLIP_NONE,
    Horizontal = SDL_FLIP_HORIZONTAL,
    Vertical = SDL_FLIP_VERTICAL
};

/**
 * @brief A singleton class wrapping SDL's functionality to 
 * create a window and render things to the screen. The constructor
 * is private and only one instance is available at a time by calling
 * Graphics::instance(). Before using the instance you need to intialize
 * it with Graphics::create(). The memory will be freed automatically
 * when the program ends or when you call Graphics::destroy()
*/
class Graphics
{
public:
    OGE_SPECIFY_CLASS_NAME("Graphics")
    OGE_DELETE_ASSIGN(Graphics)
    
    /** =SINGLETON INTERFACE= */
    
    /** @brief Creates a static instance of this class by calling
     * its constructor with given arguments */
    static void create(std::string title, int w, int h);

    /** @brief Accesses the static instance of Graphics */
    static Graphics& instance();

public:
    inline SDL_Renderer* getRenderer()
    {
        return renderer_;
    }

    ~Graphics();


    /* =RENDERING FUNCTIONS= */

    void renderClear(Color color = Color::black);

    void render(TextureInfo texture, 
                Rect renderDest, 
                Rect textureClip = Rect::none, 
                const float angle = 0.f, 
                Vector2f center = Vector2f::null, 
                Flip flip = Flip::None);
    
    void render(std::string path, Rect renderDest, Rect textureClip = Rect::none);

    void renderPresent();


    /* =DRAW FUNCTIONS= */

    void drawRect(Rect rect, Color color);
    void drawLine(Vector2f v0, Vector2f v1, Color color);
    void drawLine(Ray ray, Color color);


    /* =TEXTURE BANK MANAGEMENT= */

    bool setFont(std::string path, int size);

    /** @brief Converts an SDL_Surface to TextureInfo */
    TextureInfo textureFromSurface(SDL_Surface* surface);

    /** @brief Creates a texture from image at 'path' */
    TextureInfo createTexture(std::string path);

    void loadText(std::string id, std::string str, Color col, TextQuality q = TextQuality::Normal);
    void loadTextWrap(std::string id, std::string str, Color col, int wrap, TextQuality q = TextQuality::Normal);
    void loadTextShaded(std::string id, std::string str, Color col1, Color col2);
    void loadTextShadedWrap(std::string id, std::string str, Color col1, Color col2, int wrap);

    /** 
     * @brief Tries to get texture at path from the texture bank. If it
     * isn't in the bank, creates a new texture and saves it. 
    */
    TextureInfo loadOrGet(std::string path);

    /**
     * @brief Clears the texture bank and deallocates textures' memory 
     * @return size_t count of deallocated bank objects 
    */
    size_t clearTextureBank();

    /** 
     * @brief Gets a pointer to TextureInfo stored at path in the textureBank
     * @return TextureInfo* pointer to texture stored in textureBank or nullptr
     * if not found
    */
    TextureInfo* getFromBank(std::string path);
    bool removeFromBank(std::string path);

    Rect getWindowRect();

private:
    Graphics(std::string title, int w, int h);

    void createWindow(std::string title, int w, int h);
    void createRenderer();

private:
    static std::unique_ptr<Graphics> instance_;

    TTF_Font* font_;

    /** Is used to only load a texture once and reuse it */
    std::map<std::string, TextureInfo> textureBank_;

    SDL_Renderer* renderer_;
    SDL_Window* window_;
};


} // namespace oge

#endif // __GRAPHICS_H__