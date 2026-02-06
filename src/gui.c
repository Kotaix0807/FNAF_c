#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "gui.h"
#include "nuklear_sdl_renderer.h"

#pragma GCC diagnostic pop

static struct nk_context *ctx = NULL;
static SDL_Renderer *sdl_renderer = NULL;
static SDL_Window *sdl_window = NULL;

bool GUI_Init(SDL_Window *win, SDL_Renderer *ren, const char *font_path, float font_size)
{
    sdl_window = win;
    sdl_renderer = ren;
    ctx = nk_sdl_init(win, ren);
    if (!ctx)
        return false;

    struct nk_font_atlas *atlas;
    struct nk_font *nk_font = NULL;
    nk_sdl_font_stash_begin(&atlas);
    if (font_path)
        nk_font = nk_font_atlas_add_from_file(atlas, font_path, font_size, 0);
    nk_sdl_font_stash_end();

    if (nk_font)
        nk_style_set_font(ctx, &nk_font->handle);

    return true;
}

int GUI_HandleEvent(SDL_Event *event)
{
    return nk_sdl_handle_event(event);
}

void GUI_InputBegin(void)
{
    nk_input_begin(ctx);
}

void GUI_InputEnd(void)
{
    nk_input_end(ctx);
}

void GUI_Render(void)
{
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void GUI_Destroy(void)
{
    nk_sdl_shutdown();
    ctx = NULL;
    sdl_renderer = NULL;
    sdl_window = NULL;
}

struct nk_context* GUI_GetContext(void)
{
    return ctx;
}