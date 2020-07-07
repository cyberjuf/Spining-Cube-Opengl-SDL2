#ifndef MYWINDOW_H_INCLUDED
#define MYWINDOW_H_INCLUDED
#include "../SDLAPP/include/myclass.h"
#include "../SDLAPP/include/SDL_Music.h"
class mywindow :public SDL
{
public:
    bool m_menuopen = true;
    mywindow();
    ~mywindow();
    void Change_ZPos(float inc);
    void ToggleSpin()
    {
        spin=!spin;
    }
    virtual void OnWindowResized(SDL_WindowEvent*ev);
    virtual void OnWindowSizeChanged(SDL_WindowEvent*ev);
    virtual void render();
    virtual void OnTimer(timerdata *data);
protected:
    virtual void ImGuiDraw();
private:
    glm::vec4 g_clearColor,m_objColor= {1.0f,0.27f,0.0f,1.0f};
    glm::vec4 old_obj_color;
    const std::string m_u_color_name ="fragColor";
    bool window_swap=true;
    GLOBJ myobj;
    //GLLine<double> myobj;
    bool spin = true;
    SDL_Music mywave;
    DECLARE_MESSAGE_MAP;
};

#endif // MYWINDOW_H_INCLUDED
