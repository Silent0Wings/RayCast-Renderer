// window.h
// https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/creating-a-win32-window-wrapper-class-r1810/
// https://www.codeproject.com/Articles/2556/A-Simple-Win32-Window-Wrapper-Class

#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#define byte win_byte_override
#include <windows.h>
#include "image.h"

#undef byte

class guiwindow
{
public:
    guiwindow()
    {
        m_img = image();
    }
    guiwindow(const image &img) : m_img(img) {}

    int run(HINSTANCE hInstance, int nCmdShow);
    void set(image img)
    {
        m_img = img;
    }

private:
    image m_img; // store the image
};

#endif // GuiWindow
