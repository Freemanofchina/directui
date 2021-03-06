#include "StdAfx.h"
#include "UIBase.h"

#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

void UILIB_API __Trace(const char* fmt, ...)
{
#ifdef _DEBUG
    char buf[300] = { 0 };
    va_list args;
    va_start(args, fmt);
    ::vsnprintf(buf, dimof(buf) - 2, fmt, args);
    va_end(args);
    ::OutputDebugStringA(buf);
    ::OutputDebugStringA("\n");
#endif
}

const char* __TraceMsg(UINT uMsg)
{
#define MSGDEF(x) if(uMsg==x) return #x
    MSGDEF(WM_SETCURSOR);
    MSGDEF(WM_NCHITTEST);
    MSGDEF(WM_NCPAINT);
    MSGDEF(WM_PAINT);
    MSGDEF(WM_ERASEBKGND);
    MSGDEF(WM_NCMOUSEMOVE);  
    MSGDEF(WM_MOUSEMOVE);
    MSGDEF(WM_MOUSELEAVE);
    MSGDEF(WM_MOUSEHOVER);   
    MSGDEF(WM_NOTIFY);
    MSGDEF(WM_COMMAND);
    MSGDEF(WM_MEASUREITEM);
    MSGDEF(WM_DRAWITEM);   
    MSGDEF(WM_LBUTTONDOWN);
    MSGDEF(WM_LBUTTONUP);
    MSGDEF(WM_LBUTTONDBLCLK);
    MSGDEF(WM_RBUTTONDOWN);
    MSGDEF(WM_RBUTTONUP);
    MSGDEF(WM_RBUTTONDBLCLK);
    MSGDEF(WM_SETFOCUS);
    MSGDEF(WM_KILLFOCUS);  
    MSGDEF(WM_MOVE);
    MSGDEF(WM_SIZE);
    MSGDEF(WM_SIZING);
    MSGDEF(WM_MOVING);
    MSGDEF(WM_GETMINMAXINFO);
    MSGDEF(WM_CAPTURECHANGED);
    MSGDEF(WM_WINDOWPOSCHANGED);
    MSGDEF(WM_WINDOWPOSCHANGING);   
    MSGDEF(WM_NCCALCSIZE);
    MSGDEF(WM_NCCREATE);
    MSGDEF(WM_NCDESTROY);
    MSGDEF(WM_TIMER);
    MSGDEF(WM_KEYDOWN);
    MSGDEF(WM_KEYUP);
    MSGDEF(WM_CHAR);
    MSGDEF(WM_SYSKEYDOWN);
    MSGDEF(WM_SYSKEYUP);
    MSGDEF(WM_SYSCOMMAND);
    MSGDEF(WM_SYSCHAR);
    MSGDEF(WM_VSCROLL);
    MSGDEF(WM_HSCROLL);
    MSGDEF(WM_CHAR);
    MSGDEF(WM_SHOWWINDOW);
    MSGDEF(WM_PARENTNOTIFY);
    MSGDEF(WM_CREATE);
    MSGDEF(WM_NCACTIVATE);
    MSGDEF(WM_ACTIVATE);
    MSGDEF(WM_ACTIVATEAPP);   
    MSGDEF(WM_CLOSE);
    MSGDEF(WM_DESTROY);
    MSGDEF(WM_GETICON);   
    MSGDEF(WM_GETTEXT);
    MSGDEF(WM_GETTEXTLENGTH);   
    MSGDEF(WM_IME_SETCONTEXT);
    MSGDEF(WM_IME_NOTIFY);
    MSGDEF(WM_PRINTCLIENT);
    MSGDEF(WM_CTLCOLORSCROLLBAR);
    MSGDEF(WM_MOUSEACTIVATE);
    MSGDEF(WM_CTLCOLOREDIT);
    MSGDEF(WM_EXITSIZEMOVE);
    MSGDEF(WM_QUERYUISTATE);
    MSGDEF(WM_NOTIFYFORMAT);

    static char buf[32];
    ::sprintf(buf, "0x%04X", uMsg);
    return buf;
}

CRect::CRect()
{
    left = top = right = bottom = 0;
}

CRect::CRect(const RECT& src)
{
    left = src.left;
    top = src.top;
    right = src.right;
    bottom = src.bottom;
}

CRect::CRect(int iLeft, int iTop, int iRight, int iBottom)
{
    left = iLeft;
    top = iTop;
    right = iRight;
    bottom = iBottom;
}

int CRect::GetWidth() const
{
    return right - left;
}

int CRect::GetHeight() const
{
    return bottom - top;
}

void CRect::Empty()
{
    left = top = right = bottom = 0;
}

void CRect::Join(const RECT& rc)
{
    if (rc.left < left)  left = rc.left;
    if (rc.top < top)  top = rc.top;
    if (rc.right > right)  right = rc.right;
    if (rc.bottom > bottom)  bottom = rc.bottom;
}

void CRect::ResetOffset()
{
    ::OffsetRect(this, -left, -top);
}

void CRect::Normalize()
{
    if (left > right)  { int iTemp = left; left = right; right = iTemp; }
    if (top > bottom)  { int iTemp = top; top = bottom; bottom = iTemp; }
}

void CRect::Offset(int cx, int cy)
{
    ::OffsetRect(this, cx, cy);
}

void CRect::Inflate(int cx, int cy)
{
    ::InflateRect(this, cx, cy);
}

void CRect::Deflate(int cx, int cy)
{
    ::InflateRect(this, -cx, -cy);
}

void CRect::Union(CRect& rc)
{
    ::UnionRect(this, this, &rc);
}

CSize::CSize()
{
    cx = cy = 0;
}

CSize::CSize(const SIZE& src)
{
    cx = src.cx;
    cy = src.cy;
}

CSize::CSize(const RECT rc)
{
    cx = RectDx(rc);
    cy = RectDy(rc);
}

CSize::CSize(int _cx, int _cy)
{
    cx = _cx;
    cy = _cy;
}

CPoint::CPoint()
{
    x = y = 0;
}

CPoint::CPoint(const POINT& src)
{
    x = src.x;
    y = src.y;
}

CPoint::CPoint(int _x, int _y)
{
    x = _x;
    y = _y;
}

CPoint::CPoint(LPARAM lParam)
{
    x = GET_X_LPARAM(lParam);
    y = GET_Y_LPARAM(lParam);
}


WindowWnd::WindowWnd() : m_hWnd(NULL), m_OldWndProc(::DefWindowProc), m_subclassed(false)
{
}

HWND WindowWnd::GetHWND() const 
{ 
    return m_hWnd; 
}

UINT WindowWnd::GetClassStyle() const
{
    return 0;
}

const char* WindowWnd::GetSuperClassName() const
{
    return NULL;
}

WindowWnd::operator HWND() const
{
    return m_hWnd;
}

HWND WindowWnd::Create(HWND hwndParent, const char* name, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
    return Create(hwndParent, name, dwStyle, dwExStyle, rc.left, rc.top, RectDx(rc), RectDy(rc), hMenu);
}

HWND WindowWnd::Create(HWND hwndParent, const char* name, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
    if (GetSuperClassName() != NULL && !RegisterSuperclass())  return NULL;
    if (GetSuperClassName() == NULL && !RegisterWindowClass())  return NULL;
    m_hWnd = ::CreateWindowExUtf8(dwExStyle, GetWindowClassName(), name, dwStyle, x, y, cx, cy, hwndParent, hMenu, PaintManagerUI::GetResourceInstance(), this);
    ASSERT(m_hWnd!=NULL);
    return m_hWnd;
}

HWND WindowWnd::Subclass(HWND hWnd)
{
    ASSERT(::IsWindow(hWnd));
    ASSERT(m_hWnd==NULL);
    m_OldWndProc = SubclassWindow(hWnd, __WndProc);
    if (m_OldWndProc == NULL)  return NULL;
    m_subclassed = true;
    m_hWnd = hWnd;
    return m_hWnd;
}

void WindowWnd::Unsubclass()
{
    ASSERT(::IsWindow(m_hWnd));
    if (!::IsWindow(m_hWnd))  return;
    if (!m_subclassed)  return;
    SubclassWindow(m_hWnd, m_OldWndProc);
    m_OldWndProc = ::DefWindowProc;
    m_subclassed = false;
}

void WindowWnd::ShowWindow(bool bShow /*= true*/, bool takeFocus /*= false*/)
{
    ASSERT(::IsWindow(m_hWnd));
    if (!::IsWindow(m_hWnd))  return;
    ::ShowWindow(m_hWnd, bShow ? (takeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

bool WindowWnd::ShowModal()
{
    ASSERT(::IsWindow(m_hWnd));
    HWND hWndParent = GetWindowOwner(m_hWnd);
    ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
    ::EnableWindow(hWndParent, FALSE);
    MSG msg = { 0 };
    while ( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0))  {
        if (msg.message == WM_CLOSE)  {
            ::EnableWindow(hWndParent, TRUE);
            ::SetFocus(hWndParent);
        }
        if (!PaintManagerUI::TranslateMessage(&msg))  {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)  break;
    }
    ::EnableWindow(hWndParent, TRUE);
    ::SetFocus(hWndParent);
    if (msg.message == WM_QUIT)  ::PostQuitMessage(msg.wParam);
    return true;
}

void WindowWnd::Close()
{
    ASSERT(::IsWindow(m_hWnd));
    if (!::IsWindow(m_hWnd))  return;
    PostMessage(WM_CLOSE);
}

void WindowWnd::CenterWindow()
{
    ASSERT(::IsWindow(m_hWnd));
    ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
    RECT rcDlg = { 0 };
    ::GetWindowRect(m_hWnd, &rcDlg);
    RECT rcArea = { 0 };
    RECT rcCenter = { 0 };
    HWND hWndParent = ::GetParent(m_hWnd);
    HWND hWndCenter = ::GetWindowOwner(m_hWnd);
    ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
    if (hWndCenter == NULL)  rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

    int DlgWidth = RectDx(rcDlg);
    int DlgHeight = RectDy(rcDlg);

    // Find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    // The dialog is outside the screen, move it inside
    if (xLeft < rcArea.left)  xLeft = rcArea.left;
    else if (xLeft + DlgWidth > rcArea.right)  xLeft = rcArea.right - DlgWidth;
    if (yTop < rcArea.top)  yTop = rcArea.top;
    else if (yTop + DlgHeight > rcArea.bottom)  yTop = rcArea.bottom - DlgHeight;
    ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void WindowWnd::SetIcon(UINT nRes)
{
    HICON hIcon = (HICON)::LoadImage(PaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    ASSERT(hIcon);
    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
    hIcon = (HICON)::LoadImage(PaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    ASSERT(hIcon);
    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

bool WindowWnd::RegisterWindowClass()
{
    WNDCLASSA wc = { 0 };
    wc.style = GetClassStyle();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.lpfnWndProc = WindowWnd::__WndProc;
    wc.hInstance = PaintManagerUI::GetResourceInstance();
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = GetWindowClassName();
    ATOM ret = ::RegisterClassA(&wc);
    ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
    return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool WindowWnd::RegisterSuperclass()
{
    // Get the class information from an existing
    // window so we can subclass it later on...
    WNDCLASSEXA wc = { 0 };
    wc.cbSize = sizeof(wc);
    if (!::GetClassInfoExA(NULL, GetSuperClassName(), &wc))  {
        if (!::GetClassInfoExA(PaintManagerUI::GetResourceInstance(), GetSuperClassName(), &wc))  {
            ASSERT(!"Unable to locate window class");
            return NULL;
        }
    }
    m_OldWndProc = wc.lpfnWndProc;
    wc.lpfnWndProc = WindowWnd::__ControlProc;
    wc.hInstance = PaintManagerUI::GetResourceInstance();
    wc.lpszClassName = GetWindowClassName();
    ATOM ret = ::RegisterClassExA(&wc);
    ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
    return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK WindowWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowWnd* win = NULL;
    if (uMsg == WM_NCCREATE)  {
        CREATESTRUCT *lpcs = (CREATESTRUCT*)lParam;
        win = (WindowWnd*)lpcs->lpCreateParams;
        win->m_hWnd = hWnd;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LPARAM)win);
    } else {
        win = reinterpret_cast<WindowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (uMsg == WM_NCDESTROY && win != NULL)  {
            LRESULT lRes = ::CallWindowProc(win->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            ::SetWindowLongPtr(win->m_hWnd, GWLP_USERDATA, 0L);
            if (win->m_subclassed)  win->Unsubclass();
            win->m_hWnd = NULL;
            win->OnFinalMessage(hWnd);
            return lRes;
        }
    }
    if (win != NULL)  {
        return win->HandleMessage(uMsg, wParam, lParam);
    } else {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK WindowWnd::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowWnd* pThis = NULL;
    if (uMsg == WM_NCCREATE)  {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<WindowWnd*>(lpcs->lpCreateParams);
        ::SetPropA(hWnd, "WndX", (HANDLE) pThis);
        pThis->m_hWnd = hWnd;
    } else {
        pThis = reinterpret_cast<WindowWnd*>(::GetPropA(hWnd, "WndX"));
        if (uMsg == WM_NCDESTROY && pThis != NULL)  {
            LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            if (pThis->m_subclassed)  pThis->Unsubclass();
            ::SetPropA(hWnd, "WndX", NULL);
            pThis->m_hWnd = NULL;
            pThis->OnFinalMessage(hWnd);
            return lRes;
        }
    }
    if (pThis != NULL)  {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    } else {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT WindowWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
    ASSERT(::IsWindow(m_hWnd));
    return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 

LRESULT WindowWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
    ASSERT(::IsWindow(m_hWnd));
    return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

void WindowWnd::ResizeClient(int cx /*= -1*/, int cy /*= -1*/)
{
    ASSERT(::IsWindow(m_hWnd));
    RECT rc = { 0 };;
    if (!::GetClientRect(m_hWnd, &rc))  return;
    if (cx != -1)  rc.right = cx;
    if (cy != -1)  rc.bottom = cy;
    BOOL hasMenu = !(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL);
    DWORD style   = GetWindowStyle(m_hWnd);
    DWORD exStyle = GetWindowExStyle(m_hWnd);
    if (!::AdjustWindowRectEx(&rc, style, hasMenu, exStyle))
        return;
    UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
    ::SetWindowPos(m_hWnd, NULL, 0, 0, RectDx(rc), RectDy(rc), uFlags);
}

LRESULT WindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
}

void WindowWnd::OnFinalMessage(HWND /*hWnd*/)
{
}

StdPtrArray::StdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
    ASSERT(iPreallocSize>=0);
    if (iPreallocSize > 0)  m_ppVoid = static_cast<void**>(malloc(iPreallocSize * sizeof(void*)));
}

StdPtrArray::~StdPtrArray()
{
    free(m_ppVoid);
}

void StdPtrArray::Empty()
{
    free(m_ppVoid);
    m_ppVoid = NULL;
    m_nCount = m_nAllocated = 0;
}

void StdPtrArray::Resize(int iSize)
{
    Empty();
    m_ppVoid = static_cast<void**>(malloc(iSize * sizeof(void*)));
    ::ZeroMemory(m_ppVoid, iSize * sizeof(void*));
    m_nAllocated = iSize;
    m_nCount = iSize;
}

bool StdPtrArray::IsEmpty() const
{
    return m_nCount == 0;
}

bool StdPtrArray::Add(void* data)
{
    if (++m_nCount >= m_nAllocated) {
        m_nAllocated *= 2;
        if (m_nAllocated == 0)  m_nAllocated = 11;
        m_ppVoid = static_cast<void**>(realloc(m_ppVoid, m_nAllocated * sizeof(void*)));
        if (m_ppVoid == NULL)  return false;
    }
    m_ppVoid[m_nCount - 1] = data;
    return true;
}

bool StdPtrArray::InsertAt(int idx, void* data)
{
    if (idx == m_nCount)  return Add(data);
    if (idx < 0 || idx > m_nCount)  return false;
    if (++m_nCount >= m_nAllocated) {
        m_nAllocated *= 2;
        if (m_nAllocated == 0)  m_nAllocated = 11;
        m_ppVoid = static_cast<void**>(realloc(m_ppVoid, m_nAllocated * sizeof(void*)));
        if (m_ppVoid == NULL)  return false;
    }
    memmove(&m_ppVoid[idx + 1], &m_ppVoid[idx], (m_nCount - idx - 1) * sizeof(void*));
    m_ppVoid[idx] = data;
    return true;
}

bool StdPtrArray::SetAt(int idx, void* data)
{
    if (idx < 0 || idx >= m_nCount)  return false;
    m_ppVoid[idx] = data;
    return true;
}

bool StdPtrArray::Remove(int idx)
{
    if (idx < 0 || idx >= m_nCount)  return false;
    if (idx < --m_nCount)  ::CopyMemory(m_ppVoid + idx, m_ppVoid + idx + 1, (m_nCount - idx) * sizeof(void*));
    return true;
}

int StdPtrArray::Find(void* data) const
{
    for (int i = 0; i < m_nCount; i++)  if (m_ppVoid[i] == data)  return i;
    return -1;
}

int StdPtrArray::GetSize() const
{
    return m_nCount;
}

void** StdPtrArray::GetData()
{
    return m_ppVoid;
}

void* StdPtrArray::GetAt(int idx) const
{
    if (idx < 0 || idx >= m_nCount)  return NULL;
    return m_ppVoid[idx];
}

void* StdPtrArray::operator[] (int idx) const
{
    ASSERT(idx>=0 && idx<m_nCount);
    return m_ppVoid[idx];
}

StdValArray::StdValArray(int iElementSize, int iPreallocSize /*= 0*/) : 
m_pVoid(NULL), 
    m_nCount(0), 
    m_iElementSize(iElementSize), 
    m_nAllocated(iPreallocSize)
{
    ASSERT(iElementSize>0);
    ASSERT(iPreallocSize>=0);
    if (iPreallocSize > 0)  m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
}

StdValArray::~StdValArray()
{
    free(m_pVoid);
}

void StdValArray::Empty()
{   
    m_nCount = 0;  // NOTE: We keep the memory in place
}

bool StdValArray::IsEmpty() const
{
    return m_nCount == 0;
}

bool StdValArray::Add(LPCVOID data)
{
    if (++m_nCount >= m_nAllocated) {
        m_nAllocated *= 2;
        if (m_nAllocated == 0)  m_nAllocated = 11;
        m_pVoid = static_cast<LPBYTE>(realloc(m_pVoid, m_nAllocated * m_iElementSize));
        if (m_pVoid == NULL)  return false;
    }
    ::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), data, m_iElementSize);
    return true;
}

bool StdValArray::Remove(int idx)
{
    if (idx < 0 || idx >= m_nCount)  return false;
    if (idx < --m_nCount)  ::CopyMemory(m_pVoid + (idx * m_iElementSize), m_pVoid + ((idx + 1) * m_iElementSize), (m_nCount - idx) * m_iElementSize);
    return true;
}

int StdValArray::GetSize() const
{
    return m_nCount;
}

void* StdValArray::GetData()
{
    return static_cast<void*>(m_pVoid);
}

void* StdValArray::GetAt(int idx) const
{
    if (idx < 0 || idx >= m_nCount)  return NULL;
    return m_pVoid + (idx * m_iElementSize);
}

void* StdValArray::operator[] (int idx) const
{
    ASSERT(idx>=0 && idx<m_nCount);
    return m_pVoid + (idx * m_iElementSize);
}

WaitCursor::WaitCursor()
{
    m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
}

WaitCursor::~WaitCursor()
{
    ::SetCursor(m_hOrigCursor);
}

StdString::StdString() : m_pstr(m_buf)
{
    m_buf[0] = '\0';
}

StdString::StdString(const char ch) : m_pstr(m_buf)
{
    m_buf[0] = ch;
    m_buf[1] = '\0';
}

StdString::StdString(const char* lpsz, int nLen) : m_pstr(m_buf)
{      
    Assign(lpsz, nLen);
}

StdString::StdString(const StdString& src) : m_pstr(m_buf)
{
    Assign(src.m_pstr);
}

StdString::~StdString()
{
    if (m_buf != m_pstr)
        free(m_pstr);
}

StdString StdString::RES(UINT nRes)
{
    char szBuffer[256];
    int cchLen = ::LoadStringA(PaintManagerUI::GetLanguageInstance(), nRes, szBuffer, dimof(szBuffer) - 1);
    ASSERT(cchLen>0);
    szBuffer[cchLen] = '\0';
    return szBuffer;
}

int StdString::GetLength() const
{ 
    return (int) strlen(m_pstr); 
}

StdString::operator const char*() const 
{ 
    return (const char*)m_pstr; 
}

void StdString::Append(const char* pstr)
{
    int nNewLength = GetLength() + (int) strlen(pstr);
    if (nNewLength >= MAX_LOCAL_STRING_LEN)  {
        if (m_pstr == m_buf)  {
            m_pstr = static_cast<char*>(malloc((nNewLength + 1) * sizeof(char)));
            strcpy(m_pstr, m_buf);
            strcat(m_pstr, pstr);
        } else {
            m_pstr = static_cast<char*>(realloc(m_pstr, (nNewLength + 1) * sizeof(char)));
            strcat(m_pstr, pstr);
        }
    } else {
        if (m_pstr != m_buf)  {
            free(m_pstr);
            m_pstr = m_buf;
        }
        strcat(m_buf, pstr);
    }
}

void StdString::Assign(const char* pstr, int cchMax)
{
    if (pstr == NULL)  pstr = "";
    cchMax = (cchMax < 0 ? (int) strlen(pstr) : cchMax);
    if (cchMax < MAX_LOCAL_STRING_LEN)  {
        if (m_pstr != m_buf)  {
            free(m_pstr);
            m_pstr = m_buf;
        }
    } else if (cchMax > GetLength() || m_pstr == m_buf)  {
        if (m_pstr == m_buf)  m_pstr = NULL;
        m_pstr = static_cast<char*>(realloc(m_pstr, (cchMax + 1) * sizeof(char)));
    }
    strncpy(m_pstr, pstr, cchMax);
    m_pstr[cchMax] = '\0';
}

bool StdString::IsEmpty() const 
{ 
    return m_pstr[0] == '\0'; 
}

void StdString::Empty() 
{ 
    if (m_pstr != m_buf)  free(m_pstr);
    m_pstr = m_buf;
    m_buf[0] = '\0'; 
}

const char* StdString::GetData()
{
    return m_pstr;
}

char StdString::GetAt(int idx) const
{
    return m_pstr[idx];
}

char StdString::operator[] (int idx) const
{ 
    return m_pstr[idx];
}   

const StdString& StdString::operator=(const StdString& src)
{      
    Assign(src);
    return *this;
}

const StdString& StdString::operator=(const char* lpStr)
{      
    Assign(lpStr);
    return *this;
}

#ifndef _UNICODE
const StdString& StdString::operator=(LPCWSTR lpwStr)
{      
    int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
    LPSTR pstr = (LPSTR) _alloca(cchStr);
    if (pstr != NULL)  ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
    Assign(pstr);
    return *this;
}
#endif

const StdString& StdString::operator=(const char ch)
{
    Empty();
    m_buf[0] = ch;
    m_buf[1] = '\0';
    return *this;
}

StdString StdString::operator+(const StdString& src)
{
    Append(src);
    return *this;
}

StdString StdString::operator+(const char* lpStr)
{
    Append(lpStr);
    return *this;
}

const StdString& StdString::operator+=(const StdString& src)
{      
    Append(src);
    return *this;
}

const StdString& StdString::operator+=(const char* lpStr)
{      
    Append(lpStr);
    return *this;
}

const StdString& StdString::operator+=(const char ch)
{      
    char str[] = { ch, '\0' };
    Append(str);
    return *this;
}

bool StdString::operator == (const char* str) const { return (Compare(str) == 0); };
bool StdString::operator != (const char* str) const { return (Compare(str) != 0); };
bool StdString::operator <= (const char* str) const { return (Compare(str) <= 0); };
bool StdString::operator <  (const char* str) const { return (Compare(str) <  0); };
bool StdString::operator >= (const char* str) const { return (Compare(str) >= 0); };
bool StdString::operator >  (const char* str) const { return (Compare(str) >  0); };

void StdString::SetAt(int idx, char ch)
{
    ASSERT(idx>=0 && idx<GetLength());
    m_pstr[idx] = ch;
}

int StdString::Compare(const char* lpsz) const 
{ 
    return str::Eq(m_pstr, lpsz); 
}

int StdString::CompareNoCase(const char* lpsz) const 
{ 
    return _stricmp(m_pstr, lpsz); 
}

void StdString::MakeUpper() 
{ 
    _strupr(m_pstr); 
}

void StdString::MakeLower() 
{ 
    _strlwr(m_pstr); 
}

StdString StdString::Left(int iLength) const
{
    if (iLength < 0)  iLength = 0;
    if (iLength > GetLength())  iLength = GetLength();
    return StdString(m_pstr, iLength);
}

StdString StdString::Mid(int pos, int iLength) const
{
    if (iLength < 0)  iLength = GetLength() - pos;
    if (pos + iLength > GetLength())  iLength = GetLength() - pos;
    if (iLength <= 0)  return StdString();
    return StdString(m_pstr + pos, iLength);
}

StdString StdString::Right(int iLength) const
{
    int pos = GetLength() - iLength;
    if (pos < 0)  {
        pos = 0;
        iLength = GetLength();
    }
    return StdString(m_pstr + pos, iLength);
}

int StdString::Find(char ch, int pos /*= 0*/) const
{
    ASSERT(pos>=0 && pos<=GetLength());
    if (pos != 0 && (pos < 0 || pos >= GetLength()))  return -1;
    const char* p = strchr(m_pstr + pos, ch);
    if (p == NULL)  return -1;
    return p - m_pstr;
}

int StdString::Find(const char* pstrSub, int pos /*= 0*/) const
{
    ASSERT(pos>=0 && pos<=GetLength());
    if (pos != 0 && (pos < 0 || pos > GetLength()))  return -1;
    const char* p = strstr(m_pstr + pos, pstrSub);
    if (p == NULL)  return -1;
    return p - m_pstr;
}

int StdString::ReverseFind(char ch) const
{
    const char* p = strrchr(m_pstr, ch);
    if (p == NULL)  return -1;
    return p - m_pstr;
}

int StdString::Replace(const char* pstrFrom, const char* pstrTo)
{
    StdString sTemp;
    int nCount = 0;
    int pos = Find(pstrFrom);
    if (pos < 0)  return 0;
    int cchFrom = (int) strlen(pstrFrom);
    int cchTo = (int) strlen(pstrTo);
    while ( pos >= 0)  {
        sTemp = Left(pos);
        sTemp += pstrTo;
        sTemp += Mid(pos + cchFrom);
        Assign(sTemp);
        pos = Find(pstrFrom, pos + cchTo);
        nCount++;
    }
    return nCount;
}

int StdString::Format(const char* pstrFormat, ...)
{
    StdString sFormat = pstrFormat;
    sFormat.ProcessResourceTokens();
    // Do ordinary printf replacements
    // NOTE: Documented max-length of wvsprintf() is 1024
    char buf[1025] = { 0 };
    va_list argList;
    va_start(argList, pstrFormat);
    int n = ::wvsprintfA(buf, sFormat, argList);
    va_end(argList);
    Assign(buf);
    return n;
}

void StdString::ProcessResourceTokens()
{
    // Replace string-tokens: %{nnn}  where nnn is a resource string identifier
    int pos = Find('%');
    while (pos >= 0)  {
        if (GetAt(pos + 1) == '{')  {
            int iEndPos = pos + 2;
            while (isdigit(GetAt(iEndPos)))  iEndPos++;
            if (GetAt(iEndPos) == '}')  {
                StdString sTemp = StdString::RES((UINT)atoi(m_pstr + pos + 2));
                Replace(Mid(pos, iEndPos - pos + 1), sTemp);
            }
        }
        pos = Find('%', pos + 1);
    }
}

bool ParseInt(const char *name, const char *val, const char *expectedName, int& ret)
{
    if (str::Eq(name, expectedName)) {
        ret = atoi(val);
        return true;
    }
    return false;
}

bool ParseWidth(const char *name, const char *val, int& dx)
{
    if (str::Eq(name, "width") || str::Eq(name, "dx")) {
        dx = atoi(val);
        return true;
    }
    return false;
}

bool ParseHeight(const char *name, const char *val, int& dy)
{
    if (str::Eq(name, "height") || str::Eq(name, "dy")) {
        dy = atoi(val);
        return true;
    }
    return false;
}

