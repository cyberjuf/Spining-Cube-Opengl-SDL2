#include "../include/appbase.h"

Uint32 Timer_Callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;
    /* In this example, our callback pushes an SDL_USEREVENT event
    into the queue, and causes our callback to be called again at the
    same interval: */
    // timerdata *mytimerdata = (timerdata*)param;
    //mytimerdata->timestamp=SDL_GetTicks();
    userevent.type = SDL_USEREVENT;
    userevent.code = MY_EVENT::MYTIMER_EVENT;
    userevent.data1 = param;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}



const char * InitError::what() const throw()
{
    return msg.c_str();
}

GLException::GLException(std::string nfunction,int nline):exception()
{

    while (GLenum error = glGetError())
    {

        std::cerr <<std::endl<< "[OpenGL Error] code: " << error << "  in function name :("<<nfunction<<")"<<" line number : "<<nline<<std::endl;
        if(error != GL_NO_ERROR)
            m_iserror=true;
    }
}
GLException::GLException(const GLException &obj)
{
    //  str=obj.str;

}

const char * GLException::what() const throw()
{
    return str.str().c_str();
}

msgbase::~msgbase()
{
    std::for_each(m_arr_timerdata.begin(), m_arr_timerdata.end(), [](timerdata *&data)
    {
        delete data;
    });
}

void msgbase::SetTimer(Uint32 interval,Uint32 code)
{
    timerdata *mytimerdata = new timerdata;
    mytimerdata->cls_call = this;
    mytimerdata->interval = interval;
    mytimerdata->code=code;
    mytimerdata->timestamp=SDL_GetTicks();
    mytimerdata->timerid = SDL_AddTimer(interval, Timer_Callback, mytimerdata);
    m_arr_timerdata.push_back(mytimerdata);
    std::cout << "Timer ID : " << mytimerdata->timerid << " is set Inverval :" << interval << " ms."<<std::endl;
}

void msgbase::OnTimer(timerdata *data)
{
    std::cout << "Timer Id : " << data->timerid << " called Interval :" << data->interval <<std::endl;
}
