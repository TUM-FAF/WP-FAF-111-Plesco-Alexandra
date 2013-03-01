#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#define ID_BUTTON 100
#define ID_MYTEXT 101
#define ID_MYTEXT1 102
#define ID_MYSTATIC 103
#define ID_BUTTON1 104

LPSTR szClassName = TEXT("MyClass");
HINSTANCE hInstance;
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
   WNDCLASS wnd;
   MSG msg;
   HWND hwnd;

   hInstance = hInst;
        
   wnd.style = CS_HREDRAW | CS_VREDRAW; //we will explain this later
   wnd.lpfnWndProc = MyWndProc;
   wnd.cbClsExtra = 0;
   wnd.cbWndExtra = 0;
   wnd.hInstance = hInstance;
   wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); //default icon
   wnd.hCursor = LoadCursor(NULL, IDC_ARROW);   //default arrow mouse cursor
   wnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(25,131,224));
   wnd.lpszMenuName = NULL;                     //no menu
   wnd.lpszClassName = szClassName;

   if(!RegisterClass(&wnd))                     //register the WNDCLASS
   {
       MessageBox(NULL, "This Program Requires Windows NT", 
                        "Error", MB_OK);
       return 0;
   }

   hwnd = CreateWindowEx(
                        (DWORD)NULL,
                        szClassName,
                       "Notes",
                       WS_OVERLAPPEDWINDOW, //basic window style
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,       //set starting point to default value
                       400,
                       300,       //set all the dimensions to default value
                       NULL,                //no parent window
                       NULL,                //no menu
                       hInstance,
                       NULL);               //no parameters to pass
   ShowWindow(hwnd, iCmdShow);              //display the window on the screen
   UpdateWindow(hwnd);             //make sure the window is updated correctly

   while(GetMessage(&msg, NULL, 0, 0))      //message loop
   {
       TranslateMessage(&msg);
       DispatchMessage(&msg);
   }
   return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND button,button1,hwndText,hwndText1,hwndLabel;
   static char * message = new char[100];
    LRESULT textSize;
    HBRUSH color ;


    switch(msg)
    {
        case WM_CREATE:
            button = CreateWindowEx(
                                (DWORD) NULL,
                                TEXT("button"),                      // The class name required is button
                               TEXT("Sign me"),                  // the caption of the button
                               WS_CHILD |WS_VISIBLE | BS_PUSHBUTTON,  // the styles
                               300,230,                                  // the left and top co-ordinates
                               70,30,                              // width and height
                               hwnd,                                 // parent window handle
                               (HMENU)ID_BUTTON,                   // the ID of your button
                               hInstance,                            // the instance of your application
                               NULL) ;   
             
             button1 = CreateWindowEx(
                                (DWORD) NULL,
                                TEXT("button"),                      // The class name required is button
                               TEXT("    :)    "),                  // the caption of the button
                               WS_CHILD |WS_VISIBLE | BS_PUSHBUTTON,  // the styles
                               300,15,                                  // the left and top co-ordinates
                               70,70,                              // width and height
                               hwnd,                                 // parent window handle
                               (HMENU)ID_BUTTON1,                   // the ID of your button
                               hInstance,                            // the instance of your application
                               NULL) ;  
        
                                // extra bits you dont really need
            



            hwndText = CreateWindowEx(
                                (DWORD) NULL,
                               TEXT("edit"),                              // The class name required is edit
                               TEXT(""),                                 // Default text.
                               WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL, // the styles
                               5, 230,                                      // the left and top co-ordinates
                               280,30,                                  // width and height
                               hwnd,                                     // parent window handle
                               (HMENU)ID_MYTEXT,                         // the ID of your editbox
                               hInstance,                                // the instance of your application
                               NULL
);

                           

            hwndText1 = CreateWindowEx(
                                (DWORD) NULL,
                               TEXT("listbox"),                              // The class name required is edit
                               TEXT(""),                                 // Default text.
                               WS_VISIBLE | WS_CHILD | WS_BORDER |  WS_VSCROLL |ES_MULTILINE | ES_AUTOHSCROLL |ES_READONLY , // the styles
                               5, 5,                                      // the left and top co-ordinates
                               280,220,                                  // width and height
                               hwnd,                                     // parent window handle
                               (HMENU)ID_MYTEXT1,                         // the ID of your editbox
                               hInstance,                                // the instance of your application
                               NULL );
                                                      // extra bits you dont really need
            puts("Hell");
                     break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_BUTTON:
                    
                    textSize = SendMessage(hwndText, WM_GETTEXT, 100, (LPARAM)message);
                    message[textSize] = _T('\0');
                    

                                        
                    //SendDlgItemMessage(hwnd,ID_MYTEXT1,LB_ADDSTRING,0,(LPARAM)message);

                    SendMessage(hwndText,WM_SETTEXT,NULL, (LPARAM)"");
                    SendMessage(hwndText,LB_ADDSTRING,0, (LPARAM)message);  //curata fereastra
                 
                    
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);



                    //}
                    break;


            case ID_BUTTON1:
            MessageBox(hwnd,"Thnks for using me !","Message",MB_OK | MB_ICONINFORMATION);
            break;
            }
            break;
            
            case WM_CTLCOLORLISTBOX:

            color = CreateSolidBrush(RGB(255,255,255));
            return (LONG)color;







 case WM_CLOSE:
            if(MessageBox(hwnd, "Are you're sure ? ", "Message", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
                DestroyWindow(hwnd);

            break ;
            


            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



 