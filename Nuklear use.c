//Nuklear - Guia rápida
//Nuklear es una GUI de modo inmediato: no guardas estado de widgets, los defines cada frame y Nuklear se encarga del resto.

//Concepto básico

// Cada frame defines la UI completa. Si no la defines, no aparece.

struct nk_context *ctx = GUI_GetContext();

if (nk_begin(ctx, "Mi Ventana", nk_rect(x, y, w, h), flags))
{
    // widgets aquí
}
nk_end(ctx);  // SIEMPRE llamar, aunque nk_begin retorne 0

//Flags de ventana
Se combinan con |:

#define NK_WINDOW_BORDER      // borde visible
#define NK_WINDOW_MOVABLE     // arrastrable con el mouse
#define NK_WINDOW_SCALABLE    // redimensionable
#define NK_WINDOW_TITLE       // barra de título
#define NK_WINDOW_CLOSABLE    // botón X para cerrar
#define NK_WINDOW_MINIMIZABLE // botón para minimizar
#define NK_WINDOW_NO_SCROLLBAR // sin scrollbar

Cuando el usuario cierra con X, 
nk_begin() retorna 0 (el else en tu sprites.c).

Layout
Antes de cada fila de widgets, defines el layout:

// Una fila dinamica: N columnas de igual ancho, altura H
nk_layout_row_dynamic(ctx, 30, 1);   // 1 columna, 30px alto
nk_layout_row_dynamic(ctx, 30, 2);   // 2 columnas, 30px alto

// Fila estatica: ancho fijo en píxeles
nk_layout_row_static(ctx, 30, 120, 2);  // 2 columnas de 120px

// Fila con anchos personalizados por columna
float ratios[] = {0.3f, 0.7f};
nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratios);  // 30% y 70%


Widgets principales

struct nk_context *ctx = GUI_GetContext();
if (nk_begin(ctx, "Demo", nk_rect(100, 100, 350, 400),
    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE))
{
    // --- BOTÓN ---
    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_button_label(ctx, "Click me"))
    {
        // se ejecuta cuando se hace click
    }

    // --- CHECKBOX ---
    static nk_bool fullscreen = 0;
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_checkbox_label(ctx, "Fullscreen", &fullscreen);

    // --- SLIDER (int) ---
    static int fps = 60;
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_int(ctx, "FPS:", 30, &fps, 144, 1, 1);
    //                     label  min  val  max  step  inc_per_pixel

    // --- SLIDER (float) ---
    static float volume = 0.8f;
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_float(ctx, "Volume:", 0.0f, &volume, 1.0f, 0.01f, 0.01f);

    // --- SLIDER visual (sin texto editable) ---
    static float brightness = 0.5f;
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_slider_float(ctx, 0.0f, &brightness, 1.0f, 0.01f);

    // --- LABEL ---
    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "Texto simple", NK_TEXT_LEFT);
    // NK_TEXT_LEFT, NK_TEXT_CENTERED, NK_TEXT_RIGHT

    // --- TEXT INPUT ---
    static char buffer[64] = "";
    static int len = 0;
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 64, nk_filter_default);
    // filtros: nk_filter_default, nk_filter_decimal, nk_filter_float, nk_filter_ascii

    // --- COMBO / DROPDOWN ---
    static int selected = 0;
    const char *items[] = {"Opción A", "Opción B", "Opción C"};
    nk_layout_row_dynamic(ctx, 30, 1);
    selected = nk_combo(ctx, items, 3, selected, 25, nk_vec2(200, 150));

    // --- COLOR PICKER ---
    static struct nk_colorf color = {1, 0, 0, 1};
    nk_layout_row_dynamic(ctx, 120, 1);
    color = nk_color_picker(ctx, color, NK_RGBA);

    // --- SEPARADOR / ESPACIO ---
    nk_layout_row_dynamic(ctx, 10, 1);
    nk_spacing(ctx, 1);

    // --- GRUPO (subventana scrolleable) ---
    nk_layout_row_dynamic(ctx, 150, 1);
    if (nk_group_begin(ctx, "Lista", NK_WINDOW_BORDER)) {
        for (int i = 0; i < 20; i++) {
            nk_layout_row_dynamic(ctx, 20, 1);
            char buf[32];
            snprintf(buf, sizeof(buf), "Item %d", i);
            nk_label(ctx, buf, NK_TEXT_LEFT);
        }
        nk_group_end(ctx);
    }

    // --- TREE (secciones colapsables) ---
    if (nk_tree_push(ctx, NK_TREE_TAB, "Avanzado", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, "Contenido oculto", NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }

    // --- DOS COLUMNAS ---
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_label(ctx, "Nombre:", NK_TEXT_LEFT);
    nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 64, nk_filter_default);
}
nk_end(ctx);
Patron clave: modo inmediato
La diferencia fundamental con ncurses es que no hay estado persistente de widgets. Cada frame redibujas todo:


// ncurses (retenido): creas una vez, actualizas selectivamente
WINDOW *win = newwin(...);    // crear
mvwprintw(win, ...);          // dibujar una vez
wrefresh(win);                // mostrar
wgetch(win);                  // BLOQUEA esperando input

// Nuklear (inmediato): defines cada frame, nunca bloquea
void mi_panel(void) {
    struct nk_context *ctx = GUI_GetContext();
    if (nk_begin(ctx, "Panel", ...)) {
        // Esto se ejecuta 60 veces por segundo
        // Los widgets reaccionan al mouse/teclado automáticamente
        if (nk_button_label(ctx, "Acción"))
            hacer_algo();  // solo cuando se clickea
    }
    nk_end(ctx);
}
Donde definir tus paneles
Cualquier función llamada entre GUI_InputEnd() y GUI_Render() puede definir widgets. En tu engine, eso es dentro de Game_Render():


Game_KeyboardInput()   ← GUI_InputBegin / HandleEvent / InputEnd
Game_UpdateFrame()
Game_Render()          ← renderFrameDebug() define widgets aquí
                       ← GUI_Render() los dibuja
                       ← SDL_RenderPresent()
Para agregar un nuevo panel, simplemente crea una función y llamala desde Game_Render() antes de GUI_Render().

Ejemplo práctico: panel de configuración

void renderConfigPanel(void)
{
    struct nk_context *ctx = GUI_GetContext();
    if (nk_begin(ctx, "Config", nk_rect(400, 50, 300, 300),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
    {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_property_int(ctx, "FPS:", 30, &config.fps, 144, 1, 1);

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_property_int(ctx, "Volume:", 0, &config.master_volume, 100, 1, 1);

        nk_bool fs = config.fullscreen;
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_checkbox_label(ctx, "Fullscreen", &fs);
        if (fs != config.fullscreen) {
            config.fullscreen = fs;
            SDL_SetWindowFullscreen(window, fs ? SDL_WINDOW_FULLSCREEN : 0);
        }
    }
    nk_end(ctx);
}


//VENTANAS MULTIPLES

//En Nuklear, cada par nk_begin nk_end con un nombre distinto es una ventana independiente.
//Todas comparten el mismo ctx. Simplemente llamás a varias en el mismo frame:

// Ventana 1
if (nk_begin(ctx, "Frame Debug Setup", nk_rect(50, 50, 300, 220), flags)) {
    // widgets ventana 1...
}
nk_end(ctx);

// Ventana 2
if (nk_begin(ctx, "Config", nk_rect(400, 50, 300, 300), flags)) {
    // widgets ventana 2...
}
nk_end(ctx);

// Ventana 3
if (nk_begin(ctx, "Log", nk_rect(50, 300, 500, 200), flags)) {
    // widgets ventana 3...
}
nk_end(ctx);
Puntos clave:

El nombre (primer string de nk_begin) es el identificador único de cada ventana — no puede repetirse.
El nk_rect solo se usa la primera vez (posición/tamaño inicial).
Después Nuklear recuerda dónde el usuario la movió/redimensionó.
Cada ventana se cierra independientemente con nk_window_close(ctx, "nombre").
Podés poner cada ventana en su propia función para mantener el código organizado,
 y llamarlas todas desde renderFrameDebug() (o desde Game_Render() antes de GUI_Render()).