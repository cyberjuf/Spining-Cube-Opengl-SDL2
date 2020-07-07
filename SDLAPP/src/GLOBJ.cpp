#include "GLOBJ.h"
#include <glm/gtc/type_ptr.hpp>
GLOBJ::BufferData::BufferData(const void* data,unsigned int type,size_t buffer_size)
{
    m_size = buffer_size;
    m_type = type;
    m_index = glLoadArray(data);
}

GLOBJ::BufferData::BufferData(const BufferData &obj)
{
    m_size = obj.m_size;
    m_type = obj.m_type;
    m_index = obj.m_index;
}

GLOBJ::BufferData::~BufferData()
{
    glDeleteBuffers(1,&m_index);
}

unsigned int GLOBJ::BufferData::glLoadArray(const void* &vertex)
{
    unsigned int gpubuffer;
    glGenBuffers(1,&gpubuffer);
    glBindBuffer(m_type,gpubuffer);
    glBufferData(m_type,m_size,vertex,GL_STATIC_DRAW);
    glBindBuffer(m_type,0);
    return gpubuffer;
}

void GLOBJ::BufferData::Bind()const
{
    glBindBuffer(m_type,m_index);
}

void GLOBJ::BufferData::UnBind()const
{
    glBindBuffer(m_type,0);
}
//end BufferData

void GLOBJ::VertexBufferLayout::ApplyAttrib(GLOBJ::BufferData &vertex,unsigned int program)const
{
    vertex.Bind();
    unsigned long int offset = 0;
    for(unsigned int i=0; i<m_elements.size(); i++)
    {
        const auto &element = m_elements[i];
        unsigned int attrloca = glGetAttribLocation(program,element.attr_name.c_str());
        glEnableVertexAttribArray(attrloca);
        glVertexAttribPointer(i,element.count,element.type,element.normalized,m_Stride,(const void*)offset);
        offset += element.count*VertexBufferLayout::VertexBufferElement::GetSizeofType(element.type);
        glEnableVertexAttribArray(0);
    }
    vertex.UnBind();
}

GLOBJ::GLOBJ(BufferData *indices,const std::string &glProgramFile)
    :m_indices(indices)//,m_projection(glm::vec3(0.0f,0.0f,10.0f),60.0f,(float)width/height,0.1f,1000.0f)
{
    m_drawMode = DrawMode::DRAWELEMENT;
    glGenVertexArrays(1,&m_rendererID);
    Bind();
    ShaderProgramSouce source = ParseShader(glProgramFile);
    m_GL_program_index = CreateShaderProgram(source);
    UnBind();
}

GLOBJ::GLOBJ(unsigned int width,unsigned int height,const std::string &glProgramFile)
// :m_projection(glm::vec3(0,0,-3),70.0f,(float)width/height,0.01f,1000.0f)
{
    glGenVertexArrays(1,&m_rendererID);
    if(glProgramFile!="")
    {
        Bind();
        ShaderProgramSouce source = ParseShader(glProgramFile);
        m_GL_program_index = CreateShaderProgram(source);
        UnBind();
    }
}

GLOBJ::~GLOBJ()
{
    if(m_VertexBuffer!=nullptr)
        delete m_VertexBuffer;
    if(m_indices!=nullptr)
        delete m_indices;
    glDeleteProgram(m_GL_program_index);
    glDeleteVertexArrays(1,&m_rendererID);
}

void GLOBJ::AddBuffer(BufferData *vertex,const VertexBufferLayout &layout)
{
    Bind();
    if(m_VertexBuffer!=nullptr)
        delete m_VertexBuffer;
    m_VertexBuffer = (BufferData*)vertex;
    if(m_drawMode==DrawMode::NONE)
        m_drawMode=DrawMode::DRAWBUFFER;
    layout.ApplyAttrib(*vertex,m_GL_program_index);
    UnBind();
}

void GLOBJ::Bind()const
{
    glBindVertexArray(m_rendererID);
    glUseProgram(m_GL_program_index);
}

void GLOBJ::UnBind()const
{
    glBindVertexArray(0);
    glUseProgram(0);
}

GLOBJ::ShaderProgramSouce GLOBJ::ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath,std::ifstream::in);
    std::string line;
    std::stringstream ss[2];
    std::string errmsg = "Error Can not open \"" + filepath +"\" Shader file.!!!";
    if(stream.fail())
        throw InitError(errmsg);
    enum class ShaderType
    {
        NONE=-1,VERTEX=0,FRAGMENT=1
    };
    ShaderType type = ShaderType::NONE;
    while (getline(stream,line))
    {
        if(line.find("#shader")!= std::string::npos)
        {
            if(line.find("vertex")!=std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment")!=std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    stream.close();
    std::cout << "\nVertex Program " << std::endl;
    std::cout << ss[0].str() << std::endl;
    std::cout << "Fragment Program " << std::endl;
    std::cout << ss[1].str() << std::endl;
    return {ss[0].str(),ss[1].str()};
}

unsigned int GLOBJ::CreateShaderProgram(const ShaderProgramSouce &source)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,source.FragmentSource);
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

unsigned int GLOBJ::CompileShader(unsigned int type,const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if(result==GL_FALSE)
    {
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char *message = (char*) alloca(length*sizeof(char));
        glGetShaderInfoLog(id,length,&length,message);
        glDeleteShader(id);
        std::string errmsg = "Fail to Commpile ";
        errmsg += (type==GL_VERTEX_SHADER ? "vertex":"fragment");
        errmsg += "shader\n";
        errmsg += message;
        errmsg +=  "\n";
        throw InitError(errmsg);
    }
    std::cout <<"Compile " <<(type==GL_VERTEX_SHADER ? "vertex":"fragment") << "  shader OK!!."<< std::endl;
    return id;
}

int GLOBJ::glGetUniformLocation(const std::string &name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    int location = ::glGetUniformLocation(m_GL_program_index,name.c_str());
    if(location!=-1)
    {
        m_UniformLocationCache[name] = location;
        return location;
    }
    else
    {
        std::stringstream errmsg;
        errmsg << "Can not find uniform location :\"" << name << "\"." << std::endl;
        throw InitError(errmsg.str());
    }
}

void GLOBJ::glUniform4f(const std::string &u_name,const glm::vec4 &data)
{
    int location = glGetUniformLocation(u_name);
    glUseProgram(m_GL_program_index);
    ::glUniform4f(location,data.r,data.g,data.b,data.a);
}

void GLOBJ::glUniformMatrix4fv(const std::string &u_name,const glm::mat4 &data)
{
    int location = glGetUniformLocation(u_name);
    glUseProgram(m_GL_program_index);
    ::glUniformMatrix4fv(location,1,GL_FALSE, &data[0][0]);
}

void GLOBJ::glDrawElements(GLenum mode,GLsizei count,GLenum type)
{
    GLResetError();
    ::glDrawElements(mode,count,type,nullptr);
    GLException glerror(__PRETTY_FUNCTION__,__LINE__);
    if(glerror.IsError())
        throw glerror;
}

void GLOBJ::GLResetError()
{
    while(glGetError()!= GL_NO_ERROR);
}

void GLOBJ::drawIt(const GLenum mode,const GLenum type)
{
    Bind();
    glUniformMatrix4fv("tranMatrix",m_tranMatrix.GetModel());
    glUniformMatrix4fv("projection",(m_IsUseProjection?  m_projection.GetProjection():glm::mat4(1)));
    switch(m_drawMode)
    {
    case DrawMode::DRAWELEMENT :
    {
        m_indices->Bind();
        glDrawElements(mode,m_indices->GetSize(),type);
        m_indices->UnBind();
    }
    break;
    case DrawMode::DRAWBUFFER:
    {
        m_VertexBuffer->Bind();
        glDrawArrays(mode,0,(m_VertexBuffer->GetSize()/(sizeof(float)*2)));
        m_VertexBuffer->UnBind();
    }
    break;
    case DrawMode::NONE :
        throw GLException("There are notthing to draw.",__LINE__);
    }
    UnBind();
}
