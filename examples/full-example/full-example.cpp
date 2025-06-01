#include "../../clayman.hpp"
#include "../../include/raylib/clay_renderer_raylib.c"
#include "../../include/clay.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////init manager//////////////////////////////////////

//Reserve global memory for fonts
Font fonts[1];

//Initialize ClayMan object
ClayMan clayMan(1024, 786, Raylib_MeasureText, fonts); 

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////raylib helper functions//////////////////////////////////////

//Called to set up Clay_Raylib renderer
void initRaylib(uint32_t initialWidth, uint32_t initialHeight, const char* title, const char* fontPath, uint32_t loadedFontSize){
    //Initialize Raylib renderer
    Clay_Raylib_Initialize(initialWidth, initialHeight, title, FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    //Load fonts after raylib
    fonts[0] = LoadFontEx(fontPath, loadedFontSize, 0, 400); 

    //Set font filters
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR); //Native Raylib command
}

//Called to render the layout with Raylib
void raylibRender(Clay_RenderCommandArray renderCommands){
    BeginDrawing(); //Native Raylib command
    ClearBackground(BLACK); //Native Raylib command
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing(); //Native Raylib command
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////app-specific//////////////////////////////////////

//Singleton for app data
struct AppData {
    //Gets the singleton object instance
    static AppData& getInstance() { 
        static AppData instance; 
        return instance;
    }

    typedef struct {
        uint32_t id;
        std::string title;
        std::string contents;
    } Document;

    std::vector<Document> documents = {
        {0,"Squirrels", "The Secret Life of Squirrels: Nature's Clever Acrobats\nSquirrels are often overlooked creatures, dismissed as mere park inhabitants or backyard nuisances. Yet, beneath their fluffy tails and twitching noses lies an intricate world of cunning, agility, and survival tactics that are nothing short of fascinating. As one of the most common mammals in North America, squirrels have adapted to a wide range of environments from bustling urban centers to tranquil forests and have developed a variety of unique behaviors that continue to intrigue scientists and nature enthusiasts alike.\n\nMaster Tree Climbers\nAt the heart of a squirrel's skill set is its impressive ability to navigate trees with ease. Whether they're darting from branch to branch or leaping across wide gaps, squirrels possess an innate talent for acrobatics. Their powerful hind legs, which are longer than their front legs, give them remarkable jumping power. With a tail that acts as a counterbalance, squirrels can leap distances of up to ten times the length of their body, making them some of the best aerial acrobats in the animal kingdom.\nBut it's not just their agility that makes them exceptional climbers. Squirrels' sharp, curved claws allow them to grip tree bark with precision, while the soft pads on their feet provide traction on slippery surfaces. Their ability to run at high speeds and scale vertical trunks with ease is a testament to the evolutionary adaptations that have made them so successful in their arboreal habitats.\n\nFood Hoarders Extraordinaire\nSquirrels are often seen frantically gathering nuts, seeds, and even fungi in preparation for winter. While this behavior may seem like instinctual hoarding, it is actually a survival strategy that has been honed over millions of years. Known as \"scatter hoarding,\" squirrels store their food in a variety of hidden locations, often burying it deep in the soil or stashing it in hollowed-out tree trunks.\nInterestingly, squirrels have an incredible memory for the locations of their caches. Research has shown that they can remember thousands of hiding spots, often returning to them months later when food is scarce. However, they don't always recover every stash some forgotten caches eventually sprout into new trees, contributing to forest regeneration. This unintentional role as forest gardeners highlights the ecological importance of squirrels in their ecosystems.\n\nThe Great Squirrel Debate: Urban vs. Wild\nWhile squirrels are most commonly associated with rural or wooded areas, their adaptability has allowed them to thrive in urban environments as well. In cities, squirrels have become adept at finding food sources in places like parks, streets, and even garbage cans. However, their urban counterparts face unique challenges, including traffic, predators, and the lack of natural shelters. Despite these obstacles, squirrels in urban areas are often observed using human infrastructure such as buildings, bridges, and power lines as highways for their acrobatic escapades.\nThere is, however, a growing concern regarding the impact of urban life on squirrel populations. Pollution, deforestation, and the loss of natural habitats are making it more difficult for squirrels to find adequate food and shelter. As a result, conservationists are focusing on creating squirrel-friendly spaces within cities, with the goal of ensuring these resourceful creatures continue to thrive in both rural and urban landscapes.\n\nA Symbol of Resilience\nIn many cultures, squirrels are symbols of resourcefulness, adaptability, and preparation. Their ability to thrive in a variety of environments while navigating challenges with agility and grace serves as a reminder of the resilience inherent in nature. Whether you encounter them in a quiet forest, a city park, or your own backyard, squirrels are creatures that never fail to amaze with their endless energy and ingenuity.\nIn the end, squirrels may be small, but they are mighty in their ability to survive and thrive in a world that is constantly changing. So next time you spot one hopping across a branch or darting across your lawn, take a moment to appreciate the remarkable acrobat at work a true marvel of the natural world.\n" },
        {1,"Lorem Ipsum", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." },
        {2,"Vacuum Instructions", "Chapter 3: Getting Started - Unpacking and Setup\n\nCongratulations on your new SuperClean Pro 5000 vacuum cleaner! In this section, we will guide you through the simple steps to get your vacuum up and running. Before you begin, please ensure that you have all the components listed in the \"Package Contents\" section on page 2.\n\n1. Unboxing Your Vacuum\nCarefully remove the vacuum cleaner from the box. Avoid using sharp objects that could damage the product. Once removed, place the unit on a flat, stable surface to proceed with the setup. Inside the box, you should find:\n\n    The main vacuum unit\n    A telescoping extension wand\n    A set of specialized cleaning tools (crevice tool, upholstery brush, etc.)\n    A reusable dust bag (if applicable)\n    A power cord with a 3-prong plug\n    A set of quick-start instructions\n\n2. Assembling Your Vacuum\nBegin by attaching the extension wand to the main body of the vacuum cleaner. Line up the connectors and twist the wand into place until you hear a click. Next, select the desired cleaning tool and firmly attach it to the wand's end, ensuring it is securely locked in.\n\nFor models that require a dust bag, slide the bag into the compartment at the back of the vacuum, making sure it is properly aligned with the internal mechanism. If your vacuum uses a bagless system, ensure the dust container is correctly seated and locked in place before use.\n\n3. Powering On\nTo start the vacuum, plug the power cord into a grounded electrical outlet. Once plugged in, locate the power switch, usually positioned on the side of the handle or body of the unit, depending on your model. Press the switch to the \"On\" position, and you should hear the motor begin to hum. If the vacuum does not power on, check that the power cord is securely plugged in, and ensure there are no blockages in the power switch.\n\nNote: Before first use, ensure that the vacuum filter (if your model has one) is properly installed. If unsure, refer to \"Section 5: Maintenance\" for filter installation instructions." },
        {3,"To Delete", "This article will get automatically deleted." },
        {4,"Article 5", "Article 5" }
    };

    uint32_t selectedDocumentIndex = 0;

    //Toggles popup
    bool showPopup = false;

    //Holds the images, once loaded, for the popup close button
    Texture2D closeBlackTexture;
    Texture2D closeWhiteTexture;

    struct Colors{
        Clay_Color white = { 255, 255, 255, 255};
        Clay_Color gray_med = { 90, 90, 90, 255 };
        Clay_Color gray_light = { 140, 140, 140, 255 };
        Clay_Color gray_light2 = { 120, 120, 120, 255 };
        Clay_Color gray_light2_alpha = { 120, 120, 120, 120 };
        Clay_Color gray_dark = { 40, 40, 40, 255 };
        Clay_Color gray_dark2 = { 43, 41, 51, 255 };
    } colors; //Instantly invoke instance with defaults

    //Called after raylib is initialized
    void loadImages(){
        Image closeBlackImage = LoadImage("resources/CloseBlack.png");
        Image closeWhiteImage = LoadImage("resources/CloseWhite.png");
        closeBlackTexture = LoadTextureFromImage(closeBlackImage);
        closeWhiteTexture = LoadTextureFromImage(closeWhiteImage);
    }

    //Used to change documents on an interval, to demonstrate dynamic strings
    void updateStrings(){
        uint16_t framecount = clayMan.getFramecount();
        if(clayMan.getFramecount() == 200){
            documents[2] = {documents[2].id, "Change-up", "Testing the dynamic ability of strings with C++ magic!"};
        }else if(framecount == 400){ 
            documents.push_back({13254,"New Doc","This doc was dynamically added! It will be dynamically removed soon!"});
        }else if(framecount == 600){
            uint32_t delInd = documents.size() - 3;
            documents.erase(documents.begin() + delInd);
        }
    }

    private:
        //Internal singleton constructor
        AppData() {}
};
//Global application data singleton instance variable
AppData appData = AppData::getInstance();

//Singleton for element configurations
struct AppConfigs{
    //Gets the singleton object instance
    static AppConfigs& getInstance() {
        static AppConfigs instance; 
        return instance;
    }

    Clay_LayoutConfig outerLayoutConfig = Clay_LayoutConfig{ 
        .sizing = clayMan.expandXY(), 
        .padding = clayMan.padAll(16), 
        .childGap = 16, 
        .layoutDirection = CLAY_TOP_TO_BOTTOM 
    };

    Clay_LayoutConfig headerLayoutConfig = Clay_LayoutConfig{ 
        .sizing = clayMan.expandXfixedY(60), 
        .padding = clayMan.padX(16), 
        .childGap = 16, 
        .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } 
    };

    Clay_LayoutConfig headerButtonLayoutConfig = Clay_LayoutConfig { .padding = clayMan.padXY(16, 8) };
    Clay_LayoutConfig dropdownLayoutConfig = Clay_LayoutConfig {.sizing = clayMan.fixedSize(200, 200), .layoutDirection = CLAY_TOP_TO_BOTTOM };
    Clay_LayoutConfig dropdownItemLayoutConfig = Clay_LayoutConfig { .padding = clayMan.padAll(16)};
    Clay_LayoutConfig sidebarButtonLayout = {.sizing = clayMan.expandX(), .padding = clayMan.padAll(16)};

    Clay_TextElementConfig headerTextConfig = { 
        .textColor = appData.colors.white, 
        .fontId = 0, 
        .fontSize = 16 
    };

    Clay_TextElementConfig sidebarTextConfig = { 
        .textColor = appData.colors.white, 
        .fontId = 0, 
        .fontSize = 20
    };

    Clay_TextElementConfig documentTextConfig = { 
        .textColor = appData.colors.white, 
        .fontId = 0, 
        .fontSize = 24
    };
    
    uint16_t borderWidth = 4;
    Clay_BorderWidth border = (Clay_BorderWidth){4,4,4,4,0};

    private:
        //Internal singleton constructor
        AppConfigs() {}

}; 
//Global singleton instance variable for Clay configs
AppConfigs appConfigs = AppConfigs::getInstance();

//Singleton for reusable elements
struct ReusableElements{
    //Gets the singleton object instance
    static ReusableElements& getInstance() {
        static ReusableElements instance; 
        return instance;
    }

    void RenderHeaderButton(std::string text) {
        clayMan.element(
            {
                .layout = appConfigs.headerButtonLayoutConfig,
                .backgroundColor = appData.colors.gray_light, 
                .cornerRadius = {5,5,5,5},
            },
            [&]{ //Children in this lambda
                clayMan.textElement(text, appConfigs.headerTextConfig);
            }
        );
    }

    void RenderDropdownMenuItem(std::string text) {
        clayMan.element(
            {
                .layout = appConfigs.dropdownItemLayoutConfig,
            },
            [&]{
                clayMan.textElement(text, appConfigs.headerTextConfig);
            }
        );
    }

    private:
        //Internal singleton constructor
        ReusableElements() {}

};
//Global singleton variable for reusable element functions
ReusableElements reusableElements = ReusableElements::getInstance();

//Singleton for popup elements
struct Popup{
    //Gets the singleton object instance
    static Popup& getInstance(){
        static Popup instance;
        return instance;
    }

    //Clay_OnHover callback, for classic macro element
    static void HandlePopupButtonHover(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
        if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
            appData.showPopup = !appData.showPopup;
        }
    }

    //Using classic CLAY macros
    void popUpButton(){
        CLAY({
            .id = CLAY_ID("PopUpButton"),
            .layout = appConfigs.dropdownItemLayoutConfig,
        }){
            Clay_OnHover(HandlePopupButtonHover, 0);
            CLAY_TEXT(CLAY_STRING("PopUp"), CLAY_TEXT_CONFIG(appConfigs.headerTextConfig));
        }
    }

    //Example popup element, using all element methods nested here as an example
    void popUpWindow(){

        //On-CLick for close button
        if(clayMan.pointerOver("PopupClose") && clayMan.mousePressed()){appData.showPopup = false;}

        if(appData.showPopup){   
            //Popup parameters
            float popupWidth = clayMan.getWindowWidth() * 0.5;
            float popupHeight = clayMan.getWindowHeight() * 0.5;

            //This is the lambda method, it takes id, configs, and child-lambda. Do not call closeElement()
            clayMan.element( 
                {
                    .id = clayMan.hashID("Popup"),
                    .layout = {.sizing = clayMan.fixedSize(popupWidth, popupHeight), .layoutDirection = CLAY_TOP_TO_BOTTOM},
                    .backgroundColor = appData.colors.gray_dark2, 
                    .cornerRadius = {8,8,8,8},
                    .floating = {
                        .parentId = clayMan.getClayElementId("OuterContainer").id,
                        .attachPoints = {.element = CLAY_ATTACH_POINT_CENTER_CENTER, .parent = CLAY_ATTACH_POINT_CENTER_CENTER},
                        .attachTo = CLAY_ATTACH_TO_PARENT
                    },
                    .border = {.color = appData.colors.gray_light, .width = appConfigs.border},
                },
                [&]{//Children of popup are in this lambda
                    //This is the openeElement method, call closeElement after child elements
                    clayMan.openElement({
                        .id = clayMan.hashID("PopupCloseContainer"),
                        .layout = {.sizing = clayMan.expandX(), .padding = clayMan.padAll(appConfigs.borderWidth), .childAlignment = {.x = CLAY_ALIGN_X_RIGHT}}
                    });{//Children of close container are in this block, then closeElement is called
                        //This is the element method again. 
                        clayMan.element(
                            {
                                .id = clayMan.hashID("PopupClose"),
                                .layout = {.sizing = clayMan.fixedSize(24,24)}
                            },
                            [&](){//Children of popup close button in this lambda
                                //This is the original macro method, do not call endConfigs or closeElement
                                CLAY( 
                                    {
                                        .id = CLAY_ID("CloseImage"),
                                        .layout = {.sizing = {
                                            .width = (Clay_SizingAxis { .size = { .minMax = { 0 } }, .type = CLAY__SIZING_TYPE_GROW }), //expand x
                                            .height = (Clay_SizingAxis { .size = { .minMax = { 0 } }, .type = CLAY__SIZING_TYPE_GROW }) //expand y
                                        }},
                                        .cornerRadius = {8,8,8,8},
                                        .image = {
                                                .imageData = Clay_Hovered()? &appData.closeWhiteTexture : &appData.closeBlackTexture,
                                                .sourceDimensions = {64, 64}
                                        }
                                        
                                    }
                                ){}//Children of CLoseImage would go here inside {}
                            }
                        );
                    }clayMan.closeElement(); //We call closeelement to close popup close container element

                    clayMan.element(
                        {
                            .layout = { .sizing = clayMan.expandXY(), .padding = clayMan.padAll(80)}
                        },
                        [&]{
                            clayMan.textElement("This is a pop-up window!", appConfigs.documentTextConfig);
                        }
                    );
                }
            );
        }
    }

    private:
        //Internal singleton constructor
        Popup() {}
};
//Global singleton instance variable for popup functions
Popup popup = Popup::getInstance();



/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////main layout function//////////////////////////////////

//The user layout function, call between beginLayout() and endLayout().
void myLayout(){
    clayMan.element(
        {
            .id = clayMan.hashID("OuterContainer"),
            .layout = appConfigs.outerLayoutConfig,
            .backgroundColor = appData.colors.gray_dark2
        },
        [&]{ 
            clayMan.element(
                {
                    .id = clayMan.hashID("HeaderBar"),
                    .layout = appConfigs.headerLayoutConfig,
                    .backgroundColor = appData.colors.gray_med,
                    .cornerRadius = {8,8,8,8}
                },
                [&]{
                    clayMan.element(
                        {
                            .id = clayMan.hashID("FileButton"),
                            .layout = {.padding = clayMan.padXY(16, 8)},
                            .backgroundColor = appData.colors.gray_light,
                            .cornerRadius = {5,5,5,5}
                        },
                        [&]{
                            clayMan.textElement("File", appConfigs.headerTextConfig);

                            if (!appData.showPopup & (clayMan.pointerOver("FileButton") || clayMan.pointerOver("FileMenu"))) {
                                clayMan.element(
                                    {
                                        .id = clayMan.hashID("FileMenu"),
                                        .layout = {.padding = clayMan.padY(8)},
                                        .floating = {
                                            .attachPoints = {
                                                .element=CLAY_ATTACH_POINT_LEFT_TOP, 
                                                .parent=CLAY_ATTACH_POINT_LEFT_BOTTOM
                                            },
                                            .attachTo = CLAY_ATTACH_TO_PARENT
                                        }
                                    },
                                    [&]{
                                        clayMan.element(
                                            {
                                                .layout = appConfigs.dropdownLayoutConfig,
                                                .backgroundColor = appData.colors.gray_dark2,
                                                .cornerRadius = {8,8,8,8}
                                            },
                                            [&]{
                                                popup.popUpButton();
                                                reusableElements.RenderDropdownMenuItem("New");
                                                reusableElements.RenderDropdownMenuItem("Open");
                                                reusableElements.RenderDropdownMenuItem("Close");
                                            }
                                        );
                                    }
                                );
                            }
                        }
                    );

                    reusableElements.RenderHeaderButton("Edit");

                    clayMan.element(
                        {
                            .id = clayMan.hashID("HeaderSpacer"),
                            .layout = {.sizing = clayMan.expandXY()}
                        }, 
                        [&](){}
                    );

                    reusableElements.RenderHeaderButton("Upload");
                    reusableElements.RenderHeaderButton("Media");
                    reusableElements.RenderHeaderButton("Support");
                }
            );
            clayMan.element(
                {
                    .id = clayMan.hashID("LowerContent"),
                    .layout = {.sizing = clayMan.expandXY(), .childGap = 16}
                },
                [&]{
                    clayMan.element(
                        {
                            .id = clayMan.hashID("Sidebar"),
                            .layout = {
                                .sizing = clayMan.expandYfixedX(250), 
                                .padding = clayMan.padAll(16), 
                                .childGap = 8, 
                                .layoutDirection = CLAY_TOP_TO_BOTTOM 
                            },
                            .backgroundColor = appData.colors.gray_med
                        },
                        [&]{ 
                            for (int i = 0; i < appData.documents.size(); i++) {
                                if (i == appData.selectedDocumentIndex) {
                                    clayMan.element(
                                        {
                                            .layout = appConfigs.sidebarButtonLayout,
                                            .backgroundColor = appData.colors.gray_light2,
                                            .cornerRadius = {8,8,8,8}
                                        },
                                        [&]{
                                            clayMan.textElement(appData.documents[i].title, appConfigs.sidebarTextConfig);
                                        }
                                    );
                                }else{
                                    std::string docID = "SidebarButton" + std::to_string(i);
                                    
                                    bool overButton = !appData.showPopup & clayMan.pointerOver(docID);
                                    bool mousePressed = clayMan.mousePressed();
                                    if(overButton && mousePressed){appData.selectedDocumentIndex = i;}

                                    clayMan.element(
                                        {
                                            .id = clayMan.hashID(docID),
                                            .layout = appConfigs.sidebarButtonLayout,
                                            .backgroundColor = overButton? appData.colors.gray_light2_alpha : (Clay_Color){0,0,0,0},
                                            .cornerRadius = {8,8,8,8}
                                        },
                                        [&]{ 
                                            clayMan.textElement(appData.documents[i].title, appConfigs.sidebarTextConfig);
                                        }
                                    );
                                }
                            }

                        }
                    );
                    clayMan.element(
                        {
                            .id = clayMan.hashID("MainContent"),
                            .layout = { 
                                .sizing = clayMan.expandXY(), 
                                .padding = clayMan.padAll(16), 
                                .childGap = 16, 
                                .layoutDirection = CLAY_TOP_TO_BOTTOM 
                            },
                            .backgroundColor = appData.colors.gray_med,
                            .clip = {.vertical = true} //TODO: call Clay_GetScrollOffset() internally for each child
                        },
                        [&]{
                            if(appData.selectedDocumentIndex >= appData.documents.size()) appData.selectedDocumentIndex = appData.documents.size() - 1;

                            clayMan.textElement(appData.documents[appData.selectedDocumentIndex].title, appConfigs.documentTextConfig);
                            clayMan.textElement(appData.documents[appData.selectedDocumentIndex].contents, appConfigs.documentTextConfig);

                        } 
                    );
                }
            );
            popup.popUpWindow();
        }
    );
}

/////////////////////////////////////////////////////////////////////
///////////////////////application entry/////////////////////////////

int main(void) {

    //Initialize Raylib
    initRaylib(
        clayMan.getWindowWidth(),
        clayMan.getWindowHeight(), 
        "ClayMan C++ Full Example", 
        "resources/Roboto-Regular.ttf", 
        48
    );

    appData.loadImages();

    //Raylib render loop
    while (!WindowShouldClose()) { 
       
        appData.updateStrings();

        //Get mouse data from Raylib native functions
        Vector2 mousePosition = GetMousePosition(); 
        Vector2 scrollDelta = GetMouseWheelMoveV();

        //Update internal mouse, time, and windows state
        clayMan.updateClayState(
            GetScreenWidth(), //Get window dimensions from Raylib
            GetScreenHeight(), 
            mousePosition.x, 
            mousePosition.y, 
            scrollDelta.x,
            scrollDelta.y, 
            GetFrameTime(), //Get fram delta from Raylib
            IsMouseButtonDown(0) //Get left button state from Raylib
        );
        
        clayMan.beginLayout(); //prep frame for layout
        myLayout(); //build your layout
        Clay_RenderCommandArray renderCommands = clayMan.endLayout(); //get the render commands
        raylibRender(renderCommands); //Render the layout
        
    }
    return 0;
}