// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "Views.h"

class FrameWindowWnd : public WindowWnd, public INotifyUI
{
public:
   FrameWindowWnd() : m_hWndClient(NULL) { };
   const TCHAR* GetWindowClassName() const { return _T("UIMainFrame"); };
   UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME; };
   void OnFinalMessage(HWND /*hWnd*/) { delete this; };

   void Notify(TNotifyUI& msg)
   {
      if (msg.sType == _T("click") || msg.sType == _T("link"))  _CreatePage(msg.pSender->GetName());
      if (msg.sType == _T("itemactivate"))  _CreatePage(_T("page_search"));
   }

   LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
   {
      if (uMsg == WM_CREATE)  {
         SetIcon(IDR_MAINFRAME);
         _CreatePage("page_start");
      }
      if (uMsg == WM_DESTROY)  {
         ::PostQuitMessage(0L);
      }
      if (uMsg == WM_SETFOCUS)  {
         ::SetFocus(m_hWndClient);
      }
      if (uMsg == WM_SIZE)  {
         RECT rcClient;
         ::GetClientRect(m_hWnd, &rcClient);
         ::MoveWindow(m_hWndClient, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
         return 0;
      }
      if (uMsg == WM_ERASEBKGND)  {
         return 1;
      }
      if (uMsg == WM_PAINT)  {
         PAINTSTRUCT ps = { 0 };
         ::BeginPaint(m_hWnd, &ps);
         ::EndPaint(m_hWnd, &ps);
         return 0;
      }
      return WindowWnd::HandleMessage(uMsg, wParam, lParam);
   }

   void _CreatePage(StdString sName)
   {
      StandardPageWnd* win = NULL;
      if (sName == "page_start")  win = new StartPageWnd;
      if (sName == "page_configure")  win = new ConfigurePageWnd;
      if (sName == "page_reports")  win = new ReportsPageWnd;
      if (sName == "page_systems")  win = new SystemsPageWnd;
      if (sName == "page_registers")  win = new RegistersPageWnd;
      if (sName == "link_start")  win = new StartPageWnd;
      if (sName == "link_configure")  win = new ConfigurePageWnd;
      if (sName == "link_reports")  win = new ReportsPageWnd;
      if (sName == "link_systems")  win = new SystemsPageWnd;
      if (sName == "link_registers")  win = new RegistersPageWnd;
      if (win != NULL)  {
         if (m_hWndClient != NULL)  ::PostMessage(m_hWndClient, WM_CLOSE, 0, 0L);
         win->m_pm.AddNotifier(this);
         m_hWndClient = win->Create(m_hWnd, NULL, UI_WNDSTYLE_CHILD, 0);
         PostMessage(WM_SIZE);
         return;
      }
      if (sName == "page_search")  win = new SearchPageWnd;
      if (sName == "page_edit")  win = new EditPageWnd;
      if (win != NULL)  {
         win->Create(m_hWnd, NULL, UI_WNDSTYLE_FRAME, UI_WNDSTYLE_EX_FRAME);
         return;
      }
   }

protected:
   HWND m_hWndClient;
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
   PaintManagerUI::SetResourceInstance(hInstance);

   HRESULT Hr = ::CoInitialize(NULL);
   if (FAILED(Hr))  return 0;

   if (::LoadLibrary("d3d9.dll") == NULL)  ::MessageBox(NULL, "Denne applikation vises bedst med DirectX 9 installeret!", "Test", MB_ICONINFORMATION);

   FrameWindowWnd* pFrame = new FrameWindowWnd();
   if (pFrame == NULL)  return 0;
   pFrame->Create(NULL, "Test", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

   PaintManagerUI::MessageLoop();

   ::CoUninitialize();
   return 0;
}
