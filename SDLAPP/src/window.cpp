#include "../include/window.h"
#include "../include/application.h"
extern application *THEAPP;
BEGIN_BASE_MESSAGE_MAP(SDL)
MSG_SDL_WINDOWEVENT_RESIZED,
MSG_SDL_WINDOWEVENT_SHOWN,
MSG_SDL_WINDOWEVENT_HIDDEN,
MSG_SDL_WINDOWEVENT_MOVED,
MSG_SDL_WINDOWEVENT_ENTER,
MSG_SDL_WINDOWEVENT_LEAVE,
MSG_SDL_WINDOWEVENT_EXPOSED,
MSG_SDL_WINDOWEVENT_SIZE_CHANGED,
MSG_SDL_WINDOWEVENT_MINIMIZED,
MSG_SDL_WINDOWEVENT_MAXIMIZED,
MSG_SDL_WINDOWEVENT_RESTORED,
MSG_SDL_WINDOWEVENT_FOCUS_GAINED,
MSG_SDL_WINDOWEVENT_FOCUS_LOST,
END_MESSAGE_MAP;

SDL::SDL( Uint32 flags,const std::string title)
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();
    m_windowTitle=title;
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    if ( (m_window=SDL_CreateWindow(m_windowTitle.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE| SDL_WINDOW_ALLOW_HIGHDPI)) == nullptr )
        throw InitError("Create Window Error.");
    SDL_SetWindowMinimumSize(m_window,400,300);
    InitGL();
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    //SDL_DestroyRenderer( m_renderer );
    SDL_GL_DeleteContext(m_glcontext);
    SDL_Quit();
}
void SDL::InitGL()
{
    m_glcontext=SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_glcontext);
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    glewExperimental = true;
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name)
    {
        return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name);
    });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        throw InitError("Failed to initialize OpenGL loader!");
    }
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
    glEnable(GL_DEPTH_TEST);

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glcontext);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}

void SDL::Update()
{
    SDL_GL_SwapWindow(m_window);

}

void SDL::ShowCloseWindow(bool IsShow)
{
    m_Isclosewindow = IsShow;
}

void SDL::OnWindowResized(SDL_WindowEvent *ev)
{
    SDL_Log("Window %d resized to %dx%d",
            ev->windowID, ev->data1,
            ev->data2);
    glViewport(0,0,ev->data1,ev->data2);
}

void SDL::OnWindowShow(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d shown", ev->windowID);
}


void SDL::OnWindowHidden(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d hidden", ev->windowID);
}

void SDL::OnWindowMove(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d moved to %d,%d",
            ev->windowID, ev->data1,
            ev->data2);
}

void SDL::OnWindowEnter(SDL_WindowEvent*ev)
{
    SDL_Log("Mouse entered window %d",
            ev->windowID);
}

void SDL::OnWindowLeave(SDL_WindowEvent*ev)
{
    SDL_Log("Mouse left window %d", ev->windowID);
}

void SDL::OnWindowExposed(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d exposed", ev->windowID);
}

void SDL::OnWindowSizeChanged(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d size changed to %dx%d",
            ev->windowID, ev->data1,
            ev->data2);
}

void SDL::OnWindowMinimized(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d minimized", ev->windowID);
}

void SDL::OnWindowMaximized(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d maximized", ev->windowID);
}

void SDL::OnWindowRestored(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d restored", ev->windowID);
}

void SDL::OnWindowFocusGained(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d gained keyboard focus",
            ev->windowID);
}

void SDL::OnWindowFocusLost(SDL_WindowEvent*ev)
{
    SDL_Log("Window %d lost keyboard focus",
            ev->windowID);
}

void SDL::ImGuiDraw()
{
    ImGui::Begin("Setting & Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("clear color", (float*)&m_clearColor);
    ImGui::End();
}

void SDL::OnCloseWindow()
{
     ImGui::Begin("Are you sure to Quit?",&m_Isclosewindow,ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize);
        ImGui::SameLine(10,10);
        if(ImGui::Button("Quit now."))
        {
            m_Isclosewindow=false;
            THEAPP->SetRunApp(false);
        }
        ImGui::SameLine(100,10);
        if(ImGui::Button("Cancle"))
        {
            m_Isclosewindow=false;
        }
        ImGui::End();
}


void SDL::draw()
{

    if(!m_glInit)
    {
        m_glInit=true;
        //SetCurrent();

        //glEnable(GL_AUTO_NORMAL);
        // glShadeModel(GL_SMOOTH);
    }


    glClearColor(m_clearColor[0],m_clearColor[1],m_clearColor[2],m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    render();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();
    ImGuiDraw();
    if(m_Isclosewindow)
    {
       OnCloseWindow();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
