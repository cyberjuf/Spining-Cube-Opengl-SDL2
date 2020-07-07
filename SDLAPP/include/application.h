#ifndef APPLICATION_H
#define APPLICATION_H
#include "../include/appbase.h"
class SDL;
class application : public msgbase
{
public:
    application();
    virtual ~application();
    void SetMainWnd(const SDL * mainWnd);
    void SetRunApp(bool running=true)
    {
        Isrun=running;
        if(Isrun==false)SDL_Log("Application Quit Normal.");
    }
    virtual void run();
    virtual void Init(int argc,char*argv[]);
    virtual void OnSdlQuit();
    virtual void OnSdlWindow(SDL_Event* e);
    virtual void OnUserEvent(SDL_UserEvent *ev);
    virtual void OnMouseWheel(const SDL_MouseWheelEvent &e);
    virtual void OnMouseMotion(const SDL_MouseMotionEvent &e);
    virtual void OnKeybord(const SDL_KeyboardEvent&ev);
protected:
    int argc;
    char ** argv;
    SDL *m_mainwnd=nullptr;
private:
    application(const application &other) {}
    void operator=(const application &other) {}
    bool Isrun = true;
    unsigned int m_delaytime = 0;
    DECLARE_MESSAGE_MAP;
};

#endif // APPLICATION_H
