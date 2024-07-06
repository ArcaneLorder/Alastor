#include "Imitate.h"

namespace Alastor {

    Imitate::Imitate()
    {

    }



    void Imitate::mouseMessage(HWND hWnd, int x, int y, int mouseButton, int mouseAction, bool wait) {
        // ��鴰�ھ���Ƿ���Ч  
        if (hWnd == NULL) {
            hWnd = GetDesktopWindow();
        }

        // ȷ��������Ч  
        if (x < 0 || y < 0) {
            return;
        }

        // ��������ֵ  
        LPARAM lParam = MAKELPARAM(x, y);

        // ������갴ť�Ͷ���������Ϣ  
        switch (mouseButton) {
        case 1: // ���  
            switch (mouseAction) {
            case 1: // ����  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                break;
            case 2: // ˫��  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                    SendMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                    // ע�⣺�첽����˫����Ϣ���ܲ��ᰴԤ�ڹ�������Ϊ���ε�����ܲ����ں̵ܶ�ʱ���ڵ���  
                    PostMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, lParam);
                }
                break;
            case 3: // ����  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                }
                break;
            case 4: // �ſ�  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                break;
            }
            break;
        case 2: // �Ҽ�  
            switch (mouseAction) {
            case 1: // ����  
                if (wait) {
                    SendMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
                    SendMessage(hWnd, WM_RBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
                    PostMessage(hWnd, WM_RBUTTONUP, 0, lParam);
                }
                break;
                // �Ҽ���˫�������ºͷſ����Ը�����Ҫ���  
            }
            break;
        case 3: // �м�  
            switch (mouseAction) {
            case 1: // ����  
                if (wait) {
                    SendMessage(hWnd, WM_MBUTTONDOWN, MK_MBUTTON, lParam);
                    SendMessage(hWnd, WM_MBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_MBUTTONDOWN, MK_MBUTTON, lParam);
                    PostMessage(hWnd, WM_MBUTTONUP, 0, lParam);
                }
                break;
                // �м���˫�������ºͷſ����Ը�����Ҫ���  
            }
            break;
            // ������Ӹ������갴ť����  
        default:
            // �����갴ť����1-3�ķ�Χ�ڣ��򲻷����κ���Ϣ  
            break;
        }
    }
}
