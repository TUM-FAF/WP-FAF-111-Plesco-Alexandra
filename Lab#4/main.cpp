#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <assert.h>
#include <windowsx.h>
#include "resource.h"

const char g_szClassName[] = "Animation Problem";
const int ID_TIMER = 1;

//Speed and ball number controllers
int iBallSpeed = 1;
int iMousePositionX = 1;
int iMousePositionY = 1;

int iNumberOfBalls  = 0;
int iNumberOfClicks = 0;
bool bNumberOfBallsChanged;

int iTimerOfRefresh = 25;

HINSTANCE hInst;

//Ball structure with all the manipulation data
typedef struct BALLINFO {
  int width;
  int height;
  int x;
  int y;

  int dx;
  int dy;
  bool empty = true;
} BALLINFO;

//bitmap arrays containing bitmaps and their data
BALLINFO BallInfo[10];
HBITMAP  hbmBall[10];
HBITMAP  hbmMask[10];

//All the functions used
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP CreateBitmapMask(HBITMAP, COLORREF);
void DrawBall(HDC, RECT*);
void UpdateBall(RECT*);
bool valueInRange(int, int, int);
bool rectOverlap(BALLINFO, BALLINFO);
void AddOneBall();
void ShiftBallsInArray ();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow) {

  WNDCLASSEX wc;
  HWND hwnd;
  MSG Msg;
  hInst           = hInstance;

  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  if(!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Window Registration Failed!", "Error!",
        MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      g_szClassName,
      "Animation problem",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
      NULL, NULL, hInstance, NULL);

  if(hwnd == NULL) {
    MessageBox(NULL, "Window Creation Failed!", "Error!",
        MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return Msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

     static int  iMousePositionX = 1;
     static int  iMousePositionY = 1;
            HDC  hdc;
            RECT rcClient;

  switch(msg) {


    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;


    case WM_LBUTTONDOWN: {
        UINT ret;
        BITMAP bm;
        int i;

        //Drow the first ball at mouse position
        if(iNumberOfBalls < 1) {

          //Get mouse position
          iMousePositionX = GET_X_LPARAM(lParam);
          iMousePositionY = GET_Y_LPARAM(lParam);

          //Add a new bitmap to be drown in the cycle
          iNumberOfBalls  += 1;
          RedrawWindow(hwnd, NULL, NULL, true);
        }
    }
      break;


    case WM_PAINT: {

        PAINTSTRUCT ps;
        UINT ret;
        BITMAP bm;

        hdc = BeginPaint(hwnd, &ps);

        // Load the bitmaps from the resources
        hbmBall[iNumberOfBalls] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BALL));
        if(hbmBall[iNumberOfBalls] == NULL)
          MessageBox(hwnd, "Could not load BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);

        //Create a mask for every bitmap loaded
        hbmMask[iNumberOfBalls] = CreateBitmapMask(hbmBall[iNumberOfBalls], RGB(0, 0, 0));
        if(hbmMask[iNumberOfBalls] == NULL)
          MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);

        GetObject(hbmBall[iNumberOfBalls], sizeof(bm), &bm);

        //ZeroMemory(&BallInfo[iNumberOfBalls], sizeof(BallInfo[iNumberOfBalls]));
        BallInfo[iNumberOfBalls].width = bm.bmWidth;
        BallInfo[iNumberOfBalls].height = bm.bmHeight;

        //Draw only the first ball under the cursor
        if(iNumberOfBalls <= 1) {BallInfo[iNumberOfBalls].x = iMousePositionX;}
        if(iNumberOfBalls <= 1) {BallInfo[iNumberOfBalls].y = iMousePositionY;}

        //Givi the speed of the first ball
        if(iNumberOfBalls <= 1) {BallInfo[iNumberOfBalls].dx = iBallSpeed;}
        if(iNumberOfBalls <= 1) {BallInfo[iNumberOfBalls].dy = iBallSpeed;}

       //Set Timer
        ret = SetTimer(hwnd, ID_TIMER, iTimerOfRefresh, NULL);
        if(ret == 0)
          MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);

        EndPaint(hwnd, &ps);
      }
      break;


    case WM_TIMER:{

        if(bNumberOfBallsChanged == true){
            //Redrow the new ball along with the existing ones
            RedrawWindow(hwnd, NULL, NULL, true);
            bNumberOfBallsChanged = false;
        }

        hdc = GetDC(hwnd);

        GetClientRect(hwnd, &rcClient);
        UpdateBall(&rcClient);
        DrawBall(hdc, &rcClient);

        ReleaseDC(hwnd, hdc);
      }
      break;

      case WM_MOUSEWHEEL:{

        if(iTimerOfRefresh >=1 && iTimerOfRefresh <= 100){
        ((short) HIWORD(wParam)< 0) ? iTimerOfRefresh+=5 : iTimerOfRefresh-=5;

        //Prevent the speed control to block
        if(iTimerOfRefresh <= 0){iTimerOfRefresh = 1;}
        if(iTimerOfRefresh >= 100){iTimerOfRefresh = 99;}

        //Update on timer
        KillTimer(hwnd, ID_TIMER);

        SetTimer(hwnd, ID_TIMER, iTimerOfRefresh, NULL);
        }

      }
      break;


    case WM_DESTROY:
      KillTimer(hwnd, ID_TIMER);
      for(int i = 1; i <= iNumberOfBalls; i ++){
        DeleteObject(hbmBall[i]);
        DeleteObject(hbmMask[i]);
      }

      PostQuitMessage(0);
      break;


    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }


  return 0;
}


HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent) {
  HDC hdcMem, hdcMem2;
  HBITMAP hbmMask;
  BITMAP bm;

  GetObject(hbmColour, sizeof(BITMAP), &bm);
  hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

  hdcMem = CreateCompatibleDC(0);
  hdcMem2 = CreateCompatibleDC(0);

  SelectObject(hdcMem, hbmColour);
  SelectObject(hdcMem2, hbmMask);

  SetBkColor(hdcMem, crTransparent);

  BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

  BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return hbmMask;
}

void DrawBall(HDC hdc, RECT* prc){
  HDC hdcBuffer = CreateCompatibleDC(hdc);
  HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
  HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

  HDC hdcMem = CreateCompatibleDC(hdc);
  HBITMAP hbmOld;


  FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));

  //Enter in a cycle to drow all the balls in the buffer
  for(int i = 1; i <= iNumberOfBalls; i ++){
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMask[i]);

    BitBlt(hdcBuffer, BallInfo[i].x, BallInfo[i].y, BallInfo[i].width, BallInfo[i].height, hdcMem, 0, 0, SRCAND);

    SelectObject(hdcMem, hbmBall[i]);
    BitBlt(hdcBuffer, BallInfo[i].x, BallInfo[i].y, BallInfo[i].width, BallInfo[i].height, hdcMem, 0, 0, SRCPAINT);
  }

  //Swap the buffer
  BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

  SelectObject(hdcMem, hbmOld);
  DeleteDC(hdcMem);

  SelectObject(hdcBuffer, hbmOldBuffer);
  DeleteDC(hdcBuffer);
  DeleteObject(hbmBuffer);
}

void UpdateBall(RECT* prc) {
  for(int i = 1; i <= iNumberOfBalls; i ++) {
    BallInfo[i].x += BallInfo[i].dx;
    BallInfo[i].y += BallInfo[i].dy;

    if(BallInfo[i].x < 0) {
      BallInfo[i].x = 0;
      BallInfo[i].dx = iBallSpeed;
       // add a new ball(make an effect of splitting one ball into 2 balls)
      BallInfo[iNumberOfBalls+1].x = 0;
      BallInfo[iNumberOfBalls+1].y = BallInfo[i].y ;
      BallInfo[iNumberOfBalls+1].dx = iBallSpeed;
      BallInfo[iNumberOfBalls+1].dy = -1 * BallInfo[i].dy;
      AddOneBall();
    }  else if(BallInfo[i].x + BallInfo[i].width > prc->right) {
      BallInfo[i].x = prc->right - BallInfo[i].width;
      BallInfo[i].dx = -iBallSpeed;
      // add a new ball(make an effect of splitting one ball into 2 balls)
      BallInfo[iNumberOfBalls+1].x = prc->right - BallInfo[i].width;
      BallInfo[iNumberOfBalls+1].y = BallInfo[i].y;
      BallInfo[iNumberOfBalls+1].dy = -1 * BallInfo[i].dy;
      BallInfo[iNumberOfBalls+1].dx = -iBallSpeed;
      AddOneBall();
    }

    if(BallInfo[i].y < 0) { // cheching on y-axis
      BallInfo[i].y = 0;
      BallInfo[i].dy = iBallSpeed;
      // add a new ball(make an effect of splitting one ball into 2 balls)
      BallInfo[iNumberOfBalls+1].y = 0;                    // y position of the new ball
      BallInfo[iNumberOfBalls+1].x = BallInfo[i].x;        // x position remains the same
      BallInfo[iNumberOfBalls+1].dy = iBallSpeed;          // on y-axis the direction and speed remains the same
      BallInfo[iNumberOfBalls+1].dx = -1 * BallInfo[i].dx; // on x-axis the direction inverts
      AddOneBall();
        //BallInfo[i].empty = true;
        //ShiftBallsInArray();
        //DeleteObject(hbmBall[iNumberOfBalls]);
        //DeleteObject(hbmMask[iNumberOfBalls]);
       // iNumberOfBalls -= 1;
    }  else if(BallInfo[i].y + BallInfo[i].height > prc->bottom) {
      BallInfo[i].y = prc->bottom - BallInfo[i].height;
      BallInfo[i].dy = -iBallSpeed;
       // add a new ball(make an effect of splitting one ball into 2 balls)
      BallInfo[iNumberOfBalls+1].y = prc->bottom - BallInfo[i].height;
      BallInfo[iNumberOfBalls+1].x = BallInfo[i].x;
      BallInfo[iNumberOfBalls+1].dy = -iBallSpeed;
      BallInfo[iNumberOfBalls+1].dx = -1 * BallInfo[i].dx;
      AddOneBall();
    }

  }
}

void AddOneBall(){
      if(iNumberOfBalls < 8){ iNumberOfBalls  += 1;}
      bNumberOfBallsChanged = true;
}

void ShiftBallsInArray () {
    for(int i = 1; i <= iNumberOfBalls; i ++) {
        if(BallInfo[i].empty == true){
            for(int j = i; j <= iNumberOfBalls-1; j ++) {
                BallInfo[j] = BallInfo[j+1];
            }
        }
    }
}
