#pragma warning(disable : 4244) //
#pragma warning(disable : 4083)     //
#pragma warning(disable : 4305)         //
#pragma warning(disable : 4715)             // Just a harmless warnings
#pragma warning(disable : 4101)         //  
#pragma warning(disable : 4267)     //
#pragma warning(disable : 4996) //

#define USE_WELCOME_WINDOW false

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#include <stdint.h>
#include <windows.h>
#include <shobjidl.h>
#include <tchar.h>

#include <stdlib.h>
#include <string>

#include <sstream>
#include <shlobj.h>
#include <sstream>
#include <locale>
#include <algorithm>
#include <codecvt>

#include <cstdlib> //std::system(const char*)
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <cmath>
#include <string>

using namespace std;

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 600;


bool Use_Darkmode = false;

const wchar_t* GUI_TITLE = L"Empower Smart Deploy";
//===================================Loaded Icons/images==========================================
wchar_t* GUI_ICON               = L"../../../Utils/images/esd_logo_blue_N78_icon.ico";
const char* welcome_image_path  = "../../../Utils/images/empower logo.jpg"; 
const char* main_image_path     = "../../../Utils/images/ESD_LOGO_BLUE.jpg"; 

//========================================Fonts===================================================
const char* Title_Font1      = "../../../Utils/Fonts/LeagueGothic-Regular.otf";       const unsigned int Title_Font_pxlSize     = 88; 
const char* Title_Font2      = "../../../Utils/Fonts/LeagueGothic-Regular.otf";       const unsigned int Title_Font_pxlSize2    = 55; 

const char* Button_Font     = "../../../Utils/Fonts/SourceSansPro-Semibold.otf";      const unsigned int Button_Font_pxlSize    = 24;    
const char* Text_Font_bld   = "../../../Utils/Fonts/SourceSansPro-SemiboldIt.otf";    const unsigned int Text_Bold_Font_pxlSize = 18;    
const char* Text_Font       = "../../../Utils/Fonts/SourceSansPro-It.otf";            const unsigned int Text_Font_pxlSize      = 14;

ImFont* ImGui_Title_Font    = nullptr;
ImFont* ImGui_Title_Font2    = nullptr;
ImFont* ImGui_Button_Font   = nullptr;
ImFont* ImGui_Text_Font_bld = nullptr;
ImFont* ImGui_Text_Font     = nullptr;

//============================Buttons Names and Batch File Variables=================================
const char* CSR_batchFile = "..\\..\\..\\Utils\\batch_files\\sample.bat";                                
const char* PSE_batchFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  
const char* INO_batchFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  
const char* IGC_BatchFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  
const char* GVF_bathcFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  
const char* DLE_batchFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  
const char* CFU_batchFile = "..\\..\\..\\Utils\\batch_files\\<add the name of the batch file in here>";  

const char* Button1 = "Check System for Requirements";
const char* Button2 = "Prep System for Empower";
const char* Button3 = "Install .Net 3.5 Offline";
const char* Button4 = "Install Google Chrome";
const char* Button5 = "Get Verify Files";
const char* Button6 = "Download Empower";
const char* Button7 = "Check for Updates";
const char* Exit_Button = "Exit";                                                                                        

static std::string selected_bat_file; 
//===================================welcoming page control=========================================
#if USE_WELCOME_WINDOW
    bool is_in_welcomePage = true;
    std::string installation_path = "C:\\"; //you can use this to retrieve the specified path.

    const char* Welcome_Title_Header = "Waters";
    const char* Welcom_Title_Subheader = "THE SCIENCE OF WHAT'S POSSIBLE";
#endif
//=======================================IMAGE LOADING DATA=============================================
static int my_image_width = 0;
static int my_image_height = 0;
static ID3D11ShaderResourceView* my_texture = NULL;
//==========================================Data===================================================
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
//======================================Graphics Handlers=========================================
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
//========================================Helper Functions========================================
static inline void ControlPanel();
static inline void ButtonSpacerPadding(float x, float y);
 
static inline void WelcomePage(HWND hwnd);
static inline void PageControllerpanel(float width = 0);
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

//================================================================================================
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    WNDCLASSEXW wc = { };
    wc.cbSize           = sizeof(wc);
    wc.style            = CS_CLASSDC;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0L;
    wc.cbWndExtra       = 0L;
    wc.hInstance        = hInstance;
    wc.hIcon            = (HICON) LoadImage(hInstance, GUI_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
    wc.hCursor          = nullptr;
    wc.hbrBackground    = nullptr;
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = GUI_TITLE;
    wc.hIconSm          = LoadIcon(NULL, GUI_ICON );

    ::RegisterClassExW(&wc);

    HWND hwnd = ::CreateWindowW(
        wc.lpszClassName, 
        GUI_TITLE, 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
        100, 
        100, 
        WIDTH, 
        HEIGHT, 
        nullptr, 
        nullptr, 
        wc.hInstance, 
        nullptr
    );

     // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return 1;
        }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.IniFilename = NULL;  //prevents from saving the last states and position of each widgets

    if(Use_Darkmode)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.3f;
        style.Colors[ImGuiCol_WindowBg].w = 1.3f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImGui_Button_Font   = io.Fonts->AddFontFromFileTTF(Button_Font  , Button_Font_pxlSize   ); //DEFAULT FONT
    ImGui_Title_Font    = io.Fonts->AddFontFromFileTTF(Title_Font1  , Title_Font_pxlSize    ); 
    ImGui_Title_Font2   = io.Fonts->AddFontFromFileTTF(Title_Font2  , Title_Font_pxlSize2   );
    ImGui_Text_Font_bld = io.Fonts->AddFontFromFileTTF(Text_Font_bld, Text_Bold_Font_pxlSize);
    ImGui_Text_Font     = io.Fonts->AddFontFromFileTTF(Text_Font    , Text_Font_pxlSize     );
//==============================================LOAD IMAGE HERE============================================================
    bool ret = LoadTextureFromFile(main_image_path, &my_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
//=========================================================================================================================
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

#if USE_WELCOME_WINDOW
        if(!is_in_welcomePage)
#endif
        {
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace", nullptr, window_flags);
            ImGui::PopStyleVar();
            ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags | ImGuiDockNodeFlags_NoWindowMenuButton);

                static auto first_time = true;
                if (first_time)
                {
                    first_time = false;

                    ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                    ImGui::DockBuilderSetNodeSize(dockspace_id, dockspace_size);

                    // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                    //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                    //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                    ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.3f, nullptr, &dockspace_id);
                    ImGuiID dock_id_left = dockspace_id;

                    // we now dock our windows into the docking node we made above
                    ImGui::DockBuilderDockWindow("CMD viewport", dock_id_right);
                    ImGui::DockBuilderDockWindow("Controll Panel", dock_id_left);
                    ImGui::DockBuilderFinish(dockspace_id);
                }
            }

            ImGui::End();

            ImGui::Begin("CMD viewport");
            {   
                 struct Funcs
                {
                    static int MyResizeCallback(ImGuiInputTextCallbackData* data)
                    {
                        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                        {
                            ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                            IM_ASSERT(my_str->begin() == data->Buf);
                            my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                            data->Buf = my_str->begin();
                        }
                        return 0;
                    }

                    // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
                    // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
                    static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
                    {
                        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                        return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
                    }
                };


                static std::string bat_output;
                
                ImVector<char> my_str;
                my_str.resize(bat_output.length() + 1);
                strcpy(my_str.Data, bat_output.c_str());

                if (my_str.empty())
                    my_str.push_back(0);
                Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 48));

                if(!selected_bat_file.empty())
                {      
                    char buffer[255]; 
                    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
                    HANDLE stdout_rd, stdout_wr;
                    
                    if (CreatePipe(&stdout_rd, &stdout_wr, &sa, 0)) {
                        STARTUPINFO si = {sizeof(STARTUPINFO)};
                        PROCESS_INFORMATION pi;

                        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
                        si.wShowWindow = SW_HIDE;
                        si.hStdOutput = stdout_wr;
                        
                        std::wstring wideStr;
                        try {
                            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                            wideStr = converter.from_bytes(selected_bat_file);
                        }
                        catch (const std::exception& e) {
                            std::stringstream ss;
                            ss << "Error converting string: " << e.what();
                            bat_output = ss.str();
                            return 1; // Handle the conversion error
                        }
                       
                        if (CreateProcess(NULL,  &wideStr[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
                            CloseHandle(stdout_wr);
                            char buffer[4096];
                            DWORD bytesRead;

                            bat_output.clear();
                            while (ReadFile(stdout_rd, buffer, sizeof(buffer), &bytesRead, NULL) != 0 && bytesRead != 0) {
                                bat_output.append(buffer, bytesRead);
                            }

                            CloseHandle(stdout_rd);
                            CloseHandle(pi.hProcess);
                            CloseHandle(pi.hThread);
                        }
                        else {
                            bat_output = "Error executing batch file." ;
                        }
                        selected_bat_file.clear();
                    }
                    else {
                         bat_output = "Error creating pipe.";
                    }

                }   
            }
            ImGui::End();

            ImGuiWindowClass window_class;
            window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe; 
            ImGui::SetNextWindowClass(&window_class);
            ImGui::Begin("Controll Panel", nullptr);
            {
                ControlPanel();
            }
            ImGui::End();
        }

#if USE_WELCOME_WINDOW
        else
            WelcomePage(hwnd);
#endif
        //render frontend window system
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    delete ImGui_Title_Font   ;
    delete ImGui_Title_Font2  ;
    delete ImGui_Button_Font  ;
    delete ImGui_Text_Font_bld;
    delete ImGui_Text_Font    ;

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

inline void ControlPanel()
{
    const float pan_col1 = 50;
#if USE_WELCOME_WINDOW
    const float pan_row  = 7;
#else
    const float pan_row  = 15;
#endif
    const ImVec2 ButtonSize =ImVec2(pan_col1 + 228.57, 50); 

    ImGui::Columns(2, "MyColumns", false);
    ButtonSpacerPadding(pan_col1,pan_row);

    if(ImGui::Button(Button1, ButtonSize))
    {
        selected_bat_file = CSR_batchFile;
    }

    ButtonSpacerPadding(pan_col1,pan_row);
    if(ImGui::Button(Button2, ButtonSize))
    {
        selected_bat_file = PSE_batchFile;
    }

    ButtonSpacerPadding(pan_col1,pan_row);
    if(ImGui::Button(Button3, ButtonSize))
    {
        selected_bat_file = INO_batchFile;
    }

    ButtonSpacerPadding(pan_col1,pan_row);
    if(ImGui::Button(Button4, ButtonSize))
    {
        selected_bat_file = IGC_BatchFile;
    }

    ButtonSpacerPadding(pan_col1,pan_row);
    if(ImGui::Button(Button5, ButtonSize))
    {
        selected_bat_file = GVF_bathcFile;
    }

    //download
    ButtonSpacerPadding(pan_col1,pan_row);
    if(ImGui::Button(Button6, ButtonSize))
    {
        selected_bat_file = DLE_batchFile;
    }

    ImGui::NextColumn();
        const float pan_col2 = 50;
        ButtonSpacerPadding(pan_col2 + 2, 15);
        ImGui::Image((void*)my_texture, ImVec2(pan_col1 + 228.57, my_image_width * 0.74), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), Use_Darkmode? ImVec4(255, 255, 255, 255) : ImVec4(0, 0, 0, 255));
#if USE_WELCOME_WINDOW
        ButtonSpacerPadding(pan_col2, 31);
#else
        ButtonSpacerPadding(pan_col2, pan_row + 43);
#endif
        if(ImGui::Button(Button7, ButtonSize))
        {
            selected_bat_file = CFU_batchFile;
        }
        ButtonSpacerPadding(pan_col2, pan_row);
        if(ImGui::Button(Exit_Button, ButtonSize))
        {
            ::PostQuitMessage(0);
        }
    
    ImGui::Columns(1);

#if USE_WELCOME_WINDOW
    PageControllerpanel(pan_col2);
#endif
}   

void ButtonSpacerPadding(float x, float y)
{   
    ImGui::Dummy(ImVec2(0, y));
    ImGui::Dummy(ImVec2(x, 0));
    ImGui::SameLine();
}

//==============================================Folder Dialog================================================================
#if USE_WELCOME_WINDOW
    static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
    {

        if(uMsg == BFFM_INITIALIZED)
        {
            std::string tmp = (const char *) lpData;
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }

        return 0;
    }

    std::string BrowseFolder(std::string saved_path)
    {
        TCHAR path[MAX_PATH];

        const char * path_param = saved_path.c_str();

        BROWSEINFO bi = { 0 };
        bi.lpszTitle  = ("Browse for folder...");
        bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.lpfn       = BrowseCallbackProc;
        bi.lParam     = (LPARAM) path_param;

        LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

        if ( pidl != 0 )
        {
            //get the name of the folder and put it in path
            SHGetPathFromIDList ( pidl, path );

            //free memory used
            IMalloc * imalloc = 0;
            if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
            {
                imalloc->Free ( pidl );
                imalloc->Release ( );
            }

            return path;
        }

        return "";
    }
//===========================================================================================================================

    void WelcomePage(HWND hwnd)
    {   
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::Begin("Welcome Page", nullptr, flags);
        {   
            ImGui::Columns(2, "FrontColmns", false);
            {
                int image_width = 0;
                int image_height = 0;
                GLuint image_texture = 0;

                bool load_success = LoadTextureFromFile(welcome_image_path, &image_texture, &image_width, &image_height);
                IM_ASSERT(load_success);

                ImGui::Image((void*)(intptr_t)image_texture, ImVec2(ImGui::GetColumnWidth() - 40, HEIGHT - 40));

                //ImGui::SetColumnWidth(-1, image_width + 140);
            }
            ImGui::NextColumn();
            {
                const float center = ImGui::GetColumnWidth() / 2;

                ImGui::Indent(center - 80);
                ImGui::PushFont(ImGui_Title_Font);
                    ImGui::Text(Welcome_Title_Header);
                ImGui::PopFont();

                ImGui::Unindent(147);
                ImGui::PushFont(ImGui_Title_Font2);
                     ImGui::Text(Welcom_Title_Subheader);
                ImGui::PopFont();

                ButtonSpacerPadding(0,100);
                //ImGui::Unindent(2);

                ImGui::InputText("##label", &installation_path, ImGuiInputTextFlags_None); 
                ImGui::SameLine();
                if(ImGui::Button("browse"))
                    installation_path = BrowseFolder(installation_path);

                PageControllerpanel(ImGui::GetColumnWidth());
            }
            ImGui::Columns(1);
        }
        ImGui::End();
    }

    void PageControllerpanel(float width)
    {   
        const ImVec2 ButtonSize =ImVec2(110, 40); 

        ButtonSpacerPadding(0, is_in_welcomePage? 170 : 30);
        ImGui::Separator();

        ButtonSpacerPadding(0, 15);
        if(is_in_welcomePage)
        {
            if(ImGui::Button("Cancel",ButtonSize))
                ::PostQuitMessage(0);
        }   
        const float seperation = ( is_in_welcomePage ? (width * 0.284) + 100 : (ImGui::GetWindowWidth()  * 0.584) + width);
        ImGui::BeginDisabled(is_in_welcomePage);
            ImGui::SameLine(seperation); 
            if(ImGui::Button("Previous",ButtonSize))
            {
                is_in_welcomePage = true;
            }   
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!is_in_welcomePage);
            ImGui::SameLine(seperation + 122); 
            if(ImGui::Button("Next",ButtonSize))
            {
                is_in_welcomePage = false;
            }
        ImGui::EndDisabled();

        ImGui::Columns(1);
    }
#endif

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}