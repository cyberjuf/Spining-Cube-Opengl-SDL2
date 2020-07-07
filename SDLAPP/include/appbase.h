#ifndef APPBASE_H_INCLUDED
#define APPBASE_H_INCLUDED

#include <SDL.h>
#include<SDL_mixer.h>
#include <algorithm>    // std::find_if
#include <vector>       // std::vector
#include<GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class msgbase;

typedef void (msgbase::*msgbasefunc)(void);
typedef void (*msgfunc)(msgbase*,msgbasefunc,SDL_Event*);

struct msgmap
{
    Uint32 Msg;
    msgfunc MsgFunc;
    msgbasefunc ClassFunc;
};

struct timerdata
{
    msgbase *cls_call;
    Uint32 interval;
    Uint32 timestamp;
    Uint32 code;
    SDL_TimerID timerid;
};

enum MY_EVENT {NONE,MYTIMER_EVENT};

class msgbase
{
public :
    ~msgbase();
    virtual msgmap* FindMsg(Uint32 msg)=0;
    virtual void OnTimer(timerdata *data);
protected:
    std::vector<timerdata*> m_arr_timerdata;
    void SetTimer(Uint32 interval,Uint32 code=-1);
};

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

class GLException : public std::exception
{
protected:
    std::stringstream str;
    bool m_iserror = false;
public:
    GLException(std::string nfunction,int nline);
    GLException(const GLException&obj);
    bool IsError()
    {
        return m_iserror;
    }
    virtual ~GLException() throw() {}
    virtual const char * what() const throw();
};

#define DECLARE_MESSAGE_MAP public: \
    virtual msgmap* FindMsg(Uint32 msg); \
    static std::vector<msgmap> MsgMap
#define BEGIN_BASE_MESSAGE_MAP(nclass) typedef nclass defclass; \
    msgmap* nclass::FindMsg(Uint32 msg){ \
    msgmap* result=nullptr; \
       std::find_if (MsgMap.begin(), MsgMap.end(), [&](msgmap &MsgItem){ \
            if(msg==MsgItem.Msg) {result = &MsgItem; return true;} else return false; \
            }); \
            return result; \
    } \
    std::vector<msgmap> nclass::MsgMap ={
#define BEGIN_MESSAGE_MAP(nclass,baseclass) typedef nclass defclass; \
                                msgmap* nclass::FindMsg(Uint32 msg){ \
                                msgmap* result=nullptr; \
                                   std::find_if (MsgMap.begin(), MsgMap.end(), [&](msgmap &MsgItem){ \
                                        if(msg==MsgItem.Msg) {result = &MsgItem; return true;} else return false; \
                                        }); \
                                        if(result != nullptr) return result; else return baseclass::FindMsg(msg); \
                                } \
                                std::vector<msgmap> nclass::MsgMap ={
#define END_MESSAGE_MAP {0,nullptr,nullptr}}

#define MSG_SDL_QUIT {SDL_QUIT,V_v_sig,static_cast<msgbasefunc>(&defclass::OnSdlQuit)}
#define MSG_SDL_WINDOWEVENT {SDL_WINDOWEVENT,V_SDL_Event_sig,reinterpret_cast<msgbasefunc>(&defclass::OnSdlWindow)}
#define MSG_USER_EVENT {SDL_USEREVENT,V_SDL_UserEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnUserEvent)}
#define MSG_SDL_MOUSEWHEEL {SDL_MOUSEWHEEL,V_SDL_MouseWheel_Event_sig,reinterpret_cast<msgbasefunc>(&defclass::OnMouseWheel)}
#define MSG_SDL_MOUSEMOTION {SDL_MOUSEMOTION,V_SDL_MouseMotion_Event_sig,reinterpret_cast<msgbasefunc>(&defclass::OnMouseMotion)}
#define MSG_SDL_MOUSEBUTTONDOWN {SDL_MOUSEBUTTONDOWN,V_SDL_MouseButtonEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnMouseButtonDown)}
#define MSG_SDL_KEYBORD_EVENT {SDL_KEYDOWN,V_SDL_KeybordEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnKeybord)}

//map window message
#define MSG_SDL_WINDOWEVENT_RESIZED {SDL_WINDOWEVENT_RESIZED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowResized)}
#define MSG_SDL_WINDOWEVENT_SHOWN {SDL_WINDOWEVENT_SHOWN,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowShow)}
#define MSG_SDL_WINDOWEVENT_HIDDEN {SDL_WINDOWEVENT_HIDDEN,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowHidden)}
#define MSG_SDL_WINDOWEVENT_MOVED {SDL_WINDOWEVENT_MOVED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowMove)}
#define MSG_SDL_WINDOWEVENT_ENTER {SDL_WINDOWEVENT_ENTER,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowEnter)}
#define MSG_SDL_WINDOWEVENT_LEAVE {SDL_WINDOWEVENT_LEAVE,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowLeave)}
#define MSG_SDL_WINDOWEVENT_EXPOSED {SDL_WINDOWEVENT_EXPOSED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowExposed)}
#define MSG_SDL_WINDOWEVENT_SIZE_CHANGED {SDL_WINDOWEVENT_SIZE_CHANGED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowSizeChanged)}
#define MSG_SDL_WINDOWEVENT_MINIMIZED {SDL_WINDOWEVENT_MINIMIZED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowMinimized)}
#define MSG_SDL_WINDOWEVENT_MAXIMIZED {SDL_WINDOWEVENT_MAXIMIZED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowMaximized)}
#define MSG_SDL_WINDOWEVENT_RESTORED {SDL_WINDOWEVENT_RESTORED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowRestored)}
#define MSG_SDL_WINDOWEVENT_FOCUS_GAINED {SDL_WINDOWEVENT_FOCUS_GAINED,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowFocusGained)}
#define MSG_SDL_WINDOWEVENT_FOCUS_LOST {SDL_WINDOWEVENT_FOCUS_LOST,V_SDL_WindowEvent_sig,reinterpret_cast<msgbasefunc>(&defclass::OnWindowFocusLost)}

//end map window message

template<class T>
void V_v_sig(T *obj,msgbasefunc classfunc,SDL_Event*)
{
    (obj->*classfunc)();
}

template<class T>
void V_SDL_Event_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(SDL_Event*)>(classfunc);
    (obj->*ctempfunc)(e);
}

template<class T>
void V_SDL_UserEvent_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(SDL_UserEvent*)>(classfunc);
    (obj->*ctempfunc)(&e->user);
}

template<class T>
void V_SDL_WindowEvent_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(SDL_WindowEvent*)>(classfunc);
    (obj->*ctempfunc)(&e->window);
}

template<class T>
void V_SDL_MouseWheel_Event_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(const SDL_MouseWheelEvent&)>(classfunc);
    (obj->*ctempfunc)(e->wheel);
}

template<class T>
void V_SDL_MouseMotion_Event_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(const SDL_MouseMotionEvent&)>(classfunc);
    (obj->*ctempfunc)(e->motion);
}

template<class T>
void V_SDL_MouseButtonEvent_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(const SDL_MouseButtonEvent&)>(classfunc);
    (obj->*ctempfunc)(e->button);
}

template<class T>
void V_SDL_KeybordEvent_sig(T *obj,msgbasefunc classfunc,SDL_Event* e)
{
    auto ctempfunc=reinterpret_cast<void (T::*)(const SDL_KeyboardEvent&)>(classfunc);
    (obj->*ctempfunc)(e->key);
}



#endif // APPBASE_H_INCLUDED
