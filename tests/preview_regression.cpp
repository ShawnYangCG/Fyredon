#include "Fyredon/Other/pch.h"
#include "Fyredon/Core/Application.h"
#include "Fyredon/Render/Texture.h"
#include "Fyredon/ECS/fbx_property_component.h"
#include <stdexcept>
namespace Fyredon {
struct PreviewRegression {
    static void require(bool value, const char* message) {
        if (!value) throw std::runtime_error(message);
    }
    static void run() {
        auto& app=EditorApplication::get_instance();
        app.init(1024,768,"Preview regression");
        app.tick_engine();app.tick_editor();
        app.menuAction->createDanceDemo();
        app.tick_engine();app.tick_editor();
        auto& renderer=Renderer::get_renderer();
        auto& state=static_cast<GUI_Menu*>(app.gui_org->gui_panels["Menu"])->menu_state;
        auto* dancer=app.ECSworld->getEntityByName("MC_dance");
        auto* animation=static_cast<fbx_property_component*>(dancer->getComponentByName("Animation"));
        for(int cycle=0;cycle<3;++cycle) {
            state.play=cycle==1;state.pause=cycle==2;app.tick_play();
            const bool wasPlaying=state.play!=0, wasPaused=state.pause!=0;
            int width=renderer.width,height=renderer.height;
            require(app.begin_preview(),"Preview creation failed");
            require(!glfwGetWindowAttrib(app.window,GLFW_VISIBLE),"Editor still visible");
            require(state.play && animation->isPlaying(),"Preview did not start Play");
            const int before=animation->currentFrameIndex;
            for(int frame=0;frame<4;++frame) {
                glfwSetTime(glfwGetTime()+0.1);
                app.tick_preview();
            }
            require(animation->currentFrameIndex!=before,"Animation did not advance");
            int pw,ph;glfwGetFramebufferSize(app.preview_window,&pw,&ph);
            require(renderer.width==pw && renderer.height==ph,"Wrong render resolution");
            glfwMakeContextCurrent(app.preview_window);
            while(glGetError()!=GL_NO_ERROR) {}
            glBindFramebuffer(GL_READ_FRAMEBUFFER,app.preview_read_fbo);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,
                renderer.get_colour_attachment()->ID,0);
            require(glCheckFramebufferStatus(GL_READ_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE,
                "Shared texture framebuffer incomplete");
            unsigned char pixel[4]={};
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glReadPixels(pw/2,ph/2,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);
            require(pixel[0]||pixel[1]||pixel[2],"Preview image empty");
            require(glGetError()==GL_NO_ERROR,"Preview OpenGL error");
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,0,0);
            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glfwMakeContextCurrent(app.window);
            glfwSetWindowShouldClose(app.preview_window,GLFW_TRUE);
            app.tick_preview();
            require(!app.preview_window,"Preview did not close");
            require(glfwGetWindowAttrib(app.window,GLFW_VISIBLE),"Editor not restored");
            require(!glfwWindowShouldClose(app.window),"Closing preview closed editor");
            require(renderer.width==width && renderer.height==height,"Editor resolution not restored");
            require((state.play!=0)==wasPlaying && (state.pause!=0)==wasPaused,"Playback state not restored");
            require(animation->isPlaying()==wasPlaying,"Animation state not restored");
            app.tick_engine();app.tick_editor();app.tick_play();
        }
        app.shutdown_gui();
        std::cout<<"PASS: three Preview cycles, Play/paused/stopped restoration, advancing dance animation, shared texture pixels, fullscreen resolution, editor return."<<std::endl;
    }
};
}
int main() {
    try { Fyredon::PreviewRegression::run();return 0; }
    catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
