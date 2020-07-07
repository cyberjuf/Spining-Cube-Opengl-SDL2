#include "../SDLAPP/include/SDL_Music.h"
#include"mywindow.h"

BEGIN_MESSAGE_MAP(mywindow,SDL)
    MSG_SDL_WINDOWEVENT_RESIZED,
    MSG_SDL_WINDOWEVENT_SIZE_CHANGED,
END_MESSAGE_MAP;

//unsigned int indices[]={0,1,2,2,1,3};
unsigned int indices[] =
{
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    12,13,14,
    14,15,12,
    //4, 5, 1,
    //1, 0, 4,
    // top
    8,9,10,
    10,11,8
};
mywindow::mywindow():SDL(SDL_INIT_EVERYTHING,"SDL OPENGL class test.!!!")//{
    //test GLOBJ
    ,myobj(new GLOBJ::BufferData(indices,GL_ELEMENT_ARRAY_BUFFER,sizeof(indices)),"colorshade.glsl")
    ,mywave(std::string("123.wav"))
{
    float objvector[] =
    {
        // front
        -1.0, -1.0,  1.0, 1.0f,0.0f,0.0f,
            1.0, -1.0,  1.0, 0.0f,1.0f,0.0f,
            1.0,  1.0,  1.0, 0.0f,0.0f,1.0f,
            -1.0,  1.0,  1.0, 1.0f,1.0f,0.0f,
            // back
            -1.0, -1.0, -1.0, 0.0f,1.0f,0.0f,
            1.0, -1.0, -1.0, 1.0f,0.0f,0.0f,
            1.0,  1.0, -1.0, 1.0f,1.0f,0.0f,
            -1.0,  1.0, -1.0, 0.0f,0.0f,1.0f,
            //top
            -0.999,  0.999,  0.999, 1.0f,1.0f,1.0f,
            0.999,  0.999,  0.999, 1.0f,1.0f,1.0f,
            0.999,  0.999, -0.999, 1.0f,1.0f,1.0f,
            -0.999,  0.999, -0.999, 1.0f,1.0f,1.0f,
            //bottom
            -0.999, -0.999, -0.999, 0.0f,0.0f,1.0f,
            0.999, -0.999, -0.999, 0.0f,0.0f,1.0f,
            0.999, -0.999,  0.999, 0.0f,0.0f,1.0f,
            -0.999, -0.999,  0.999, 0.0f,0.0f,1.0f,
        };

    GLOBJ::VertexBufferLayout mylayout("position",GLOBJ::BufferLayoutType::FLOAT,3);//x,y
    mylayout.AddLayout("VertColor",GLOBJ::BufferLayoutType::FLOAT,3);//r,g,b
    myobj.AddBuffer(new GLOBJ::BufferData(objvector,GL_ARRAY_BUFFER,sizeof(objvector))
                    ,mylayout);
    myobj.SetScale(glm::vec3(0.4,0.4,0.4));
    SetTimer(10);
}

mywindow::~mywindow()
{

}

void mywindow::render()
{
    myobj.drawIt(GL_TRIANGLES,GL_UNSIGNED_INT);
}



/*glm::mat4 projection(1);
glm::mat4 View       = glm::lookAt(
							glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
							glm::vec3(0,0,0), // and looks at the origin
							glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
					   );
glm::mat4 Model      = glm::mat4(1.0f);*/
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
void mywindow::ImGuiDraw()
{
    if(!m_menuopen) return;
    ImGui::Begin("Setting & Info : Press space to close|open menu",&m_menuopen);
    if (ImGui::BeginTabBar("##TabBar"))
    {
        if (ImGui::BeginTabItem("Config Tab"))
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::ColorEdit4("clear color", (float*)&m_clearColor);
            ImGui::SameLine();
            HelpMarker("Change Background Color");
            ImGui::Checkbox("Toggle Spine Object", &spin);
            ImGui::Separator();
            ImGui::Text("Slide to rescale Object.");
            ImGui::SliderFloat("Scale X", &(myobj.GetRefScale().x), 0.0f, 1.0f);
            ImGui::SliderFloat("Scale Y", &(myobj.GetRefScale().y), 0.0f, 1.0f);
            ImGui::SliderFloat("Scale Z", &(myobj.GetRefScale().z), 0.0f, 1.0f);
            ImGui::Separator();
            ImGui::SliderFloat("Position X", &(myobj.GetRefPosition().x), -1.0f, 1.0f);
            ImGui::SliderFloat("Position Y", &(myobj.GetRefPosition().y), -1.0f, 1.0f);
            ImGui::SliderFloat("Position Z", &(myobj.GetRefPosition().z), -1.0f, 1.0f);
            ImGui::Separator();
            static bool musicPause = false;
            if(ImGui::Button("Play/Pause wav"))
            {
                if(musicPause && !mywave.IsItPlay) musicPause = false;
                if(!musicPause)mywave.PlayIt();
                else mywave.PauseIt();
                musicPause = !musicPause;
            }
            ImGui::SameLine();
            HelpMarker("Click to Play or Pause some music.");
            ImGui::SameLine();
            if(ImGui::Button("Stop wav"))
            {
                musicPause = false;
                mywave.StopMusic();
            }
            ImGui::SameLine();
            ImGui::Checkbox("Repeat?", &mywave.IsRepeat);
            ImGui::SliderInt("Volumn Adjust",&mywave.m_volume,0,100,"Altitude = %d %%");
            ImGui::ProgressBar(mywave.GetCurrentPogress(), ImVec2(0.0f,0.0f));
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Progress Bar");
            if(ImGui::Button("Fullscreen"))
            {
                static bool fullscreen = false;
                if(!fullscreen) SDL_SetWindowFullscreen(m_window,SDL_WINDOW_FULLSCREEN);
                else  SDL_SetWindowFullscreen(m_window,0);
                fullscreen = !fullscreen;
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("BG/FG draw lists"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            ImGui::Checkbox("Draw in Background draw list", &draw_bg);
            ImGui::SameLine();
            HelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
            ImGui::Checkbox("Draw in Foreground draw list", &draw_fg);
            ImGui::SameLine();
            HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
            ImVec2 window_pos = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                ImGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10+4);
            if (draw_fg)
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0,10);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("ABOUT THIS DEMO:");
        ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
        ImGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
        ImGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
                          "and Metrics (general purpose Dear ImGui debugging tool).");
        ImGui::Separator();

        ImGui::Text("PROGRAMMER GUIDE:");
        ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        ImGui::BulletText("See comments in imgui.cpp.");
        ImGui::BulletText("See example applications in the examples/ folder.");
        ImGui::BulletText("Read the FAQ at http://www.dearimgui.org/faq/");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        ImGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        ImGui::Separator();

        ImGui::Text("USER GUIDE:");
        ImGui::ShowUserGuide();
    }
    ImGui::End();
}

void mywindow::OnWindowResized(SDL_WindowEvent*ev)
{
    SDL::OnWindowResized(ev);
}
void mywindow::OnWindowSizeChanged(SDL_WindowEvent*ev)
{
    SDL::OnWindowSizeChanged(ev);
    int width;
    int height;

    width = ev->data1;
    height = ev->data2;

}

void mywindow::OnTimer(timerdata *)
{
    if(spin)
    {
        glm::vec3 rotchange = myobj.GetRefRotation()+glm::vec3(0.005f,0.002f,0.003f);
        myobj.SetRotation(rotchange);
    }
}

void mywindow::Change_ZPos(float inc)
{
    glm::vec3 scalechange = myobj.GetRefScale()+glm::vec3(inc,inc,inc);
    myobj.SetScale(scalechange);
}
