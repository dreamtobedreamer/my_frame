#ifndef MYNUKLEAR_TEST_H
#define MYNUKLEAR_TEST_H
/**
 * 示例程序简述：
 *
 */
/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/demo/sdl_opengl3/nuklear_sdl_gl3.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the include
 * and the corresponding function. */
/*#include "../style.c"*/
/*#include "../calculator.c"*/
/*#include "../overview.c"*/
/*#include "../node_editor.c"*/

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/









class MyNuklearTest : public MyTask
{
public:
    MyNuklearTest()
        :MyTask()
    {
        printf("something const\n");
    }

    virtual ~MyNuklearTest()
    {
    }

    ///////////////////////////////////////////////// 可以重写的函数
    /**
     * 线程初始化函数，仅初始化调用一次，
     * note that: 需要调用父类中的方法
     */
    virtual void OnInit() override
    {
        printf("something init\n");
        /* SDL setup */
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
        SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        win = SDL_CreateWindow("Demo",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
        glContext = SDL_GL_CreateContext(win);
        SDL_GetWindowSize(win, &win_width, &win_height);

        /* OpenGL setup */
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glewExperimental = 1;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to setup GLEW\n");
            exit(1);
        }

        ctx = nk_sdl_init(win);
        /* Load Fonts: if none of these are loaded a default font will be used  */
        /* Load Cursor: if you uncomment cursor loading please hide the cursor */
        {struct nk_font_atlas *atlas;
            nk_sdl_font_stash_begin(&atlas);
            /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
            /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
            /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
            /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
            /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
            /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
            nk_sdl_font_stash_end();
            /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
            /*nk_style_set_font(ctx, &roboto->handle);*/}

        /* style.c */
        /*set_style(ctx, THEME_WHITE);*/
        /*set_style(ctx, THEME_RED);*/
        /*set_style(ctx, THEME_BLUE);*/
        /*set_style(ctx, THEME_DARK);*/

        background = nk_rgb(28,48,62);

        MyTask::OnInit();
        /**
         * 设置线程是帧循环的，而不是基于事件驱动的
         */
        SetLoop(true);
        /**
         * 设置该线程只处理指定自己ID的事件
         */
        SetSpecifledEv(true);
    }

    /**
     * 线程退出函数，仅退出调用一次，
     * note that: 需要调用父类中的方法
     */
    virtual void OnExit()
    {
        nk_sdl_shutdown();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        MyTask::OnExit();
    }

    /**
     * 每次线程收到事件便会调用此函数，如果设置线程为无限循环的(SetLoop)，
     * 则不管有没有事件该线程会一直调用此函数，
     * 所以需要在Update中加一点延时
     */
    virtual void Update(MyList *evs)
    {// delete MyEvent after used
        MyEvent* begin = (MyEvent*)evs->Begin();
        MyEvent* end = (MyEvent*)evs->End();
        MyEvent* me = NULL;
        for(;begin != end;)
        {
            me = (MyEvent*)begin->next;
            evs->Del(begin,false);

            // TODO...
            // TODO end

            delete begin;
            begin = me;
        }
        // TODO...

        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT)
            {
//                this->Quit();
//                MyApp::theApp->Quit();
//                MyApp::theApp->DelEvent(this);
                printf("SDL quit\n");
                exit(0);
            }
            nk_sdl_handle_event(&evt);
        } nk_input_end(ctx);

        /* GUI */
        if (nk_begin(ctx, "Demo1", nk_rect(50, 50, 200, 200),
                     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                     NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                printf("button pressed!\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 22, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_end(ctx);

        /* -------------- EXAMPLES ---------------- */
        /*calculator(ctx);*/
        /*overview(ctx);*/
        /*node_editor(ctx);*/
        /* ----------------------------------------- */

        /* Draw */
        {
            float bg[4];
            nk_color_fv(bg, background);
            SDL_GetWindowSize(win, &win_width, &win_height);
            glViewport(0, 0, win_width, win_height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg[0], bg[1], bg[2], bg[3]);
            /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
            SDL_GL_SwapWindow(win);
        }
        usleep(1000 * 16);
        // TODO end
    }


    /**
     * 入口函数
     */
    static void Test()
    {
        MyApp app(1);

        // provess task
        MyNuklearTest* tt = new MyNuklearTest();
        MyApp::theApp->AddEvent(tt);

        app.Exec();
    }

private:

    /* Platform */
    SDL_Window *win;
    SDL_GLContext glContext;
    struct nk_color background;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;
};

#endif // MYNUKLEAR_TEST_H
