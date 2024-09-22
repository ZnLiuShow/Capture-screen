#include <opencv2/opencv.hpp>
#include <Windows.h>
#include "W_Screenshot.h"



Screenshot::Screenshot()
{
    double zoom = getZoom();
    m_width = GetSystemMetrics(SM_CXSCREEN) * zoom;
    m_height = GetSystemMetrics(SM_CYSCREEN) * zoom;
    m_screenshotData = new char[m_width * m_height * 4];
    memset(m_screenshotData, 0, m_width);

    // ��ȡ��Ļ DC
    m_screenDC = GetDC(NULL);
    m_compatibleDC = CreateCompatibleDC(m_screenDC);

    // ����λͼ
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, m_width, m_height);
    SelectObject(m_compatibleDC, m_hBitmap);
}

/* ��ȡ������Ļ�Ľ�ͼ */
cv::Mat Screenshot::getScreenshot()
{
    // �õ�λͼ������
    BitBlt(m_compatibleDC, 0, 0, m_width, m_height, m_screenDC, 0, 0, SRCCOPY);
    GetBitmapBits(m_hBitmap, m_width * m_height * 4, m_screenshotData);

    // ����ͼ��
    cv::Mat screenshot(m_height, m_width, CV_8UC4, m_screenshotData);

    return screenshot;
}

/** @brief ��ȡָ����Χ����Ļ��ͼ
 * @param x ͼ�����Ͻǵ� X ����
 * @param y ͼ�����Ͻǵ� Y ����
 * @param width ͼ����
 * @param height ͼ��߶�
 */
cv::Mat Screenshot::getScreenshot(int x, int y, int width, int height)
{
    cv::Mat screenshot = getScreenshot();
    return screenshot(cv::Rect(x, y, width, height));
}

/* ��ȡ��Ļ����ֵ */
double Screenshot::getZoom()
{
    // ��ȡ���ڵ�ǰ��ʾ�ļ�����
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // ��ȡ�������߼����
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfo(hMonitor, &monitorInfo);
    int cxLogical = (monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);

    // ��ȡ������������
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;

    return cxPhysical * 1.0 / cxLogical;
}

cv::Mat Screenshot::hwnd2mat(HWND hwnd) {
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
    width = windowsize.right / 1;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}