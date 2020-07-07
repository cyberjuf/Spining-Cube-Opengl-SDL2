#ifndef WINDOW_H
#define WINDOW_H
#include "../include/appbase.h"
#include "../include/GLOBJ.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <exception>
#include <string>
class SDL : public msgbase
{

    //SDL_Renderer * m_renderer;
    SDL_GLContext m_glcontext;
public:
    SDL_Window* GetWindow()const
    {
        return m_window;
    }
    SDL( Uint32 flags = 0,const std::string title="");
    void Update();

    float GetFramePerSec()
    {
        return framepersec;
    }

    void SetFramePerSec(float value)
    {
        framepersec=value;
    }

    const Uint32 GetwindowID()
    {
        Uint32 result= SDL_GetWindowID(m_window);
        if(result==0)
            throw InitError();
        return result;
    }
    virtual ~SDL();
    virtual void InitGL();
    virtual void draw();
    virtual void render()=0;
    virtual void OnCloseWindow();
    virtual void ShowCloseWindow(bool IsShow=true);
    virtual void OnWindowResized(SDL_WindowEvent*ev);
    virtual void OnWindowShow(SDL_WindowEvent*ev);
    virtual void OnWindowHidden(SDL_WindowEvent*ev);
    virtual void OnWindowMove(SDL_WindowEvent*ev);
    virtual void OnWindowEnter(SDL_WindowEvent*ev);
    virtual void OnWindowLeave(SDL_WindowEvent*ev);
    virtual void OnWindowExposed(SDL_WindowEvent*ev);
    virtual void OnWindowSizeChanged(SDL_WindowEvent*ev);
    virtual void OnWindowMinimized(SDL_WindowEvent*ev);
    virtual void OnWindowMaximized(SDL_WindowEvent*ev);
    virtual void OnWindowRestored(SDL_WindowEvent*ev);
    virtual void OnWindowFocusGained(SDL_WindowEvent*ev);
    virtual void OnWindowFocusLost(SDL_WindowEvent*ev);
protected:
    SDL_Window * m_window=nullptr;
    glm::vec4 m_clearColor= {0.235f,0.0f,0.0f,1.0f};
    std::string m_windowTitle;
    virtual void ImGuiDraw();
private:
    float framepersec =120.0f;
    bool m_glInit = false;
    bool m_Isclosewindow = false;
    SDL(const SDL &other) {}
    void operator=(const SDL &other) {}
    DECLARE_MESSAGE_MAP;
};

#endif // WINDOW_H
