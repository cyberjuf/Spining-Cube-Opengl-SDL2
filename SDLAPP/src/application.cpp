#include <iostream>
#include "../include/application.h"
#include "../include/window.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <GL/glut.h>
//#include <SDL/SDL_mixer.h>

application *THEAPP =nullptr;

BEGIN_BASE_MESSAGE_MAP(application)
MSG_USER_EVENT,
MSG_SDL_QUIT,
MSG_SDL_WINDOWEVENT,
MSG_SDL_MOUSEWHEEL,
MSG_SDL_MOUSEMOTION,
MSG_SDL_KEYBORD_EVENT,
END_MESSAGE_MAP;

application::application()
{
    THEAPP = this;
}

application::~application()
{
    if(m_mainwnd!=nullptr)
    {
        delete m_mainwnd;
        m_mainwnd=nullptr;
    }
}

void application::OnSdlQuit()
{
    m_mainwnd->ShowCloseWindow();
}

void application::OnSdlWindow(SDL_Event* e)
{
    msgmap *temp = m_mainwnd->FindMsg(e->window.event);
    if(temp==nullptr)
        return;
    (*temp->MsgFunc)(m_mainwnd,temp->ClassFunc,e);
}

void application::SetMainWnd(const SDL * mainWnd)
{
    m_mainwnd=(SDL*)mainWnd;
}

void application::run()
{
    if(m_mainwnd==nullptr)
        throw InitError("Main Window not Set!!!.");
    SDL_Event e;
    Uint32 starttime=0;
    Uint32 endtime=0;
    Uint32 delta=0;
    bool delay = true;
    float currentfps=ImGui::GetIO().Framerate;//milisec
    if(m_mainwnd->GetFramePerSec()==0)
        delay = false;

    while(Isrun)
    {


        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            msgmap *temp = FindMsg(e.type);
            if(temp==nullptr)
                break;
            (*temp->MsgFunc)(this,temp->ClassFunc,&e);
        }
        m_mainwnd->draw();
        m_mainwnd->Update();
        if(delay)
        {
            currentfps=ImGui::GetIO().Framerate;
            if(currentfps > m_mainwnd->GetFramePerSec())
            {
                m_delaytime =(1000.0f / m_mainwnd->GetFramePerSec()*2)- (1000.0f / ImGui::GetIO().Framerate);
                SDL_Delay(m_delaytime);
            }
            else SDL_Delay(0);
        }

    }
}

void application::Init(int argc,char*argv[])
{
    this->argc=argc;
    this->argv=argv;
    //glutInit(&argc, argv);
}

void application::OnUserEvent(SDL_UserEvent *ev)
{
    switch(ev->code)
    {
    case MY_EVENT::MYTIMER_EVENT :
    {
        ((timerdata*)ev->data1)->cls_call->OnTimer((timerdata*)ev->data1);
    }
    break;
    }
}

void application::OnMouseWheel(const SDL_MouseWheelEvent &e)
{
    if(e.y > 0)
        SDL_Log("Mouse Event Wheel UP");
    else
        SDL_Log("Mouse Event Wheel DOWN");
}

void application::OnMouseMotion(const SDL_MouseMotionEvent &)
{
    SDL_Log("Mouse Motion Detected.");
}

void application::OnKeybord(const SDL_KeyboardEvent&ev)
{
    SDL_Log("Keybord event Detected. %c",ev.keysym.sym);
}

int main( int argc, char * argv[] )
{
    try
    {
        if(THEAPP==nullptr)
            throw InitError("TheApp not define!.");
        THEAPP->Init(argc,argv);
        THEAPP->run();
        return EXIT_SUCCESS;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }
    catch (const GLException &err)
    {
        std::cerr << err.what() << std::endl;
    }
    return EXIT_FAILURE;
}
