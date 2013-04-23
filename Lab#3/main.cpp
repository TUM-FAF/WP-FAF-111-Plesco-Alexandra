#include <windows.h>
#include <windowsx.h>
#define ID_BUTTON 100
#define IDB_BEZIER 102
#define IDB_LINE 104
#define IDB_PEN 106
#define IDB_ELLIPSE 108
#define IDB_RECTANGLE 110
#define IDB_ERASER   112


POINT pointPen;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL PolyBezier(HDC hdc, CONST POINT *lppt, DWORD cPoints);

/*  Make the class name into a global variable  */
char szClassName[ ] = "labul 3";
HINSTANCE hInstance;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(145,145,145));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Paint version 1.0",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           900,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{   static HWND button, BezierTool, LineTool,Pen,EllipseTool,RectangleTool,EraserTool;
    int width;
    int xMouse, yMouse;
    HDC hdc = GetDC(hwnd);

    static POINT line, lineStart;
    static BOOL lineStarted;

    static BOOL ellipseStarted;
    static RECT ellipse;

    static BOOL rectangleStarted;
    static RECT rectangle;

    static int bezierStage = 0;
    static POINT bezierPoints[4];
    static POINT bezierPoint;

    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    HBITMAP hbmplogo = NULL;
    hbmplogo = (HBITMAP)LoadImage(hInstance, "paint.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmplogo, sizeof(bitmap), &bitmap);
	HDC hDC;
    PAINTSTRUCT Ps;
	HBRUSH  NewBrush;
	HPEN linePen;
	HPEN linePen2;
	HPEN linePen3;
	linePen= CreatePen(PS_SOLID,2,RGB(255,255,0));
	linePen2=CreatePen(PS_DASHDOTDOT,4,RGB(255,0,255));
	linePen3=CreatePen(PS_STYLE_MASK,6,RGB(0,128,255));
	SelectObject(hdc,linePen);

    POINT Pt[4] = { {  20,  12 }, {  88, 246 }, { 364, 192 }, { 250,  48 } };
    PolyBezierTo(hdc,Pt,4);
    switch (message)                  /* handle the messages */
    {
     case WM_CREATE:

          BezierTool = CreateWindowEx(
                0,
                "Button",
                "Bezier",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 250,
                180, 20,
                hwnd,
                (HMENU)IDB_BEZIER,
                hInstance,
                NULL);

                LineTool = CreateWindowEx(
                0,
                "Button",
                "Line",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 280,
                180, 20,
                hwnd,
                (HMENU)IDB_LINE,
                hInstance,
                NULL);
               Pen = CreateWindowEx(
                0,
                "Button",
                "Pen",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 310,
                180, 20,
                hwnd,
                (HMENU)IDB_PEN,
                hInstance,
                NULL);
                EllipseTool = CreateWindowEx(
                0,
                "Button",
                "Ellipse",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 340,
                180, 20,
                hwnd,
                (HMENU)IDB_ELLIPSE,
                hInstance,
                NULL);
                RectangleTool = CreateWindowEx(
                0,
                "Button",
                "Rectangle",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 370,
                180, 20,
                hwnd,
                (HMENU)IDB_RECTANGLE,
                hInstance,
                NULL);

                EraserTool = CreateWindowEx(
                0,
                "Button",
                "Eraser",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                30, 415,
                180, 20,
                hwnd,
                (HMENU)IDB_ERASER,
                hInstance,
                NULL);



          hbmplogo = (HBITMAP)LoadImage(hInstance, "paint.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            return 0;

      case WM_MOUSEMOVE:



            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);

             if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 )
            {
                if(wParam & MK_LBUTTON)
                {
                    if (Button_GetCheck(Pen) == BST_CHECKED)
                    {
                        width = 2;
                        linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, xMouse, yMouse, NULL);
                        LineTo(hdc, pointPen.x, pointPen.y);
                        DeleteObject(linePen);
                        pointPen.x = xMouse;
                        pointPen.y = yMouse;
                    }

                    if (Button_GetCheck(LineTool) == BST_CHECKED)
                    {
                        width = 2;

                        // draw previous line with white
                        linePen = CreatePen(PS_SOLID, width, RGB(255, 255, 255));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                        LineTo(hdc, line.x, line.y);
                        DeleteObject(linePen);


                        // draw the line
                        linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                        SelectObject(hdc, linePen);
                        MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                        LineTo(hdc, xMouse, yMouse);
                        DeleteObject(linePen);

                        line.x = xMouse;
                        line.y = yMouse;
                    }

                     // Eraser
                    if((wParam == MK_LBUTTON) && (Button_GetCheck(EraserTool) == BST_CHECKED))
                    {
                        width = 6;

                        rectangle.left = xMouse - (width / 2);
                        rectangle.right = xMouse + (width / 2);
                        rectangle.top = yMouse - (width / 2);
                        rectangle.bottom = yMouse + (width / 2);
                        InvalidateRect(hwnd, &rectangle, FALSE);
                        SendMessage(hwnd, WM_PAINT, 0, 0);
                        //ValidateRect(hwnd, &rect);
                    }
                }


            }
            break;

      case WM_LBUTTONUP :
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);
            if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 ) {
                if (Button_GetCheck(LineTool) == BST_CHECKED)
                {
                    width = 2;

                    // draw previous line with white

                    SelectObject(hdc, CreatePen(PS_SOLID, width, RGB(0, 0, 0)));
                    MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                    LineTo(hdc, line.x, line.y);


                    // draw the line
                    linePen = CreatePen(PS_SOLID, width, RGB(0, 0, 0));
                    SelectObject(hdc, linePen);
                    MoveToEx(hdc, lineStart.x, lineStart.y, NULL);
                    LineTo(hdc, xMouse, yMouse);
                    DeleteObject(linePen);

                    line.x = xMouse;
                    line.y = yMouse;
                }
                if(ellipseStarted)
                {
                    SelectObject(hdc, linePen2);
                    SelectObject(hdc, NewBrush);

                    Ellipse(hdc, ellipse.left, ellipse.top, xMouse, yMouse);
                    DeleteObject(linePen2);
                    DeleteObject(NewBrush);

                    ellipseStarted = false;
                }

                if(rectangleStarted)
                {
                    SelectObject(hdc, linePen);
                    SelectObject(hdc, NewBrush);
                    Rectangle(hdc, rectangle.left, rectangle.top, xMouse, yMouse);

                    DeleteObject(linePen);
                    DeleteObject(NewBrush);

                    rectangleStarted = false;
                }

                if(bezierStage == 1)
                {
                    bezierPoint.x = xMouse;
                    bezierPoint.y = yMouse;
                    bezierPoints[1] = bezierPoint;
                    bezierStage = 2;
                }

                if(bezierStage == 3)
                {
                    bezierPoint.x = xMouse;
                    bezierPoint.y = yMouse;
                    bezierPoints[3] = bezierPoint;
                    bezierStage = 0;
                    SelectObject(hdc, linePen3);
                    PolyBezier(hdc, bezierPoints, 4);
                    DeleteObject(linePen3);
                }
            }

        break;


        case WM_LBUTTONDOWN:
            xMouse = GET_X_LPARAM(lParam);
            yMouse = GET_Y_LPARAM(lParam);

            //250, 20, 880, 550
            if (xMouse > 250 && xMouse < 880 && yMouse > 20 && yMouse < 550 )
            {
                if(wParam == MK_LBUTTON)
                {
                    if (Button_GetCheck(Pen) == BST_CHECKED)
                    {
                        pointPen.x = xMouse;
                        pointPen.y = yMouse;
                    }
                    if (Button_GetCheck(LineTool) == BST_CHECKED)
                    {
                        lineStart.x = xMouse;
                        lineStart.y = yMouse;
                        line.x = xMouse;
                        line.y = yMouse;
                    }
                    if((wParam == MK_LBUTTON) && (Button_GetCheck(EllipseTool) == BST_CHECKED))
                    {
                        ellipse.left = xMouse;
                        ellipse.top = yMouse;
                        ellipseStarted = true;

                    }

                    if((wParam == MK_LBUTTON) && (Button_GetCheck(RectangleTool) == BST_CHECKED))
                    {
                        rectangle.left = xMouse;
                        rectangle.top = yMouse;
                        rectangleStarted = true;
                    }

                    if((wParam == MK_LBUTTON) && (Button_GetCheck(BezierTool) == BST_CHECKED))
                    {
                        if(bezierStage == 0)
                        {
                            bezierPoint.x = xMouse;
                            bezierPoint.y = yMouse;
                            bezierPoints[0] = bezierPoint;
                            bezierStage = 1;
                        }
                        else
                        {
                            bezierPoint.x = xMouse;
                            bezierPoint.y = yMouse;
                            bezierPoints[2] = bezierPoint;
                            bezierStage = 3;
                        }
                    }



                }
            }
            break;
    case WM_PAINT:




         hDC = BeginPaint(hwnd, &Ps);

        NewBrush = CreateSolidBrush(RGB(255, 255, 255));

        SelectObject(hDC, NewBrush);
        Rectangle(hDC, 250, 20, 880, 550);
        DeleteObject(NewBrush);
        //PolyBezier(hDC, Pt, 4);
        //EndPaint(hWnd, &Ps);


        // Draw logo
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmplogo);

            // Copy the bits from the memory DC into the current dc
            BitBlt(hdc,
                10,
                10,
                bitmap.bmWidth, bitmap.bmHeight,
                hdcMem, 0, 0, SRCCOPY);

            // Restore the old bitmap
            DeleteDC(hdcMem);

		EndPaint(hwnd, &Ps);

		break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
