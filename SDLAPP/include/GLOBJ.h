#ifndef GLOBJ_H
#define GLOBJ_H
#include "appbase.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
class GLOBJ
{
public:
//#BEGIN STRUCT ShaderProgramSource
    struct ShaderProgramSouce
    {
        std::string VertexSource;
        std::string FragmentSource;
    };
//#END STRUCT GLOBJ::ShaderProgramSource

    enum BufferLayoutType
    {
        FLOAT,UINT,UCHAR
    };

//#BEGIN CLASS GLOBJ::BufferDAta
    class BufferData
    {
    public:
        BufferData()=delete;
        BufferData(const void *data,unsigned int type,size_t buffer_size);
        BufferData(const BufferData &obj);
        virtual ~BufferData();
        virtual void Bind()const;
        virtual void UnBind()const;
        size_t GetSize()const
        {
            return m_size;
        }
        unsigned int GetIndex()const
        {
            return m_index;
        }
    protected:
        unsigned int m_index,m_type;
        size_t m_size;
        unsigned int glLoadArray(const void* &vertex);
    };
//#END CLASS GLOBJ::BufferDAta

//#BEGIN CLASS GLOBJ::VertexBufferLayout
    class VertexBufferLayout
    {
    public:
        struct VertexBufferElement
        {
            std::string attr_name;
            unsigned int type;
            unsigned int count;
            unsigned char normalized;
            static unsigned int GetSizeofType(unsigned int type)
            {
                switch(type)
                {
                case  GL_FLOAT :
                    return 4;
                case  GL_UNSIGNED_INT :
                    return 4;
                case  GL_UNSIGNED_BYTE :
                    return 1;
                }
                return 0;
            }
        };
        VertexBufferLayout()=delete;
        VertexBufferLayout(std::string attr_name,BufferLayoutType t_layout,unsigned int count):m_Stride(0)
        {
            AddLayout(attr_name,t_layout,count);
        }
        void ApplyAttrib(GLOBJ::BufferData &vertex,unsigned int program)const;
        void AddLayout(std::string attr_name,BufferLayoutType t_layout,unsigned int count)
        {
            switch(t_layout)
            {
            case BufferLayoutType::FLOAT :
                Pushf(attr_name,count);
                break;
            case BufferLayoutType::UINT  :
                Pushui(attr_name,count);
                break;
            case BufferLayoutType::UCHAR :
                Pushuchar(attr_name,count);
                break;
            }
        }
        inline unsigned int GetStride()const
        {
            return m_Stride;
        }
    private:
        std::vector<VertexBufferElement> m_elements;
        unsigned int m_Stride;
        void Pushf(std::string attr_name,unsigned int count)
        {
            m_elements.push_back({attr_name,GL_FLOAT,count,GL_FALSE});
            m_Stride += count*VertexBufferLayout::VertexBufferElement::GetSizeofType(GL_FLOAT);
        }

        void Pushui(std::string attr_name,unsigned int count)
        {
            m_elements.push_back({attr_name,GL_UNSIGNED_INT,count,GL_FALSE});
            m_Stride += count*VertexBufferLayout::VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT);
        }

        void Pushuchar(std::string attr_name,unsigned int count)
        {
            m_elements.push_back({attr_name,GL_UNSIGNED_BYTE,count,GL_TRUE});
            m_Stride += count*VertexBufferLayout::VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE);
        }

    };
//#END CLASS GLOBJ::VertexBufferLayout
//#BEGIN CLASS GLOBJ::GLTranform
    class GLTranform
    {
    public:
        GLTranform(const glm::vec3 &pos= glm::vec3(),const glm::vec3 &rot= glm::vec3(),const glm::vec3 &scale=glm::vec3(1.0f))
            :m_pos(pos),m_rot(rot),m_scale(scale)
        {

        }

        inline glm::mat4 GetModel()
        {
            glm::mat4 posMatrix = glm::translate(glm::mat4(1.0),m_pos);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f),m_scale);
            glm::mat4 xRotMatrix = glm::rotate(posMatrix,m_rot.x,glm::vec3(1,0,0));
            glm::mat4 yRotMatrix = glm::rotate(posMatrix,m_rot.y,glm::vec3(0,1,0));
            glm::mat4 zRotMatrix = glm::rotate(posMatrix,m_rot.z,glm::vec3(0,0,1));
            glm::mat4 RotMatrix = xRotMatrix*yRotMatrix*zRotMatrix;
            return posMatrix*RotMatrix*scaleMatrix;
        }

        inline glm::vec3& GetPos()
        {
            return m_pos;
        }
        inline glm::vec3& GetRotage()
        {
            return m_rot;
        }
        inline glm::vec3& GetScale()
        {
            return m_scale;
        }
        inline void SetPos(const glm::vec3&pos)
        {
            m_pos=pos;
        }
        inline void SetRotage(const glm::vec3&rot)
        {
            m_rot=rot;
        }
        inline void SetScale(const glm::vec3&scale)
        {
            m_scale=scale;
        }
    protected:
        glm::vec3 m_pos,m_rot,m_scale;

    };
//#END class GLTranform
//#BEGIN GLOBJ::GLProjection Class
    class GLProjection
    {
    public:
        GLProjection()
        {

        }
        ~GLProjection() {}
        virtual void Init(const glm::vec3& pos,float fov,float aspect,float zNear,float zFar)
        {
            m_projection = glm::perspective(fov,aspect,zNear,zFar);
            m_position = pos;
            m_forward = glm::vec3(0,0,1);
            m_up = glm::vec3(0,1,0);
        }
        glm::mat4 GetProjection()const
        {
            return glm::lookAt(m_position,m_position+m_forward,m_up)*m_projection;
        }
    protected:
        glm::mat4 m_projection;
        glm::vec3 m_position,m_forward,m_up;
    };
//#END GLprojection class
//GLOBJ Continue
    enum DrawMode {NONE=-1,DRAWBUFFER,DRAWELEMENT };
    GLOBJ(unsigned int width,unsigned int height,const std::string &glProgramFile="");
    GLOBJ(BufferData *indices,const std::string &glProgramFile);
    virtual ~GLOBJ();
    unsigned int GetRendererID()
    {
        return m_rendererID;
    }
    void Bind()const;
    void UnBind()const;
    glm::vec3& GetRefPosition()
    {
        return m_tranMatrix.GetPos();
    }
    void SetPosition(glm::vec3 data)
    {
        m_tranMatrix.GetPos()=data;
    }
    const glm::vec3 GetRefRotation()
    {
        return (const glm::vec3)m_tranMatrix.GetRotage();
    }
    void SetRotation(glm::vec3 data)
    {
        m_tranMatrix.GetRotage()=data;
    }
    glm::vec3& GetRefScale()
    {
        return m_tranMatrix.GetScale();
    }
    void SetScale(glm::vec3 data)
    {
        m_tranMatrix.GetScale()=data;
    }
    /*  glm::mat4& ApplyProjection(const glm::mat4 &projection)
      {
          m_projection=projection;
      }*/
    void AddBuffer(BufferData *vertex,const VertexBufferLayout &layout);
    void glUniform4f(const std::string &u_name,const glm::vec4 &data);
    void glUniformMatrix4fv(const std::string &u_name,const glm::mat4 &data);
    void SetProgram(unsigned int programID)
    {
        m_GL_program_index=programID;
    }
    unsigned int GetProgramID()const
    {
        return m_GL_program_index;
    }
    void SetProgramByID(unsigned int ProgramID)
    {
        m_GL_program_index=ProgramID;
    }
    virtual void drawIt(const GLenum mode,const GLenum type=0);
    void SetUseProjection(bool data = false)
    {
        m_IsUseProjection = data;
    }
protected:
    BufferData *m_indices=nullptr,*m_VertexBuffer=nullptr;
    void GLResetError();
    ShaderProgramSouce ParseShader(const std::string &filepath);
    unsigned int CreateShaderProgram(const ShaderProgramSouce &source);
    unsigned int CompileShader(unsigned int type,const std::string& source);
    void glDrawElements(GLenum mode,GLsizei count,GLenum type=GL_NONE);
    int glGetUniformLocation(const std::string &name);
private:
    unsigned int m_GL_program_index=0,m_rendererID;
    DrawMode m_drawMode = DrawMode::NONE;
    GLOBJ::GLTranform m_tranMatrix;
    GLOBJ::GLProjection m_projection;
    bool m_IsUseProjection = false;
    std::unordered_map<std::string,unsigned int> m_UniformLocationCache;
};
//END GLOBJ class
//#BEGIN GLLine class
template<typename Tname>
class GLLine:public GLOBJ
{
public:
    GLLine():GLOBJ("basic.glsl") {}
    GLLine(const std::vector<Tname> &vecData):GLOBJ("basic.glsl")
    {
        AddDAta(vecData);
    }
    void AddDAta(const std::vector<Tname> &vecData)
    {
        auto it = std::max_element(std::begin(vecData), std::end(vecData));
        m_max = *it;
        it = std::min_element(std::begin(vecData), std::end(vecData));
        m_min = *it;
        m_delta = m_max-m_min;
        std::for_each(vecData.begin(),vecData.end(),[&](const Tname &data)
        {
            float tdata = (float)(data-m_min)/m_delta;
            tdata = (tdata*(2.0f-(2.0f*m_trim/100)))-(2.0f-(2.0f*m_trim/100))/2;//Y scale calculate (tdata*2.0f)-1.0f;
            m_glyVec.push_back(tdata);
        });
        float xspeade = 2.0f/(m_glyVec.size()-1);
        float xbegin = -1.0f;
        unsigned int index = 0;
        float oldData,oldX;
        std::for_each(m_glyVec.begin(),m_glyVec.end(),[&](const float &data)
        {

            if(index%2==0&&index!=0)
            {
                m_glxyVec.push_back(oldX);
                m_glxyVec.push_back(oldData);
                m_glxyVec.push_back(xbegin);
                m_glxyVec.push_back(data);
                m_glxyVec.push_back(xbegin);
                m_glxyVec.push_back(data);
            }
            else
            {
                m_glxyVec.push_back(xbegin);
                m_glxyVec.push_back(data);
            }
            oldData=data;
            oldX=xbegin;
            xbegin += xspeade;
            index++;
        });
        AddBuffer(new GLOBJ::BufferData(&m_glxyVec[0],GL_ARRAY_BUFFER,m_glxyVec.size()*sizeof(float))
                  ,GLOBJ::VertexBufferLayout(GLOBJ::BufferLayoutType::FLOAT,2));

    }
    virtual void drawIt()
    {
        // GLOBJ::drawIt( GL_LINES_ADJACENCY);
        GLOBJ::drawIt( GL_LINES);
    }
    void SetTrim(unsigned int ptrim)
    {
        if(ptrim>100)
            ptrim = 100;
        m_trim=ptrim;
    }
private:
    std::vector<float> m_glyVec,m_glxyVec;
    Tname m_min,m_max,m_delta;
    unsigned int m_trim = 0;
};
//#END GLLine class
#endif // GLOBJ_H
