// #include <cstddef>
// #include <iostream>
#define CLAY_IMPLEMENTATION
#include "include/clay.h"
#include "clayman.hpp"

static bool claymaninstancehasbeencreated = false;

ClayMan::ClayMan(
    const uint32_t initialWidth, 
    const uint32_t initialHeight, 
    Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void* userData),
    void* measureTextUserData
):windowWidth(initialWidth), windowHeight(initialHeight) {
    assert(claymaninstancehasbeencreated == false && "Only One Instance of ClayMan Should be Created!");
    if(windowWidth == 0){windowWidth = 1;}
    if(windowHeight == 0){windowHeight = 1;}
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory));

    Clay_Initialize(clayMemory, (Clay_Dimensions) {
        .width = (float)windowWidth,
        .height = (float)windowHeight
    }, (Clay_ErrorHandler) handleErrors);

    Clay_SetMeasureTextFunction(measureTextFunction, measureTextUserData);

    claymaninstancehasbeencreated = true;
}

ClayMan::ClayMan(const uint32_t initialWidth, const uint32_t initialHeight):windowWidth(initialWidth), windowHeight(initialHeight){
    assert(claymaninstancehasbeencreated == false && "Only One Instance of ClayMan Should be Created!");
    if(windowWidth == 0){windowWidth = 1;}
    if(windowHeight == 0){windowHeight = 1;}
    claymaninstancehasbeencreated = true;
}

ClayMan::ClayMan(const ClayMan &clayMan){
    assert(false && "Do not pass clayMan by value, pass by reference!");
}

void ClayMan::updateClayState(
    const uint32_t width, 
    const uint32_t height, 
    const float mouseX, 
    const float mouseY, 
    const float scrollDeltaX, 
    const float scrollDeltaY, 
    const float frameTime, 
    const bool leftButtonDown
){
    windowWidth = width;
    windowHeight = height;
    if(windowWidth == 0){windowWidth = 1;}
    if(windowHeight == 0){windowHeight = 1;}
    Clay_SetLayoutDimensions((Clay_Dimensions) {
        .width = (float)windowWidth,
        .height = (float)windowHeight
    });

    Clay_SetPointerState(
        (Clay_Vector2) { mouseX, mouseY },
        leftButtonDown
    );

    Clay_UpdateScrollContainers(
        true,
        (Clay_Vector2) { scrollDeltaX, scrollDeltaY },
        frameTime
    );
}

void ClayMan::beginLayout(){
    start = std::chrono::high_resolution_clock::now();
    countFrames();
    resetStringArenaIndex();
    Clay_BeginLayout();
}

Clay_RenderCommandArray ClayMan::endLayout(){
    closeAllElements();
    measureTime();
    return Clay_EndLayout();
}

void ClayMan::element(){
    openElement();
    Clay_ElementDeclaration configs;
    applyElementConfigs(configs);
    closeElement();  
}

void ClayMan::element(Clay_ElementDeclaration configs, std::function<void()> childLambda) {
    
    openElement();
    applyElementConfigs(configs);
    if(childLambda != nullptr){
        childLambda();
    }
    closeElement();           
}

void ClayMan::element(Clay_ElementDeclaration configs){
    openElement();
    applyElementConfigs(configs);
    closeElement();
}

void ClayMan::element(std::function<void()> childLambda){
    openElement();
    Clay_ElementDeclaration configs;
    applyElementConfigs(configs);
    if(childLambda != nullptr){
        childLambda();
    }
    closeElement();
}

void ClayMan::openElement(Clay_ElementDeclaration configs){
    openElement();
    applyElementConfigs(configs);
}

void ClayMan::openElement(){
    Clay__OpenElement();
    openElementCount++;
}

void ClayMan::closeElement(){
    Clay__CloseElement();
    if(openElementCount <=0){
        if(!warnedAboutUnderflow){
            printf("Whoops! All elements are already closed!");
            warnedAboutUnderflow = true;
        }
    }else{
        openElementCount--;
    }
}

void ClayMan::textElement(const std::string& text, const Clay_TextElementConfig textElementConfig){
    Clay_String cs = toClayString(text);
    Clay__OpenTextElement(
        cs, 
        Clay__StoreTextElementConfig((Clay__Clay_TextElementConfigWrapper(textElementConfig)).wrapped)
    );
}

void ClayMan::textElement(const Clay_String& text, const Clay_TextElementConfig textElementConfig){
    Clay__OpenTextElement(
        text, 
        Clay__StoreTextElementConfig((Clay__Clay_TextElementConfigWrapper(textElementConfig)).wrapped)
    );
}

Clay_Sizing ClayMan::fixedSize(const uint32_t w, const uint32_t h) {
    return{
        .width = (Clay_SizingAxis { .size = { .minMax = { (float)w, (float)w } }, .type = CLAY__SIZING_TYPE_FIXED }),
        .height = (Clay_SizingAxis { .size = { .minMax = { (float)h, (float)h } }, .type = CLAY__SIZING_TYPE_FIXED }) 
    };
}

Clay_Sizing ClayMan::expandXY(){
    return {
        .width = (Clay_SizingAxis { .size = { .minMax = {} }, .type = CLAY__SIZING_TYPE_GROW }),
        .height = (Clay_SizingAxis { .size = { .minMax = {} }, .type = CLAY__SIZING_TYPE_GROW })
    };
}

Clay_Sizing ClayMan::expandX(){
    return {
        .width = (Clay_SizingAxis { .size = { .minMax =  {} }, .type = CLAY__SIZING_TYPE_GROW }),
    };
}

Clay_Sizing ClayMan::expandY(){
    return {
        .height = (Clay_SizingAxis { .size = { .minMax = {} }, .type = CLAY__SIZING_TYPE_GROW }),
    };
}

Clay_Sizing ClayMan::expandXfixedY(const uint32_t h){
    return {
        .width = (Clay_SizingAxis { .size = { .minMax = {} }, .type = CLAY__SIZING_TYPE_GROW }),
        .height = (Clay_SizingAxis { .size = { .minMax = { (float)h, (float)h } }, .type = CLAY__SIZING_TYPE_FIXED }) 
    };
}

Clay_Sizing ClayMan::expandYfixedX(const uint32_t w){
    return {
        .width = (Clay_SizingAxis { .size = { .minMax = { (float)w, (float)w } }, .type = CLAY__SIZING_TYPE_FIXED }),
        .height = (Clay_SizingAxis { .size = { .minMax = {} }, .type = CLAY__SIZING_TYPE_GROW })
    };
}

Clay_Padding ClayMan::padAll(const uint16_t p){
    return {p, p, p, p};
}

Clay_Padding ClayMan::padX(const uint16_t p){
    return {p, p, 0, 0};
}

Clay_Padding ClayMan::padY(const uint16_t p){
    return {0, 0, p, p};
}

Clay_Padding ClayMan::padXY(const uint16_t px, const uint16_t py){
    return {px, px, py, py};
}

Clay_Padding ClayMan::padLeft(const uint16_t pl){
    return {pl, 0, 0, 0};
}

Clay_Padding ClayMan::padRight(const uint16_t pr){
    return {0, pr, 0, 0};
}

Clay_Padding ClayMan::padTop(const uint16_t pt){
    return {0, 0, pt, 0};
}

Clay_Padding ClayMan::padBottom(const uint16_t pb){
    return {0, 0, 0, pb};
}

Clay_ChildAlignment ClayMan::centerXY(){
    return {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER};
}

Clay_ElementId ClayMan::hashID(const Clay_String& id){
    return Clay__HashString(id, 0, 0);
}

Clay_ElementId ClayMan::hashID(const std::string& id){
    return Clay__HashString(toClayString(id), 0, 0);
}

bool ClayMan::mousePressed(){
    return Clay_GetCurrentContext()->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME;
}

bool ClayMan::pointerOver(const Clay_String id){
    return Clay_PointerOver(getClayElementId(id));
}

bool ClayMan::pointerOver(const std::string& id){
    return Clay_PointerOver(getClayElementId(toClayString(id)));
}

Clay_ElementId ClayMan::getClayElementId(const Clay_String id){
    return Clay_GetElementId(id);
}

Clay_ElementId ClayMan::getClayElementId(const std::string& id){
    return Clay_GetElementId(toClayString(id));
}

Clay_String ClayMan::toClayString(const std::string& str){
    int32_t length = (int32_t)str.size();
    const char* strchars = insertStringIntoArena(str);
    Clay_String cs = { .length = (int32_t)str.size(), .chars = strchars};
    return cs;
}

void ClayMan::applyElementConfigs(Clay_ElementDeclaration& configs){

    //This may need improved if clipping is used without scrolling
    if(configs.clip.horizontal || configs.clip.vertical){
        configs.clip.childOffset = Clay_GetScrollOffset();
    }

    Clay__ConfigureOpenElement((Clay__Clay_ElementDeclarationWrapper {configs}).wrapped);
}

void ClayMan::closeAllElements(){
    while(openElementCount > 0){
        if(!warnedAboutClose){
            printf("WARN: An element was not closed.");
            warnedAboutClose = true;
        }
        closeElement();
    }
}

int ClayMan::getWindowWidth(){
    return windowWidth;
}

int ClayMan::getWindowHeight(){
    return windowHeight;
}

uint32_t ClayMan::getFramecount(){
    return framecount;
}