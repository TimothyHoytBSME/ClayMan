# ClayMan

### [ClayMan](https://github.com/TimothyHoytBSME/ClayMan) is a C++ wrapper library for the `clay.h` library found [here](https://github.com/nicbarker/clay/). This library (as well as Clay), is a work in progress, but is ready to be used. There is an active cummunity on [Discord](https://discord.com/channels/1291501630973415474/1291501631510417470), any direct discussion about ClayMan can be found in the [#bindings-cpp channel](https://discord.com/channels/1291501630973415474/1338700149001818145).

- [About Clay](#about-clay)
- [About ClayMan](#about-clayman)
- [Closing Elements](#closing-elements)
- [Strings](#strings)
- [How To Use ClayMan (With Raylib Renderer)](#how-to-use-clayman-with-raylib-renderer)
- [Examples And Other Renderers](#examples-and-other-renderers)
- [Events](#events)
- [Element Configuration](#element-configuration)
- [Advanced Use](#advanced-use)
- [API](#api)
- [Final Notes](#final-notes)
- [Change Log](#change-log)

## About Clay
If you are not already familiar with `Clay`, a high-performance UI layout library in C, [go check it out](https://github.com/nicbarker/clay/)!
The Clay library is a masterclass in high-performance low-level layout management that handles your UI calculations in real time in immediate mode with blazing speeds.
The original Clay API is based around a macro system, where you call the macro structure each frame, like this:

```C
Clay_BeginLayout();

// Parent element with 8px of padding
CLAY({.layout = { .padding = CLAY_PADDING_ALL(8) }}) {
    // Child element 1
    CLAY_TEXT(CLAY_STRING("Hello World"), CLAY_TEXT_CONFIG({ .fontSize = 16 }));
    // Child element 2 with red background
    CLAY({ .backgroundColor = {255,0,0,255} }) {
        // etc
    }
}

Clay_RenderCommandArray renderCommands = Clay_EndLayout();
```
The result is a nice package of all the information a renderer needs to show the layout (and Clay has several options!).
While this is an innovative and clever way to do things, you may find yourself wanting to avoid macros or to use a more object-oriented approach.
This can be difficult to do in C. However, in C++, we have a few tools at our disposal. Enter, `ClayMan`.

## About ClayMan

`ClayMan` is a wrapper class for Clay. It handles the boilerplate and provides a flexible interface for Clay with a few handy additions.

### Features

- Handles the initialization of Clay.
- Exposes the primary macros of Clay as functions.
- Manages Clay_Strings to allow for std::string usage.
- Provides handy utility functions for convenience.
- Allows to also use classic Clay macros and functions.
- Holds closely to Clay's original macro API.
- Provides flexible ways to create your layout:

Pass element config and child-lambda:
```cpp
ClayMan.element(
    {
        .id = clayMan.hasID("someID"),
        .layout = {.padding = {1, 2, 3, 4}}
    },
    [&](){
        /*children here*/
    }
);
```

Or pass params then manually close:
```cpp
ClayMan.openElement({
        .id = clayMan.hasID("someID"),
        .layout = {.padding = {1, 2, 3, 4}}
});{

    /*children here*/

}ClayMan.closeElement();
```

Or use classic Clay macros:
```cpp
CLAY({
        .id = CLAY_ID("someID"),
        .layout = {.padding = {1, 2, 3, 4}}
}){
    /*children here*/
}
```

Or mix and match, just be sure that each element sticks to one method:
```cpp
ClayMan.openElement(
    {
        .id = clayMan.hashID("greatgreatgrandparent"),
        .layout = {.padding = {4, 5, 6, 7}}
    }
);{ //children of "greatgreatgrandparent" inside this block
    ClayMan.element(
        {
            .id = clayMan.hashID("greatgrandparent"),
            .layout = {.padding = {4, 5, 6, 7}}
        },
        [&](){ //children of "greatgrandparent" inside this lambda
            CLAY(
                {
                    .id = CLAY_ID("grandparent"),
                    .layout = {.padding = 1, 2, 3, 4}
                }
            ){ //children of "grandparent" inside this block
                ClayMan.openElement(
                    {
                        .id = clayMan.hashID("parent"),
                        .layout = {.padding = {4, 5, 6, 7}}
                    }
                );{//children of "parent" in this block
                    ClayMan.element(
                        {
                            .id = clayMan.hashID("child"),
                            .layout = {.padding = {1, 2, 3, 4}}
                        },
                        [&](){}
                    );
                    
                    ClayMan.element(
                        {
                            .id = clayMan.hashID("sibling"),
                            .layout = {.padding = {4, 5, 6, 7}}
                        },
                        [&](){}
                    );
                }ClayMan.closeElement(); //close "parent"
            } //grandparent autocloses because macro was used
        }
    ); //greatgrandparent autocloses because element() was used
}ClayMan.closeElement(); //close "greatgreatgrandparent"
```

## Closing Elements
If using the classic Clay macros or using ClayMan's `element` function, closing of elements is automatic. The main catch with `openElement` is that you must close each element manually by calling `closeElement`. This can be a bit tricky to keep track of when deeply nesting child elements. The ClayMan class automatically closes any forgotten elements to prevent crashes, but it may not close them where you want them closed. One good way to make things easier is to isolate all child elements in a block and close afterward, but it is not necessary:

```cpp
//Call without block
ClayMan.openElement({
    .id = clayMan.hashID("sibling"),
    .layout = {.padding = {4, 5, 6, 7}}
});
//children here
ClayMan.closeElement();

//Equivalent call with block
ClayMan.openElement({
    .id = clayMan.hashID("sibling"),
    .layout = {.padding = {4, 5, 6, 7}}
});{
    //children here
} ClayMan.closeElement();

```

## Strings

In Clay, you must use the `Clay_String` struct to interact with the Clay library. This requires string literals or manual pointer setup, and can be a challenge. With ClayMan, you can use `std::string` or string literals for your textual needs, allowing for dynamic text and variable passing:

```cpp
std::string elementID = "Element1";

Clay_TextElementConfig textConfig = { 
    .textColor = {255,255,255,255}, 
    .fontId = 0, 
    .fontSize = 16 
};

ClayMan.element(
    {.id = clayMan.hashID(elementID)}, 
    [&](){
        std::string text1 = "Here is some text.";
        ClayMan.textElement(text1, textConfig);
    }
);

ClayMan.openElement({.id = clayMan.hashID("Element2")});{
    std::string text2 = "Here is some other text.";
    ClayMan.textElement(text2, textConfig);
} ClayMan.closeElement();
```

**NOTE**: There is currently a 100000 char limit for the internal string arena, this includes all characters for all strings and string literals passed into ClayMan functions each frame.

## How To Use ClayMan (With Raylib Renderer)
To use this library in your project, simply copy the `clayman.hpp` header, the `clayman.cpp` source file, as well as the [include](https://github.com/TimothyHoytBSME/ClayMan/tree/main/include) folder (which has the compatible version of `clay.h` as well as the all renderers) into your working directory. Then, in your project do the following, changing the include paths as needed for your file structure:

```cpp
#include "clayman.hpp"
#include "include/raylib/clay_renderer_raylib.c"

//Your project's main entry
int main(void) {
    
    //Reserve memory for raylib fonts
    Font fonts[1];

    //Initialize ClayMan object using the constructor
    ClayMan clayMan(1024, 786, Raylib_MeasureText, fonts);

    //Initialize Raylib
    Clay_Raylib_Initialize(
        clayMan.getWindowWidth(), 
        clayMan.getWindowHeight(), 
        "Your Project Title", 
        FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT
    );

    //Load fonts after initializing raylib
    fonts[0] = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400);
    
    //Set fonts texture filters
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

    //Raylib render loop
    while (!WindowShouldClose()) { 
       
        //Raylib mouse position and scroll vectors
        Vector2 mousePosition = GetMousePosition(); 
        Vector2 scrollDelta = GetMouseWheelMoveV(); 
        
        //Update clay state (window size, mouse position/scroll, time delta, left button state)
        clayMan.updateClayState(
            GetScreenWidth(), //Raylib window width
            GetScreenHeight(), //Raylib window height
            mousePosition.x, 
            mousePosition.y, 
            scrollDelta.x, 
            scrollDelta.y, 
            GetFrameTime(), //Raylib frame delta
            IsMouseButtonDown(0) //Raylib left button down
        );

        //Prep for layout
        clayMan.beginLayout();
        
        //Example full-window parent container
        clayMan.element(
            { //Configure element
                .id = clayMan.hashID("YourElementID"),
                .layout = {
                    .sizing = clayMan.expandXY(), 
                    .padding = clayMan.padAll(16), 
                    .childGap = 16, 
                    .layoutDirection = CLAY_TOP_TO_BOTTOM
                },
                .backgroundColor = {50,50,50,255}
            },
            [&]{ //Child elements in here
                clayMan.textElement(
                    "Here is some text",
                    { //Configure text
                        .textColor = {255,255,255,255},
                        .fontId = 0, 
                        .fontSize = 16
                    }
                );
            }
        );
        
        //Pass your layout to the manager to get the render commands
        Clay_RenderCommandArray renderCommands = clayMan.endLayout(); 

        BeginDrawing(); //Start Raylib's draw block
        ClearBackground(BLACK); //Raylib's clear function
        Clay_Raylib_Render(renderCommands, fonts); //Render Clay Layout
        EndDrawing(); //End Raylib's draw block
    }
    return 0;
}

```

## Examples And Other Renderers
To use a different renderer, simply replace all of the Raylib related parts with the appropriate counter-parts (like the SDL2 renderer). For your conveneice, the above example and several other examples are available in the [examples](https://github.com/TimothyHoytBSME/ClayMan/tree/main/examples) directory to serve as templates and to show how to properly set up and use ClayMan in different scenarios. The `full-example` shows how to mix the different element styles, while the rest all use the lambda method iwth various renderers and use-cases. ClayMan comes with 3 different renderers: Raylib, SDL2, and SDL3. The layout code in each example is independant of the renderer, so, apart from user-variables in the layout, layouts can be swapped between various render setups, but renderer setups vary widely, and change the overall structure of each example to some degree.

## Events
The Clay library does not handle events. It does, however, have some on-hover and related functionality.
Normally, with Clay, you would use the `Clay_Hovered` and `Clay_OnHover` functions.
You can still use these functions if you are using the original macros or `openElement`; however, if using the `element` function, you will need to take an alternative approach:

```cpp

//These two lines replace Clay_Hovered
bool overElement = ClayMan.pointerOver("elementID");
bool mousePressed = ClayMan.mousePressed();

//This replaces Clay_OnHover and callback
if(overElement && mousePressed){
    //OnClick actions here
} 

//This is the element 
ClayMan.element(
    {
        .id = clayMan.hashID("elementID"),
        .layout = {.sizing = clayMan.fixedSize(150, 50)},
        .backgroundColor = (overElement? (Clay_Color){100,100,100,255} : (Clay_Color){200,200,200,255})
    },
    [&](){
        //text element here
    }
); 
```

Other events will need to be handled/created with other means, and are not covered in the scope of Clay or ClayMan.

## Element Configuration

In Clay, the `CLAY` macro takes a single optional configuration. Optional sub-configurations such as layout, clip, floating, and border all have their own struct types. In ClayMan, the same configuration is optionally passed directly into `element` or `openElement`. The parameters in the configurations have a set order but are all optional. See Clay's [Clay_ElementDeclaration](https://github.com/nicbarker/clay/blob/main/README.md#clay_elementdeclaration) section in its readme for more information. NOTE: For `clip` configuration, `childOffset` will be ignored in ClayMan, and will be replaced with internal scroll offsets automatically if either vertical or horizontal are set. If you are in need of clip without scroll, or wish to handle offsets externally, this is currently unsupported in ClayMan. Please submit an issue or PR if you would like to see this changed.

The `CLAY_TEXT` macro takes its own `Clay_TextElementConfig` and has no children. ClayMan captures this as its own function, `textElement`. See [Strings](#strings) section for an example.


## Advanced Use
If you want to set up a `Clay_Arena`, `Clay_ErrorHandler`, and/or text-measure function yourself, you will need to call `Clay_Initialize` and `Clay_SetMeasureTextFunction` before using any ClayMan functions. You will also need to use the `ClayMan` constructor without the text_measure function:
```cpp
#include "clayman.hpp"
#include "include/raylib/clay_renderer_raylib.c"

//Define Clay_ErrorHandler handler
void handleErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

//Application entry
int main(void) {
    //Initialize ClayMan using alternate Constructor
    ClayMan clayMan(1024, 786);

    //Manually set up Clay_Arena
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory));

    //Manually initialize Clay
    Clay_Initialize(clayMemory, (Clay_Dimensions) {
    .width = (float)clayMan.getWindowWidth(),
    .height = (float)clayMan.getWindowHeight()
    }, (Clay_ErrorHandler) handleErrors);


    //Initialize Raylib
    Clay_Raylib_Initialize(
        clayMan.getWindowWidth(), 
        clayMan.getWindowHeight(), 
        "ClayMan C++ Quick Example", 
        FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT
    );

    //Create fonts after initializing raylib
    Font fonts[] = {LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400)};

    //Set fonts texture filters
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

    //Manually set text measure function
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts); //for raylib, userData is Fonts array

    //Raylib loop
    while (!WindowShouldClose()) {

        //Get mouse info from Raylib
        Vector2 mousePosition = GetMousePosition();
        Vector2 scrollDelta = GetMouseWheelMoveV();

        //Update clay state
        clayMan.updateClayState(
            GetScreenWidth(), //Window dims from Raylib
            GetScreenHeight(), 
            mousePosition.x, 
            mousePosition.y, 
            scrollDelta.x, 
            scrollDelta.y, 
            GetFrameTime(), //Frame delta from Raylib
            IsMouseButtonDown(0) //Left button state from Raylib
        );

        clayMan.beginLayout(); //Prep for layout

        //Build your immediate-mode layout here (or create a function for it)
        Clay_TextElementConfig textConfig = {
            .textColor = {255, 255, 255, 255},
            .fontId = 0,
            .fontSize = 20
        };

        clayMan.element(
            {
                .id = clayMan.hashID("Container"),
                .layout = {
                    .sizing = clayMan.expandXY(),
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                },
                .backgroundColor = {120,120,120,255}
            },
            [&](){
                clayMan.textElement("This is an advanced use example!", textConfig);
            }
        );

        //Manually call endLayout() to get render commands and pass to renderer
        Clay_RenderCommandArray renderCommands = clayMan.endLayout();

        BeginDrawing(); //Rayblib native command
        ClearBackground(BLACK); //Rayblib native command
        Clay_Raylib_Render(renderCommands, fonts); 
        EndDrawing(); //Rayblib native command
    }
    return 0;
}
```

## API

The `ClayMan` class has the following public functions:

### Constructors 
- `ClayMan`
    - Purpose: Initializes the Clay context and creates the ClayMan instance.
    - Params (all required):
        - uint32_t **windowWidth**, the initial window width.
        - uint32_t **windowHeight**, the initial window height.
        - **Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void* userData)**, the renderer's text measuring function.
        - void* **measureTextUserData**, the userData for the text measuring function, usually fonts array.
    - Returns: **ClayMan** instance.
- `ClayMan`
    - Purpose: This constructor only creates the ClayMan object, you will need to create a `Clay_Arena` and call `Clay_Initialize` and `Clay_SetMeasureTextFunction` before using ClayMan functions. See [Advanced Use](#advanced-use).
    - Params (all required):
        - uint32_t **windowWidth**, the initial window width.
        - uint32_t **windowHeight**, the initial window height.
    - Returns: **ClayMan** instance.

### Primary
The following functions are the primary calls to operate the ClayMan instance, and are required.
- `updateClayState`
    - Purpose: Updates the Clay context with mouse, window, and frame information. These values typically come from the renderer. Call this once per frame, usually before calling buildLayout.
    - Params (all required): 
        - uint32_t **windowWidth**, the current window width.
        - uint32_t **windowHeight**, the current window height.
        - float **mouseX**, the current mouse position x-coordinate.
        - float **mouseY**, the current mouse position y-coordinate.
        - float **scrollDeltaX**, the current mousewheel x-delta.
        - float **scrollDeltaY**, the current mousewheel y-delta.
        - float **frameTime**, the time-length of the last frame.
        - bool **leftButtonDown**, the current state of the left mouse button.
    - Returns: None.
- `beginLayout`
    - Purpose: Sets the start of layout building. Resets string arena then calls `Clay_BeginLayout`.
    - Params: None.
    - Returns: None.
- `endLayout`
    - Purpose: Finalizes the layout, handles auto-closing of elements, and calls `Clay_EndLayout`.
    - Params: None.
    - Returns: **Clay_RenderCommandArray** instance, contains all drawing information to be used by renderer.

### Elements
Clay elements can be nested in any heiarchy structure of parents and children, opening each layer as you go in. The last opened element is the current element. Once an element is closed, the parent becomes the current element again, until all elements are closed.
- `element`
    - Purpose: Opens a new element, applies configurations (if any), calles child-lambda, and closes the new element automatically. Replaces `CLAY` macro.
    - Params:
        - (optional) Clay_ElementDeclaration **configs**, a single, unified struct to configure the element. See Clay's [Clay_ElementDeclaration](https://github.com/nicbarker/clay/blob/main/README.md#clay_elementdeclaration) section in its readme for more information.
        - (required) std::function<void()> **childLambda**, written as `[&](){/*children here*/}`, can be empty.
    - Returns: None.
- `openElement`
    - Purpose: Opens a new element and applies configurations (if any). The element will need to be closed manually using `closeElement` after all children.
    - Params:
        - (optional) Clay_ElementDeclaration **configs**, a single, unified struct to configure the element. See Clay's [Clay_ElementDeclaration](https://github.com/nicbarker/clay/blob/main/README.md#clay_elementdeclaration) section in its readme for more information.
    - Returns: None.
- `closeElement`
    - Purpose: Closes the current element. Required for every call to `openElement`. DO NOT CALL if `element` was used to open the element.
    - Params: None.
    - Returns: None.
- `textElement`
    - Purpose: Creates a text element in-place and is self-contained. DO NOT CALL `closeElement`. Replaces `CLAY_TEXT` macro.
    - Params (all optional):
        - std::string (or string literal) **text**, the text for the element.
        - Clay_TextElementConfig **textElementConfig**, the text configuration.
    - Returns: None.

### Sizing
The following are conveniece funcitons for creating Clay_Sizing instances.
- `fixedSize`
    - Purpose: Creates a laout .sizing configuration with fixed X, Y dimensions.
    - Params:
        - uint32_t w, the width of the sizing.
        - uint32_t h, the height of the sizing.
    - Returns: **Clay_Sizing**, a layout sub-configuration.
- `expandXY`
    - Purpose: Creates a laout .sizing configuration for expanding both directions.
    - Params: None.
    - Returns: **Clay_Sizing**, a layout sub-configuration.
- `expandX`
    - Purpose: Creates a laout .sizing configuration for expanding X direction.
    - Params: None.
    - Returns: **Clay_Sizing**, a layout sub-configuration.
- `expandY`
    - Purpose: Creates a laout .sizing configuration for expanding X direction.
    - Params: None.
    - Returns: **Clay_Sizing**, a layout sub-configuration.
- `expandXfixedY`
    - Purpose: Creates a laout .sizing configuration for expanding X direction with a fixed Y dimension.
    - Params: uint32_t **h**, the fixed height of the sizing.
    - Returns: **Clay_Sizing**, a layout sub-configuration.
- `expandYfixedX`
    - Purpose: Creates a laout .sizing configuration for expanding Y direction with a fixed X dimension.
    - Params: uint32_t **w**, the fixed width of the sizing.
    - Returns: **Clay_Sizing**, a layout sub-configuration.

### Padding
The following are convenience functions for creating Clay_Padding instances.
- `padAll`
    - Purpose: Creates a laout .padding configuraion with all sides equal padding.
    - Params: uint16_t **p**, the padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padX`
    - Purpose: Creates a laout .padding configuraion with Left and Right sides equal, Top and Bottom 0.
    - Params: uint16_t **p**, the padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padY`
    - Purpose: Creates a laout .padding configuraion with Top and Bottom sides equal, Left and Right 0.
    - Params: uint16_t **p**, the padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padXY`
    - Purpose: Creates a laout .padding configuraion with Left and Right sides equal and Top and Bottom sides equal.
    - Params:
        - uint16_t **px**, the padding x-dimension.
        - uint16_t **py**, the padding y-dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padLeft`
    - Purpose: Creates a laout .padding configuraion for the Left only, all other sides 0.
    - Params: uint16_t **pl**, the left padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padRight`
    - Purpose: Creates a laout .padding configuraion for the Right only, all other sides 0.
    - Params: uint16_t **pr**, the right padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padTop`
    - Purpose: Creates alaout . padding configuraion for the Top only, all other sides 0.
    - Params: uint16_t **pt**, the top padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.
- `padBottom`
    - Purpose: Creates a laout .padding configuraion for the Bottom only, all other sides 0.
    - Params: uint16_t **pb**, the bottom padding dimension.
    - Returns: **Clay_Padding**, a layout sub-configuration.

### Child Alignment
The following are convenience functions for creating Clay_ChildAlignment instances.
- `centerXY`
    - Purpose: Creates a laout .childAlignment configuration for centering children in both axes.
    - Params: None.
    - Returns: **Clay_ChildAlignment**, a layout sub-configuration

### Utility
The following functions are utility functions for convenience.
- `hashID`
    - Purpose: Converts a human-readable ID into a Clay hashed ID. Replaces `CLAY_ID` macro.
    - Params: std::string (or string literal) **id**, the element ID.
    - Returns: **Clay_ElementId**
- `mousePressed`
    - Purpose: Checks if left mouse button was pressed this frame.
    - Params: None.
    - Returns: **bool**
- `pointerOver`
    - Purpose: Checks if pointer is over an object with given ID.
    - Params: std::string or Clay_String (or string literal) **id**, the element ID.
    - Returns: **bool**
- `getWindowWidth`
    - Purpose: Gets current window width.
    - Params: None.
    - Returns: **int**
- `getWindowHeight`
    - Purpose: Gets current window height.
    - Params: None.
    - Returns: **int**
- `getFramecount`
    - Purpose: Gets current framecount from the manager.
    - Params: None.
    - Returns: **uint32_t**, how many frames have been processed, resets at maximum uint32_t.
- `toClayString`
    - Purpose: Creates an instance of Clay_String from string or string literal.
    - Params: std::string (or string literal) **str**, the text to capture.
    - Returns: **Clay_String** instance.

## Final Notes

Since this library is just a wrapper for Clay; any original clay structs, macros, or functions can still be called. However, there is no need to initialize Clay or create a Clay_Arena of your own. Furthermore, doing so without using the alternate Constructor could cause issues or lead to unforseen behavior, see [Advanced Use](#advanced-use). Do keep in mind, this library (and Clay), are in developement, and anything could change. The version of clay provided in this repo (and renderer), are guaranteed to be compatible, and will be updated accordingly. Any future updates to this library will be posted in the [Change Log](#change-log) section. This wrapper does not expose all of the Clay macros as functions, but most of them can still be manually used without problems if you know what you are doing. If you have any requests or issues, I am open to all. Happy Coding!

## Change Log

- January 21, 2025
    - ClayMan was published.
- January 23, 2025
    - Added example for advanced use.
- January 24, 2025
    - Updated advanced use example to use beginLayout and endLayout, so auto-close still works.
- February 6, 2025
    - Major update to match new CLAY macro with Clay_ElementDeclaration. Simplified API, added an example, and updated examples.
- February 11, 2025
    - Split ClayMan into header and source to allow inclusion in multiple sources. Added an example.
- June 1, 2025
    - Clay scroll config was replaced with clip config. For ClayMan, clip should be treated as scroll for now, just declare horizontal or vertical as true, any childOffset parameter will be replaced with scroll offsets internally. If this is a problem, please submit an issue.
