#pragma once
#include "Engine.h"
/* Declarations
----------------*/
class Window
{
  USSize pScreen;
  bool pIsOpen = true;
  bool pHasFocus = true;
  HWND pHWnd;
  Vector2 pTileSize;
  /* Event Listeners
  -------------------*/
  void onResize(unsigned short &width, unsigned short &height)
  {
    pScreen.width = width;
    pScreen.height = height;
    float screenProportion = (float)screen.width / screen.height;
    pTileSize.x = pTileSize.y / screenProportion;
  }
  void onFocus(bool hasFocus)
  {
    Window::pHasFocus = hasFocus;
  }
  /* Window Events
  -----------------*/
  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    if (message == WM_CREATE)
    {
      LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
      Window *pWindow = (Window *)pcs->lpCreateParams;

      ::SetWindowLongPtrW(
          hWnd,
          GWLP_USERDATA,
          PtrToUlong(pWindow));

      return 1;
    }

    Window *pWindow = reinterpret_cast<Window *>(
        ::GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_SETFOCUS:
    {
      pWindow->onFocus(true);
      return 0;
    }
    case WM_KILLFOCUS:
    {
      pWindow->onFocus(false);
      return 0;
    }
    case WM_SIZE:
    {
      unsigned short width = LOWORD(lParam);
      unsigned short height = HIWORD(lParam);
      pWindow->onResize(width, height);
      return 0;
    }
    case WM_NCACTIVATE:
    case WM_CAPTURECHANGED:
    {

      timer.reset();
      return 0;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 1;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

public:
  /* Window Getters
  ------------------*/
  const USSize &screen = pScreen;
  const bool &hasFocus = pHasFocus;
  const HWND &hWnd = pHWnd;
  const Vector2 &tileSize = pTileSize;
  /* Window Methods
  ------------------*/
  void close()
  {
    pIsOpen = false;
  }
  bool isOpen()
  {
    // Process Messages
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        return false;
    }
    return pIsOpen;
  }
  /* Constructor
  ---------------*/
  Window(const char *windowName, USSize screen, float tileSize) : pScreen(screen),
                                                                  pTileSize(tileSize * screen.height / screen.width, tileSize)
  {
    /* Window Inicialize
    ---------------------*/
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        Window::WindowProc,
        0,
        0,
        GetModuleHandle(NULL),
        0,
        LoadCursor(0, IDC_ARROW),
        0,
        0,
        _T(windowName),
        0};
    RegisterClassEx(&wc);
    RECT wr = {0, 0, screen.width, screen.height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    pHWnd = CreateWindowEx(
        0,                   // extended window style
        wc.lpszClassName,    // pointer to registered class name
        wc.lpszClassName,    // pointer to window name
        WS_OVERLAPPEDWINDOW, // window style
        CW_USEDEFAULT,       // horizontal position of window
        CW_USEDEFAULT,       // vertical position of window
        wr.right - wr.left,  // window width
        wr.bottom - wr.top,  // window height
        0,                   // handle to parent or owner window
        0,                   // handle to menu, or child-window identifier
        wc.hInstance,        // handle to application instance
        this                 // pointer to window-creation data
    );
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    graphics.Inicialize(pHWnd, screen);
  }
} window("2D-Renderer", USSize(800, 600), 0.2f);