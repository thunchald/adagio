/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: thunchald $
   $Notice: (C) Copyright 2014 by thunchald. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

static bool Running = true;

static HDC hDC; // GDI device context
static HGLRC hRC; // GL rendering context

GLvoid InitGL(GLsizei Width, GLsizei Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // This Will Clear The Background Color To Black
	glClearDepth(1.0);			// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);		// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);	// Enables Depth Testing

	glClearDepth(1.0);			// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);		// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);	// Enables Depth Testing

	glShadeModel(GL_SMOOTH);		// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);		// Select The Projection Matrix
	glLoadIdentity();			// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);		// Select The Modelview Matrix
}

GLvoid ResizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);	// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);		// Select The Projection Matrix
	glLoadIdentity();			// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
	glMatrixMode(GL_MODELVIEW);		// Select The Modelview Matrix
}

GLvoid DrawGLScene(GLvoid)
{
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f); // This Will Clear The Background Color To Black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();						// Reset The View
}

LRESULT CALLBACK Win32MainWindowCallback(HWND hWnd,
                                         UINT uMsg,
                                         WPARAM wParam,
                                         LPARAM lParam)
{
    LRESULT result = 0;
	RECT	Screen;					// Used Later On To Get The Size Of The Window
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=
        {
            sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
            1,                      // Version Number (?)
            PFD_DRAW_TO_WINDOW |	// Format Must Support Window
            PFD_SUPPORT_OPENGL |	// Format Must Support OpenGL
            PFD_DOUBLEBUFFER,		// Must Support Double Buffering
            PFD_TYPE_RGBA,			// Request An RGBA Format
            24,                     // Select A 16Bit Color Depth
            0, 0, 0, 0, 0, 0,		// Color Bits Ignored (?)
            0,                      // No Alpha Buffer
            0,                      // Shift Bit Ignored (?)
            0,                      // No Accumulation Buffer
            0, 0, 0, 0,				// Accumulation Bits Ignored (?)
            32,                     // 16Bit Z-Buffer (Depth Buffer)  
            0,                      // No Stencil Buffer
            0,                      // No Auxiliary Buffer (?)
            PFD_MAIN_PLANE,			// Main Drawing Layer
            0,                      // Reserved (?)
            0, 0, 0					// Layer Masks Ignored (?)
        };
    
    switch (uMsg) {

		case WM_CREATE:
			hDC = GetDC(hWnd);

            PixelFormat  = ChoosePixelFormat (hDC, &pfd);
            if (!PixelFormat) {
                MessageBox(0, "Can't find a suitable PixelFormat.", "Error", MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
                MessageBox(0, "Can't set the PixelFormat.", "Error", MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }
                    
            hRC = wglCreateContext(hDC);

            if(!hRC)
            {
                MessageBox(0,"Can't Create A GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if(!wglMakeCurrent(hDC, hRC))
            {
                MessageBox(0,"Can't activate GLRC.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            GetClientRect(hWnd, &Screen);
            InitGL(Screen.right, Screen.bottom);
            break;

        case WM_CLOSE:
        case WM_DESTROY:
            Running = false;
            wglMakeCurrent(hDC,NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd,hDC);

            PostQuitMessage(0);
            break;

        case WM_ACTIVATEAPP:
            OutputDebugString ("WM_ACTIVATEAPP\n");
            break;

        case WM_SIZE:
            OutputDebugString ("WM_SIZE\n");
            ResizeGLScene(LOWORD(lParam),HIWORD(lParam));
            break;

            // i/o
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_KEYDOWN:
        case WM_KEYUP:
        default:
            result = DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
    }
    
    return result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = Win32MainWindowCallback;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AdagioWindowClass";
    
    if (RegisterClass(&wc)) {
        HWND hWnd = CreateWindowEx(0,
                                   wc.lpszClassName,
                                   "Adagio",
                                   WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   640,
                                   480,
                                   0,
                                   0,
                                   hInstance,
                                   0);

        if (hWnd) {
            while (Running) {
                MSG msg;
                
                while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                DrawGLScene();								// Draw The Scene
                SwapBuffers(hDC);							// Swap Screen Buffers
                // if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);			// If ESC Is Pressed Quit
            }
        }
        else {OutputDebugString ("Failed to create window");}
    }
    else {OutputDebugString ("Failed to register window class");}
}
