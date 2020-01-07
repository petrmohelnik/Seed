#include "SDLWindow.h"
#include "Engine.h"

SDLWindow::SDLWindow() 
    : input(Engine::GetInput())
{
}

SDLWindow::~SDLWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLWindow::CreateWindow(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) 
	{
		throw std::runtime_error("Cannot initialize SDL: " + std::string(SDL_GetError()));
	}

	window = SDL_CreateWindow("Seed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window) 
	{
		throw std::runtime_error("Cannot create window: " + std::string(SDL_GetError()));
	}
}

void SDLWindow::InitializeOpenGL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	context = SDL_GL_CreateContext(window);
	if (!context) 
	{
		throw std::runtime_error("Cannot create OpenGL context: " + std::string(SDL_GetError()));
	}

	SDL_GL_SetSwapInterval(0); //vertical synchronization
    //SDL_SetRelativeMouseMode(SDL_TRUE);

	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) 
	{
		throw std::runtime_error("Could not initialize OpenGL, glew error: " + result);
	}

	if (GLEW_VERSION_1_1) 
	{
		std::cout << "----------------------------------------------------------------\n";
		std::cout << "Graphics Successfully Initialized\n";
		std::cout << "OpenGL Info\n";
		std::cout << "    Version: " << (GL_VERSION) << std::endl;
		std::cout << "     Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "   Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "    Shading: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "----------------------------------------------------------------\n";
	}
	else 
	{
		std::cout << "Unable to get any OpenGL version from GLEW!";
	}

    ImGui::CreateContext();
    imguiIO = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void SDLWindow::PollInputs()
{
    input.ClearInputs();

    input.InitializeState();
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
        ImGui_ImplSDL2_ProcessEvent(&event);
		input.AddInput(event);
	}

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Engine::GetWindow().window);
    ImGui::NewFrame();
}

double SDLWindow::GetTime()
{
    return (SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency());
}

void SDLWindow::Swap()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}

void SDLWindow::ResetViewport()
{
    auto windowSize = GetWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
}

glm::uvec2 SDLWindow::GetWindowSize()
{
    int width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return { width, height };
}
