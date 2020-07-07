#include "../SDLAPP/include/myclass.h"
#include "mywindow.h"
class testapp : public application
{
public:
    void Init(int argc,char*argv[]);
    virtual void OnMouseWheel(const SDL_MouseWheelEvent &e);
    virtual void OnMouseMotion(const SDL_MouseMotionEvent &e);
    virtual void OnMouseButtonDown(SDL_MouseButtonEvent& ev);
    virtual void OnKeybord(const SDL_KeyboardEvent&ev);
    DECLARE_MESSAGE_MAP;
};
BEGIN_MESSAGE_MAP(testapp,application)
    MSG_SDL_MOUSEWHEEL,
    MSG_SDL_MOUSEMOTION,
    MSG_SDL_MOUSEBUTTONDOWN,
    MSG_SDL_KEYBORD_EVENT,
END_MESSAGE_MAP;
testapp myapp;


void testapp::Init(int argc,char*argv[])
{
    application::Init(argc,argv);
    SetMainWnd(new mywindow());
}

void testapp::OnMouseWheel(const SDL_MouseWheelEvent &e)
{

    if(e.y > 0)
        ((mywindow*)m_mainwnd)->Change_ZPos(0.02);
    else ((mywindow*)m_mainwnd)->Change_ZPos(-0.02);
}

void testapp::OnMouseMotion(const SDL_MouseMotionEvent &e)
{
    /* int width,height;
     SDL_GetWindowSize(m_mainwnd->GetWindow(),&width,&height);
     switch(e.state){
         case SDL_BUTTON_LMASK :
             if(e.yrel>0) ((mywindow*)m_mainwnd)->Inc_RoX(0.5); else if(e.yrel<0) ((mywindow*)m_mainwnd)->Inc_RoX(-0.5);
             if(e.xrel>0) ((mywindow*)m_mainwnd)->Inc_RoY(0.5); else if(e.xrel<0) ((mywindow*)m_mainwnd)->Inc_RoY(-0.5);
             break;
         case SDL_BUTTON_RMASK :
             if(e.xrel>0 && e.y > height/2 || e.xrel<0 && e.y<height/2) ((mywindow*)m_mainwnd)->Inc_RoZ(0.5);
             else if(e.xrel>0 && e.y < height/2 || e.xrel<0 && e.y>height/2) ((mywindow*)m_mainwnd)->Inc_RoZ(-0.5);

             if(e.yrel<0 && e.x > width/2 || e.yrel>0 && e.x < width/2) ((mywindow*)m_mainwnd)->Inc_RoZ(0.5);
             else if(e.yrel<0 && e.x < width/2 || e.yrel>0 && e.x >width/2) ((mywindow*)m_mainwnd)->Inc_RoZ(-0.5);
             break;
     }*/
}

void testapp::OnMouseButtonDown(SDL_MouseButtonEvent& ev)
{
    //  if(ev.button == SDL_BUTTON_LEFT && ev.clicks==1)
    // {
    //     ((mywindow*)m_mainwnd)->ToggleSpin();
    // }
}

void testapp::OnKeybord(const SDL_KeyboardEvent&ev)
{
    application::OnKeybord(ev);
    if(ev.keysym.scancode == SDL_SCANCODE_SPACE)
    {
        ((mywindow*)m_mainwnd)->m_menuopen = !((mywindow*)m_mainwnd)->m_menuopen;
    }
}


