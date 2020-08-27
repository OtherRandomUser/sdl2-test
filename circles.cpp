#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stack>
#include <sstream>
#include <unistd.h>

#include <random>
#include <vector>

using namespace std;

// Estrutura para representar pontos
struct Point {
    int x, y;
};

// Variáveis necessárias para o SDL
unsigned int * pixels;
int width, height;
SDL_Surface * window_surface;
SDL_Renderer * renderer;

// Título da janela
std::string titulo = "SDL Points";

// Valores RGB para a cor de funco da janela
const int VERMELHO = 0;
const int VERDE = 0;
const int AZUL = 0;

// Gera uma estrutura Point a partir de valores para x e y
Point getPoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// Obtém a cor de um pixel de uma determinada posição
Uint32 getPixel(int x, int y)
{
    if((x>=0 && x<=width) && (y>=0 && y<=height))
        return pixels[x + width * y];
    else
        return -1;
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g, b, e alpha (transparência)
// r, g, b e a variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b, int a)
{
    pixels[x + y * width] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g e b
// r, g, e b variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b)
{
    setPixel(x, y, r, g, b, 255);
}

// Mostra na barra de título da janela a posição
// corrente do mouse
void showMousePosition(SDL_Window * window, int x, int y)
{
    std::stringstream ss;
    ss << titulo << " X: " << x << " Y: " << y;
    SDL_SetWindowTitle(window, ss.str().c_str());
}

// Imprime na console a posição corrente do mouse
void printMousePosition(int x, int y)
{
    printf("Mouse on x = %d, y = %d\n",x,y);
}

// Seta um pixel em uma determinada posição,
// através de um Uint32 representando
// uma cor RGB
void setPixel(int x, int y, Uint32 color)
{
    if((x<0 || x>=width || y<0 || y>=height)) {
        printf("Coordenada inválida : (%d,%d)\n",x,y);
        return;
    }
    pixels[x + y * width] = color;
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, b e a(transparência)
// informadas. r, g, b e a variam de 0 até 255
Uint32 RGB(int r, int g, int b, int a) {
    return SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, e b
// informadas. r, g e b variam de 0 até 255
// a transparência é sempre 255 (imagem opaca)
Uint32 RGB(int r, int g, int b) {
    return SDL_MapRGBA(window_surface->format, r, g, b, 255);
}

// Retorna um componente de cor de uma cor RGB informada
// aceita os parâmetros 'r', 'R','g', 'G','b' e 'B',
Uint8 getColorComponent( Uint32 pixel, char component ) {

    Uint32 mask;

    switch(component) {
        case 'b' :
        case 'B' :
            mask = RGB(0,0,255);
            pixel = pixel & mask;
            break;
        case 'r' :
        case 'R' :
            mask = RGB(255,0,0);
            pixel = pixel & mask;
            pixel = pixel >> 16;
            break;
        case 'g' :
        case 'G' :
            mask = RGB(0,255,0);
            pixel = pixel & mask;
            pixel = pixel >> 8;
            break;
    }
    return (Uint8) pixel;
}

/***********************************************************/

void bresenham_low(Point origin, Point dest, Uint32 color)
{
    int slope;

    if (origin.x > dest.x)
    {
        std::swap(origin, dest);
    }

    auto dx = dest.x - origin.x;
    auto dy = dest.y - origin.y;

    if (dy < 0)
    {
        slope = -1;
        dy = -dy;
    }
    else
    {
        slope = 1;
    }
    
    auto incE = 2 * dy;
    auto incNE = incE - 2 * dx;
    auto d = 2 * dy - dx;
    auto y = origin.y;

    for (int x = origin.x; x < dest.x; x++)
    {
        setPixel(x, y, color);

        if (d <= 0)
        {
            d += incE;
        }
        else
        {
            d += incNE;
            y += slope;
        }
    }
}

void bresenham_high(Point origin, Point dest, Uint32 color)
{
    int slope;

    if (origin.y > dest.y)
    {
        std::swap(origin, dest);
    }

    auto dx = dest.x - origin.x;
    auto dy = dest.y - origin.y;

    if (dx < 0)
    {
        slope = -1;
        dx = -dx;
    }
    else
    {
        slope = 1;
    }
    
    auto incE = 2 * dx;
    auto incNE = incE - 2 * dy;
    auto d = 2 * dx - dy;
    auto x = origin.x;

    for (int y = origin.y; y < dest.y; y++)
    {
        setPixel(x, y, color);

        if (d <= 0)
        {
            d += incE;
        }
        else
        {
            d += incNE;
            x += slope;
        }
    }
}

void bresenham(Point origin, Point dest, Uint32 color)
{
    if (std::abs(origin.y - dest.y) < std::abs(origin.x - dest.x))
        bresenham_low(origin, dest, color);
    else
        bresenham_high(origin, dest, color);
}

void display_bresenham_circle(Point origin, Point point, Uint32 color)
{
    setPixel(origin.x + point.x, origin.y + point.y, color);
    setPixel(origin.x - point.x, origin.y + point.y, color);
    setPixel(origin.x + point.x, origin.y - point.y, color);
    setPixel(origin.x - point.x, origin.y - point.y, color);

    setPixel(origin.x + point.y, origin.y + point.x, color);
    setPixel(origin.x - point.y, origin.y + point.x, color);
    setPixel(origin.x + point.y, origin.y - point.x, color);
    setPixel(origin.x - point.y, origin.y - point.x, color);
}

void bresenham_circle(Point origin, int radius, Uint32 color)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    display_bresenham_circle(origin, { .x = x, .y = y}, color);

    while (y > x)
    {
        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        
        display_bresenham_circle(origin, { .x = x, .y = y}, color);
    }
}

// Inicializa o SDL, abre a janela e controla o loop
// principal do controle de eventos
int main()
{
    // Inicializações iniciais obrigatórias

    setlocale(LC_ALL, NULL);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow(titulo.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_RESIZABLE);

    window_surface = SDL_GetWindowSurface(window);

    pixels = (unsigned int *) window_surface->pixels;
    width = window_surface->w;
    height = window_surface->h;

    // Fim das inicializações

    printf("Pixel format: %s\n",
        SDL_GetPixelFormatName(window_surface->format->format));

     while (1)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(window);
                    pixels = (unsigned int *) window_surface->pixels;
                    width = window_surface->w;
                    height = window_surface->h;
                    printf("Size changed: %d, %d\n", width, height);
                }
            }

            // Se o mouse é movimentado
            if(event.type == SDL_MOUSEMOTION)
            {
                // Mostra as posições x e y do mouse
                showMousePosition(window,event.motion.x,event.motion.y);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
				/*Se o botão esquerdo do mouse é pressionado */
                if(event.button.button == SDL_BUTTON_LEFT)
				{
					printf("Mouse pressed on (%d,%d)\n",event.motion.x,event.motion.y) ;
				}
            }
        }

        // Seta a cor de fundo da janela para a informada nas
        // constantes VERMELHO, VERDE e AZUL
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                setPixel(x, y, RGB(VERMELHO,VERDE,AZUL));
            }
        }

        /*********************/

        bresenham_circle({ 300, 300 }, 20, RGB(255, 0, 0));
        bresenham_circle({ 300, 300 }, 200, RGB(0, 255, 0));
        bresenham_circle({ 0, 0 }, 200, RGB(0, 0, 255));

        /*********************/

        SDL_UpdateWindowSurface(window);
    }
}
 