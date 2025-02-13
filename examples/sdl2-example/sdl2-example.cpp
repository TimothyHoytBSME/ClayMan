// #define CLAY_IMPLEMENTATION
// #include "../../include/clay.h"
#include "../../clayman.hpp"
#include "../../include/sdl2/clay_renderer_SDL2.c"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>

// #include <stdio.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include <stdlib.h>

// SDL_Surface *sample_image;


//User layout callback function, do not call Clay_BeginLayout() or Clay_EndLayout()
void myLayout(ClayMan& clayMan){
    static std::string theText = "Click the button to change the text.";

    //A reusable configuration
    Clay_TextElementConfig textConfig = {
        .textColor = {255, 255, 255, 255},
        .fontId = 0,
        .fontSize = 20
    };

    //On-Click for Button element
    if(clayMan.pointerOver("Button") & clayMan.mousePressed()){
        theText = "Button has been clicked!";
    }

    //Here we are using openElement(). We need to call closeElement() after children block
    clayMan.openElement({
        .id = clayMan.hashID("OuterContainer"),
        .layout = {
            .sizing = clayMan.expandXY(),
            .padding = clayMan.padAll(16),
            .childGap = 16,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = {50, 50, 50, 255}
    });{ //Children of OuterContainer are in this block. The block is not necessary, but helps with structure.
        //Text Elements are self-contained
        clayMan.textElement("The outer container was made with openElement function and needs closed manually", textConfig);

        //Here we are using the lambda method, we do not need to call closeElement()
        clayMan.element(
            {
                .id = clayMan.hashID("Button"),
                .layout = {
                    .sizing = clayMan.fixedSize(100, 40),
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                },
                .backgroundColor = clayMan.pointerOver("Button")? //ternary in-line config property
                    Clay_Color{120, 120, 120, 255}
                    : Clay_Color{100, 100, 100, 255}
            },
            [&]{ //children of "Button" are in this lambda
                clayMan.textElement("Click Me", textConfig);
            }
        ); //do not call closeElement() for "Button"

        clayMan.textElement(theText, textConfig);
        
        //Here is another manual element
        clayMan.openElement( //we manually open the element
            {
                .id = clayMan.hashID("ManualElement"),
                .layout = {
                    .padding = clayMan.padXY(26, 16),
                    .childGap = 16,
                    .childAlignment = clayMan.centerXY()
                },
                .backgroundColor = {0,0,0,255}
            }
        );{//Children of "ManualElement" are in this block, the block is not necessary, but helps with structure
            clayMan.textElement("This parent element was made manually.", textConfig);

            //Another lambda element
            clayMan.element(
                {
                    .id = clayMan.hashID("LambdaElement"),
                    .layout = {.padding = clayMan.padAll(16)},
                    .backgroundColor = {120,120,120,255}
                },
                [&]{
                    clayMan.textElement("This element was made with the lambda method using element function.", textConfig);
                }
            );

            //A classic macro element
            CLAY({
                .id = CLAY_ID("ClayElement"),
                .layout = { 
                    .padding = CLAY_PADDING_ALL(16), 
                    .childAlignment = {
                        .x = CLAY_ALIGN_X_CENTER, 
                        .y = CLAY_ALIGN_Y_CENTER
                    }
                },
                .backgroundColor =  (Clay_Hovered()? (Clay_Color){200,200,200,255}: (Clay_Color){100,100,100,255}),
                .cornerRadius = 10
            }){
                CLAY_TEXT(CLAY_STRING("This element was made with the standard Clay macros inside a lambda element"), CLAY_TEXT_CONFIG(textConfig));
            }
        }
        clayMan.closeElement(); //we manually close the "ManualElement" since closeElement() was used

        uint16_t bw = 8;
        clayMan.element(
            {
                .id = clayMan.hashID("AnotherElement"),
                .layout = {
                    .sizing = clayMan.expandX(),
                    .padding = clayMan.padAll(16),
                    .childAlignment = clayMan.centerXY()
                },
                .backgroundColor = {0,0,0,255},//{0,0,0,125},
                .cornerRadius = {1000,1000,1000,1000},
                .border = {
                    .color = {123,123,0,255},
                    .width = {bw, bw, bw, bw,0}
                }
            },
            [&]{
                clayMan.element(
                    {
                        .layout = {
                            .padding = clayMan.padAll(16)
                        },
                        .backgroundColor = {255,255,255,255}//{255,255,255,123}
                    },
                    [&]{
                        clayMan.textElement("This element was made using the lambda method", textConfig);
                    }
                );
            }
        );
        
    } clayMan.closeElement(); //We close the outer container manually, since openElement() was used.
}


int main(int argc, char *argv[]) {
    bool running = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "Error: could not initialize TTF: %s\n", TTF_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        fprintf(stderr, "Error: could not initialize IMG: %s\n", IMG_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("resources/Roboto-Regular.ttf", 16);
    if (!font) {
        fprintf(stderr, "Error: could not load font: %s\n", TTF_GetError());
        return 1;
    }

    SDL2_Font fonts[1] = {};

    fonts[0] = (SDL2_Font) {
        .fontId = 0,
        .font = font,
    };

    // sample_image = IMG_Load("resources/sample.png");

    int windowWidth = 800;
    int windowHeight = 600;
    ClayMan clayMan = ClayMan(windowWidth, windowHeight, SDL2_MeasureText, &fonts);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(clayMan.getWindowWidth(), clayMan.getWindowHeight(), SDL_WINDOW_RESIZABLE, &window, &renderer) < 0) {
        fprintf(stderr, "Error: could not create window and renderer: %s", SDL_GetError());
    }

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    while (running) {
        Clay_Vector2 scrollDelta = {};
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: { running = false; }
                case SDL_MOUSEWHEEL: {
                    scrollDelta.x = event.wheel.x;
                    scrollDelta.y = event.wheel.y;
                    break;
                }
            }
        }
        if(!running){break;}
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        // printf("%f\n", deltaTime);

        int mouseX = 0;
        int mouseY = 0;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        clayMan.updateClayState(
            windowWidth,
            windowHeight,
            mouseX,
            mouseY,
            scrollDelta.x,
            scrollDelta.y,
            deltaTime,
            mouseState & SDL_BUTTON(1)
        );

        clayMan.beginLayout();
        myLayout(clayMan);
        Clay_RenderCommandArray renderCommands = clayMan.endLayout();
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Clay_SDL2_Render(renderer, renderCommands, fonts);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

