#ifndef __WIN32_API_H
#define __WIN32_API_H

#define CS_VREDRAW         0x0001
#define CS_HREDRAW         0x0002
#define CS_DBLCLKS         0x0008
#define CS_OWNDC           0x0020
#define CS_CLASSDC         0x0040
#define CS_PARENTDC        0x0080
#define CS_NOCLOSE         0x0200
#define CS_SAVEBITS        0x0800
#define CS_BYTEALIGNCLIENT 0x1000
#define CS_BYTEALIGNWINDOW 0x2000
#define CS_GLOBALCLASS     0x4000
#define CW_USEDEFAULT      ((int)0x80000000)

#define PFD_TYPE_RGBA       0
#define PFD_TYPE_COLORINDEX 1

#define PFD_MAIN_PLANE     0
#define PFD_OVERLAY_PLANE  1
#define PFD_UNDERLAY_PLANE (-1)

#define PFD_DOUBLEBUFFER         0x00000001
#define PFD_STEREO               0x00000002
#define PFD_DRAW_TO_WINDOW       0x00000004
#define PFD_DRAW_TO_BITMAP       0x00000008
#define PFD_SUPPORT_GDI          0x00000010
#define PFD_SUPPORT_OPENGL       0x00000020
#define PFD_GENERIC_FORMAT       0x00000040
#define PFD_NEED_PALETTE         0x00000080
#define PFD_NEED_SYSTEM_PALETTE  0x00000100
#define PFD_SWAP_EXCHANGE        0x00000200
#define PFD_SWAP_COPY            0x00000400
#define PFD_SWAP_LAYER_BUFFERS   0x00000800
#define PFD_GENERIC_ACCELERATED  0x00001000
#define PFD_SUPPORT_DIRECTDRAW   0x00002000
#define PFD_DIRECT3D_ACCELERATED 0x00004000
#define PFD_SUPPORT_COMPOSITION  0x00008000

#define PFD_DEPTH_DONTCARE        0x20000000
#define PFD_DOUBLEBUFFER_DONTCARE 0x40000000
#define PFD_STEREO_DONTCARE       0x80000000

#define GWLP_WNDPROC    (-4)
#define GWLP_HINSTANCE  (-6)
#define GWLP_HWNDPARENT (-8)
#define GWLP_USERDATA   (-21)
#define GWLP_ID         (-12)

#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001
#define PM_NOYIELD  0x0002
/*
 * Window Messages
 */
#define RID_INPUT  0x10000003
#define RID_HEADER 0x10000005
#define WM_NULL    0x0000
#define WM_CREATE  0x0001
#define WM_DESTROY 0x0002
#define WM_MOVE    0x0003
#define WM_SIZE    0x0005
#define WM_INPUT   0x00FF

#define WM_ACTIVATE 0x0006
/*
 * WM_ACTIVATE state values
 */
#define WA_INACTIVE    0
#define WA_ACTIVE      1
#define WA_CLICKACTIVE 2

#define WM_SETFOCUS         0x0007
#define WM_KILLFOCUS        0x0008
#define WM_ENABLE           0x000A
#define WM_SETREDRAW        0x000B
#define WM_SETTEXT          0x000C
#define WM_GETTEXT          0x000D
#define WM_GETTEXTLENGTH    0x000E
#define WM_PAINT            0x000F
#define WM_CLOSE            0x0010
#define WM_MOUSEMOVE        0x0200
#define WM_QUIT             0x0012
#define WM_ERASEBKGND       0x0014
#define WM_SYSCOLORCHANGE   0x0015
#define WM_SHOWWINDOW       0x0018
#define WM_WININICHANGE     0x001A
#define WM_DEVMODECHANGE    0x001B
#define WM_ACTIVATEAPP      0x001C
#define WM_FONTCHANGE       0x001D
#define WM_TIMECHANGE       0x001E
#define WM_CANCELMODE       0x001F
#define WM_SETCURSOR        0x0020
#define WM_MOUSEACTIVATE    0x0021
#define WM_CHILDACTIVATE    0x0022
#define WM_QUEUESYNC        0x0023
#define WM_GETMINMAXINFO    0x0024
#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

#define LOWORD(l)        ((WORD)((DWORD)(l)))
#define HIWORD(l)        ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)        ((BYTE)(w))
#define HIBYTE(w)        ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/*
 * ShowWindow() Commands
 */
#define SW_HIDE            0
#define SW_SHOWNORMAL      1
#define SW_NORMAL          1
#define SW_SHOWMINIMIZED   2
#define SW_SHOWMAXIMIZED   3
#define SW_MAXIMIZE        3
#define SW_SHOWNOACTIVATE  4
#define SW_SHOW            5
#define SW_MINIMIZE        6
#define SW_SHOWMINNOACTIVE 7
#define SW_SHOWNA          8
#define SW_RESTORE         9
#define SW_SHOWDEFAULT     10
#define SW_FORCEMINIMIZE   11
#define SW_MAX             11

typedef void *HANDLE;
typedef void *HGLRC;
typedef HANDLE HDC;
typedef HANDLE HWND;

#define WINAPI __stdcall
#define WINUSERAPI
#define WINGDIAPI
#define CONST    const
#define CALLBACK __stdcall
typedef int(WINAPI *PROC)();
typedef int(WINAPI *FARPROC)();
typedef int(WINAPI *NEARPROC)();
typedef const char *LPCSTR;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef int BOOL;
typedef unsigned int UINT;
typedef float FLOAT;
typedef long LONG;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;
#if defined(_WIN64)
typedef __int64 LONG_PTR;
#else
typedef long LONG_PTR;
#endif
typedef void *LPVOID;
typedef LONG_PTR LRESULT;
typedef UINT *UINT_PTR;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef HANDLE HICON;
typedef HICON HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HMENU;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef WORD ATOM;
typedef void *HRAWINPUT;
typedef UINT *PUINT;

typedef LRESULT(CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSA
{
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCSTR lpszMenuName;
    LPCSTR lpszClassName;
} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;

typedef WNDCLASSA WNDCLASS;

HMODULE LoadLibraryA(LPCSTR lpLibFileName);
FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#define DefWindowProc DefWindowProcA

HMODULE GetModuleHandleA(LPCSTR lpModuleName);
#define GetModuleHandle GetModuleHandleA

ATOM RegisterClassA(const WNDCLASSA *lpWndClass);
#define RegisterClass RegisterClassA

#define CreateWindowA( \
    lpClassName,       \
    lpWindowName,      \
    dwStyle,           \
    x,                 \
    y,                 \
    nWidth,            \
    nHeight,           \
    hWndParent,        \
    hMenu,             \
    hInstance,         \
    lpParam)           \
    CreateWindowExA(   \
        0L,            \
        lpClassName,   \
        lpWindowName,  \
        dwStyle,       \
        x,             \
        y,             \
        nWidth,        \
        nHeight,       \
        hWndParent,    \
        hMenu,         \
        hInstance,     \
        lpParam)

WINUSERAPI
HWND WINAPI CreateWindowExA(
    DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
    HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
#define CreateWindowEx CreateWindowExA
#define CreateWindow   CreateWindowA

WINUSERAPI HDC WINAPI GetDC(HWND hWnd);

WINUSERAPI int WINAPI ReleaseDC(HWND hWnd, HDC hDC);

typedef struct tagPIXELFORMATDESCRIPTOR
{
    WORD nSize;
    WORD nVersion;
    DWORD dwFlags;
    BYTE iPixelType;
    BYTE cColorBits;
    BYTE cRedBits;
    BYTE cRedShift;
    BYTE cGreenBits;
    BYTE cGreenShift;
    BYTE cBlueBits;
    BYTE cBlueShift;
    BYTE cAlphaBits;
    BYTE cAlphaShift;
    BYTE cAccumBits;
    BYTE cAccumRedBits;
    BYTE cAccumGreenBits;
    BYTE cAccumBlueBits;
    BYTE cAccumAlphaBits;
    BYTE cDepthBits;
    BYTE cStencilBits;
    BYTE cAuxBuffers;
    BYTE iLayerType;
    BYTE bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

WINGDIAPI int WINAPI ChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
WINGDIAPI BOOL WINAPI SetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR *ppfd);

WINUSERAPI BOOL WINAPI DestroyWindow(HWND hWnd);

WINUSERAPI LONG_PTR WINAPI GetWindowLongPtrA(HWND hWnd, int nIndex);
#define GetWindowLongPtr GetWindowLongPtrA

void OutputDebugStringA(LPCSTR lpOutputString);
#define OutputDebugString OutputDebugStringA

void ExitProcess(UINT uExitCode);
LONG_PTR SetWindowLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
#define SetWindowLongPtr SetWindowLongPtrA
BOOL ShowWindow(HWND hWnd, int nCmdShow);

int DescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT, *PPOINT;

typedef struct tagMSG
{
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
    DWORD lPrivate;
} MSG, *PMSG, *NPMSG, *LPMSG;

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
#define PeekMessage PeekMessageA

BOOL TranslateMessage(const MSG *lpMsg);
LRESULT DispatchMessageA(const MSG *lpMsg);
#define DispatchMessage DispatchMessageA

BOOL SwapBuffers(HDC Arg1);

typedef struct tagRAWINPUTHEADER
{
    DWORD dwType;
    DWORD dwSize;
    HANDLE hDevice;
    WPARAM wParam;
} RAWINPUTHEADER, *PRAWINPUTHEADER, *LPRAWINPUTHEADER;

typedef struct tagRAWMOUSE
{
    USHORT usFlags;
    union
    {
        ULONG ulButtons;
        struct
        {
            USHORT usButtonFlags;
            USHORT usButtonData;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    ULONG ulRawButtons;
    LONG lLastX;
    LONG lLastY;
    ULONG ulExtraInformation;
} RAWMOUSE, *PRAWMOUSE, *LPRAWMOUSE;

typedef struct tagRAWKEYBOARD
{
    USHORT MakeCode;
    USHORT Flags;
    USHORT Reserved;
    USHORT VKey;
    UINT Message;
    ULONG ExtraInformation;
} RAWKEYBOARD, *PRAWKEYBOARD, *LPRAWKEYBOARD;

typedef struct tagRAWHID
{
    DWORD dwSizeHid;
    DWORD dwCount;
    BYTE bRawData[1];
} RAWHID, *PRAWHID, *LPRAWHID;

typedef struct tagRAWINPUT
{
    RAWINPUTHEADER header;
    union
    {
        RAWMOUSE mouse;
        RAWKEYBOARD keyboard;
        RAWHID hid;
    } data;
} RAWINPUT, *PRAWINPUT, *LPRAWINPUT;

UINT GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader);

#endif /* __WIN32_API_H */
