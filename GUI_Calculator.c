#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>

#define PI 3.141592653589793

#define COLOR_BG RGB(10, 10, 20)
#define COLOR_DISPLAY_BG RGB(20, 25, 50)
#define COLOR_DISPLAY_TEXT RGB(0, 255, 200)
#define COLOR_TEXT RGB(235, 245, 255)
#define COLOR_NUMBER RGB(30, 40, 80)
#define COLOR_OPERATOR RGB(255, 120, 0)
#define COLOR_FUNCTION RGB(140, 0, 255)
#define COLOR_EQUALS RGB(0, 255, 120)
#define COLOR_CE RGB(255, 50, 50)
#define COLOR_MODE RGB(0, 180, 255)
#define COLOR_BUTTON_TEXT RGB(255, 255, 255)
#define COLOR_BUTTON_BORDER RGB(0, 255, 200)
#define COLOR_BUTTON_HIGHLIGHT RGB(0, 255, 200)

// Button IDs
#define BTN_0 1000
#define BTN_1 1001
#define BTN_2 1002
#define BTN_3 1003
#define BTN_4 1004
#define BTN_5 1005
#define BTN_6 1006
#define BTN_7 1007
#define BTN_8 1008
#define BTN_9 1009
#define BTN_PLUS 1010
#define BTN_MINUS 1011
#define BTN_MUL 1012
#define BTN_DIV 1013
#define BTN_EQUAL 1014
#define BTN_CLEAR 1015
#define BTN_DOT 1016
#define BTN_SQRT 1017
#define BTN_POWER 1018
#define BTN_SIN 1019
#define BTN_COS 1020
#define BTN_TAN 1021
#define BTN_LOG 1022
#define BTN_LN 1023
#define BTN_FACT 1024
#define BTN_ASIN 1025
#define BTN_ACOS 1026
#define BTN_ATAN 1027
#define BTN_SINH 1028
#define BTN_COSH 1029
#define BTN_TANH 1030
#define BTN_ASINH 1031
#define BTN_ACOSH 1032
#define BTN_ATANH 1033
#define BTN_DEG_RAD 1034
#define BTN_PERM 1035
#define BTN_COMB 1036
#define BTN_DEC 1037
#define BTN_OCT 1038
#define BTN_HEX 1039
#define BTN_BIN 1040
#define BTN_PI 1041
#define BTN_INV 1042
#define BTN_X2 1043
#define BTN_X3 1044
#define BTN_10X 1045
#define BTN_CE 1046
#define BTN_DEG 1047
#define BTN_RAD 1048
#define BTN_GRA 1049
#define BTN_SHIFT 1050
#define BTN_OFF 1051
#define DISPLAY_EXPRESSION 1999
#define DISPLAY_RESULT 2000

// Globals
char display[256] = "";
char currentInput[256] = "";
char expression[512] = "";
double firstValue = 0.0;
double secondValue = 0.0;
int degreeMode = 0;
char currentOperator = '\0';
int newNumber = 1;
int resultShown = 0;
int perm_comb_mode = 0;
double n_value = 0.0;

HWND g_expressionDisplay = NULL;
HWND g_resultDisplay = NULL;
HFONT g_displayFont = NULL;
HFONT g_btnFont = NULL;

// ---------------- HELPERS ----------------

double toRadians(double x) {
    return degreeMode ? x * PI / 180.0 : x;
}

double toDegrees(double x) {
    return degreeMode ? x * 180.0 / PI : x;
}

long double factorial(int n) {
    if (n < 0) return -1;
    long double r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

void UpdateDisplay() {
    if (!g_resultDisplay || !g_expressionDisplay) return;
    if (expression[0] != '\0') {
        SetWindowTextA(g_expressionDisplay, expression);
    } else {
        SetWindowTextA(g_expressionDisplay, "input");
    }
    if (display[0] != '\0') {
        SetWindowTextA(g_resultDisplay, display);
    } else {
        SetWindowTextA(g_resultDisplay, "result");
    }
}

void ClearAll() {
    strcpy(display, "");
    strcpy(currentInput, "");
    expression[0] = '\0';
    firstValue = 0;
    secondValue = 0;
    currentOperator = '\0';
    newNumber = 1;
    resultShown = 0;
    perm_comb_mode = 0;
    n_value = 0.0;
    UpdateDisplay();
}

void AppendDigit(char digit) {
    if (resultShown) {
        ClearAll();
    }
    if (newNumber) {
        strcpy(currentInput, "");
        newNumber = 0;
    }

    int len = strlen(currentInput);
    if (len < 30) {
        currentInput[len] = digit;
        currentInput[len + 1] = '\0';
        strcpy(display, currentInput);
        UpdateDisplay();
    }
}

void PerformCalculation(HWND hwnd) {
    if (strlen(currentInput) == 0) return;

    secondValue = atof(currentInput);
    double result = 0;
    int ok = 1;

    switch (currentOperator) {
        case '+': result = firstValue + secondValue; break;
        case '-': result = firstValue - secondValue; break;
        case '*': result = firstValue * secondValue; break;
        case '/':
            if (secondValue == 0) {
                MessageBoxA(hwnd, "Division by zero!", "Error", MB_OK);
                ok = 0;
            } else result = firstValue / secondValue;
            break;
        case '^': result = pow(firstValue, secondValue); break;
        default: ok = 0;
    }

    if (ok) {
        char expr[512];
        if (expression[0] != '\0') {
            sprintf(expr, "%s %s", expression, currentInput);
        } else {
            sprintf(expr, "%s %c %s", display, currentOperator, currentInput);
        }
        strncpy(expression, expr, sizeof(expression) - 1);
        expression[sizeof(expression) - 1] = '\0';

        sprintf(display, "%.10g", result);
        strcpy(currentInput, display);
        firstValue = result;
        newNumber = 1;
        resultShown = 1;
        currentOperator = '\0';
        UpdateDisplay();
    } else {
        ClearAll();
    }
}

void SetOperator(char op, int appendCurrentInput) {
    if (strlen(currentInput) == 0 && expression[0] == '\0') return;

    if (resultShown) {
        resultShown = 0;
    }

    firstValue = atof(currentInput);
    currentOperator = op;
    newNumber = 1;

    if (appendCurrentInput) {
        if (expression[0] == '\0') {
            snprintf(expression, sizeof(expression), "%s %c", currentInput, op);
        } else {
            size_t len = strlen(expression);
            if (len > 0 && (expression[len - 1] == '+' || expression[len - 1] == '-' || expression[len - 1] == '*' || expression[len - 1] == '/' || expression[len - 1] == '^')) {
                expression[len - 1] = op;
            } else {
                strncat(expression, " ", sizeof(expression) - strlen(expression) - 1);
                strncat(expression, currentInput, sizeof(expression) - strlen(expression) - 1);
                strncat(expression, " ", sizeof(expression) - strlen(expression) - 1);
                char opText[2] = { op, '\0' };
                strncat(expression, opText, sizeof(expression) - strlen(expression) - 1);
            }
        }
    } else {
        if (expression[0] == '\0') {
            snprintf(expression, sizeof(expression), "%s %c", currentInput, op);
        } else {
            size_t len = strlen(expression);
            if (len > 0 && (expression[len - 1] == '+' || expression[len - 1] == '-' || expression[len - 1] == '*' || expression[len - 1] == '/' || expression[len - 1] == '^')) {
                expression[len - 1] = op;
            } else {
                strncat(expression, " ", sizeof(expression) - strlen(expression) - 1);
                char opText[2] = { op, '\0' };
                strncat(expression, opText, sizeof(expression) - strlen(expression) - 1);
            }
        }
    }

    strcpy(currentInput, "");
    strcpy(display, "");
    UpdateDisplay();
}

COLORREF GetButtonColor(int id) {
    if ((id >= BTN_0 && id <= BTN_9) || id == BTN_DOT) return COLOR_NUMBER;
    if (id == BTN_PLUS || id == BTN_MINUS || id == BTN_MUL || id == BTN_DIV || id == BTN_POWER) return COLOR_OPERATOR;
    if (id == BTN_EQUAL) return COLOR_EQUALS;
    if (id == BTN_CE) return COLOR_CE;
    if (id == BTN_DEG || id == BTN_RAD || id == BTN_GRA) return COLOR_MODE;
    return COLOR_FUNCTION;
}

void DrawRoundedButton(LPDRAWITEMSTRUCT pdis) {

    COLORREF baseColor = GetButtonColor(pdis->CtlID);

    if (pdis->itemState & ODS_SELECTED) {
        baseColor = RGB(
            min(255, GetRValue(baseColor) + 50),
            min(255, GetGValue(baseColor) + 50),
            min(255, GetBValue(baseColor) + 50));
    }

    RECT btnRect = pdis->rcItem;

    // Background
    HBRUSH hBackground = CreateSolidBrush(COLOR_BG);
    FillRect(pdis->hDC, &btnRect, hBackground);
    DeleteObject(hBackground);

    // Main button fill
    HBRUSH hBrush = CreateSolidBrush(baseColor);
    HPEN hPen = CreatePen(PS_SOLID, 1, COLOR_BUTTON_BORDER);

    HBRUSH oldBrush = SelectObject(pdis->hDC, hBrush);
    HPEN oldPen = SelectObject(pdis->hDC, hPen);

    int w = btnRect.right - btnRect.left;
    int h = btnRect.bottom - btnRect.top;
    int radius = 14;

    // Main rounded button
    RoundRect(pdis->hDC, 0, 0, w, h, radius, radius);

    // ===== NEON GLOW BORDER =====
    HPEN glowPen = CreatePen(PS_SOLID, 2, COLOR_BUTTON_HIGHLIGHT);
    HPEN oldGlow = SelectObject(pdis->hDC, glowPen);

    RoundRect(pdis->hDC, 1, 1, w - 1, h - 1, radius, radius);

    SelectObject(pdis->hDC, oldGlow);
    DeleteObject(glowPen);

    // Restore old objects
    SelectObject(pdis->hDC, oldBrush);
    SelectObject(pdis->hDC, oldPen);

    DeleteObject(hBrush);
    DeleteObject(hPen);

    // Button text
    RECT textRect = pdis->rcItem;

    char buttonText[64] = {0};
    GetWindowTextA((HWND)pdis->hwndItem, buttonText, sizeof(buttonText));

    SetTextColor(pdis->hDC, COLOR_BUTTON_TEXT);
    SetBkMode(pdis->hDC, TRANSPARENT);

    DrawTextA(
        pdis->hDC,
        buttonText,
        -1,
        &textRect,
        DT_CENTER | DT_VCENTER | DT_SINGLELINE
    );
}

HWND CreateCalcButton(HWND hwnd, HINSTANCE hinst, const char *text, int id, int x, int y, int w, int h) {
    HWND btn = CreateWindowA("BUTTON", text,
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        x, y, w, h, hwnd, (HMENU)id, hinst, NULL);
    if (btn && g_btnFont) {
        SendMessageA(btn, WM_SETFONT, (WPARAM)g_btnFont, TRUE);
    }
    return btn;
}

// ---------------- WINDOW PROC ----------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HINSTANCE hinst;

    switch (msg) {

    case WM_CREATE: {
        CREATESTRUCTA* cs = (CREATESTRUCTA*)lParam;
        hinst = cs->hInstance;
        HRGN hRgn = CreateRoundRectRgn(0, 0, 760, 720, 25, 25);
        SetWindowRgn(hwnd, hRgn, TRUE);

        degreeMode = 0;

        g_displayFont = CreateFontA(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Bahnschrift SemiBold");

        g_btnFont = CreateFontA(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");

        g_expressionDisplay = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_RIGHT | SS_CENTERIMAGE,
            20, 15, 700, 55,
            hwnd, (HMENU)DISPLAY_EXPRESSION, hinst, NULL);

        g_resultDisplay = CreateWindowA("STATIC", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_RIGHT | SS_CENTERIMAGE,
            20, 65, 700, 85,
            hwnd, (HMENU)DISPLAY_RESULT, hinst, NULL);

        SendMessageA(g_expressionDisplay, WM_SETFONT, (WPARAM)g_displayFont, TRUE);
        SendMessageA(g_resultDisplay, WM_SETFONT, (WPARAM)g_displayFont, TRUE);

        int btnX = 20;
        int btnY = 190;
        int btnW = 90;
        int btnH = 75;
        int gap = 10;

        CreateCalcButton(hwnd, hinst, "CE", BTN_CLEAR, btnX + (btnW + gap) * 0, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "MODE", BTN_DEG_RAD, btnX + (btnW + gap) * 1, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "nPr", BTN_PERM, btnX + (btnW + gap) * 2, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "nCr", BTN_COMB, btnX + (btnW + gap) * 3, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "log", BTN_LOG, btnX + (btnW + gap) * 4, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "ln", BTN_LN, btnX + (btnW + gap) * 5, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "OFF", BTN_OFF, btnX + (btnW + gap) * 6, btnY, btnW, btnH);

        btnY += btnH + gap;
        CreateCalcButton(hwnd, hinst, "7", BTN_7, btnX + (btnW + gap) * 0, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "8", BTN_8, btnX + (btnW + gap) * 1, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "9", BTN_9, btnX + (btnW + gap) * 2, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "/", BTN_DIV, btnX + (btnW + gap) * 3, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "sin", BTN_SIN, btnX + (btnW + gap) * 4, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "cos", BTN_COS, btnX + (btnW + gap) * 5, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "tan", BTN_TAN, btnX + (btnW + gap) * 6, btnY, btnW, btnH);

        btnY += btnH + gap;
        CreateCalcButton(hwnd, hinst, "4", BTN_4, btnX + (btnW + gap) * 0, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "5", BTN_5, btnX + (btnW + gap) * 1, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "6", BTN_6, btnX + (btnW + gap) * 2, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "*", BTN_MUL, btnX + (btnW + gap) * 3, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "asin", BTN_ASIN, btnX + (btnW + gap) * 4, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "acos", BTN_ACOS, btnX + (btnW + gap) * 5, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "atan", BTN_ATAN, btnX + (btnW + gap) * 6, btnY, btnW, btnH);

        btnY += btnH + gap;
        CreateCalcButton(hwnd, hinst, "1", BTN_1, btnX + (btnW + gap) * 0, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "2", BTN_2, btnX + (btnW + gap) * 1, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "3", BTN_3, btnX + (btnW + gap) * 2, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "-", BTN_MINUS, btnX + (btnW + gap) * 3, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "sinh", BTN_SINH, btnX + (btnW + gap) * 4, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "cosh", BTN_COSH, btnX + (btnW + gap) * 5, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "tanh", BTN_TANH, btnX + (btnW + gap) * 6, btnY, btnW, btnH);

        btnY += btnH + gap;
        CreateCalcButton(hwnd, hinst, "0", BTN_0, btnX + (btnW + gap) * 0, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, ".", BTN_DOT, btnX + (btnW + gap) * 1, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "=", BTN_EQUAL, btnX + (btnW + gap) * 2, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "+", BTN_PLUS, btnX + (btnW + gap) * 3, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "asinh", BTN_ASINH, btnX + (btnW + gap) * 4, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "acosh", BTN_ACOSH, btnX + (btnW + gap) * 5, btnY, btnW, btnH);
        CreateCalcButton(hwnd, hinst, "atanh", BTN_ATANH, btnX + (btnW + gap) * 6, btnY, btnW, btnH);

        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);

        if (id >= BTN_0 && id <= BTN_9) {
            AppendDigit('0' + (id - BTN_0));
        }

        else if (id == BTN_CLEAR) {
            ClearAll();
        }

        else if (id == BTN_CE) {
            strcpy(currentInput, "");
            strcpy(display, "");
            UpdateDisplay();
        }

        else if (id == BTN_DEC || id == BTN_OCT || id == BTN_HEX || id == BTN_BIN) {
            if (strlen(currentInput) == 0) break;
            long value = atol(currentInput);
            if (id == BTN_DEC) {
                snprintf(display, sizeof(display), "%ld", value);
                snprintf(expression, sizeof(expression), "DEC(%s)", currentInput);
            } else if (id == BTN_OCT) {
                snprintf(display, sizeof(display), "%lo", (unsigned long)value);
                snprintf(expression, sizeof(expression), "OCT(%s)", currentInput);
            } else if (id == BTN_HEX) {
                snprintf(display, sizeof(display), "%lX", (unsigned long)value);
                snprintf(expression, sizeof(expression), "HEX(%s)", currentInput);
            } else {
                unsigned long v = (unsigned long)value;
                char binary[65];
                binary[64] = '\0';
                int pos = 63;
                if (v == 0) binary[pos--] = '0';
                while (v && pos >= 0) {
                    binary[pos--] = '0' + (v & 1UL);
                    v >>= 1UL;
                }
                snprintf(display, sizeof(display), "%s", &binary[pos + 1]);
                snprintf(expression, sizeof(expression), "BIN(%s)", currentInput);
            }
            strcpy(currentInput, display);
            UpdateDisplay();
        }

        else if (id == BTN_PI) {
            sprintf(currentInput, "%.10g", PI);
            strcpy(display, currentInput);
            UpdateDisplay();
        }

        else if (id == BTN_INV) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v == 0) {
                MessageBoxA(hwnd, "Division by zero!", "Error", MB_OK);
                break;
            }
            double result = 1.0 / v;
            sprintf(expression, "1/(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            UpdateDisplay();
        }

        else if (id == BTN_X2) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = v * v;
            sprintf(expression, "(%s)^2", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            UpdateDisplay();
        }

        else if (id == BTN_X3) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = v * v * v;
            sprintf(expression, "(%s)^3", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            UpdateDisplay();
        }

        else if (id == BTN_10X) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = pow(10.0, v);
            sprintf(expression, "10^(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            UpdateDisplay();
        }

        else if (id == BTN_DEG) {
            degreeMode = 1;
            MessageBoxA(hwnd, "Degrees mode", "Angle Mode", MB_OK);
        }

        else if (id == BTN_RAD) {
            degreeMode = 0;
            MessageBoxA(hwnd, "Radians mode", "Angle Mode", MB_OK);
        }

        else if (id == BTN_GRA) {
            MessageBoxA(hwnd, "Gradians mode not implemented yet", "Info", MB_OK);
        }

        else if (id == BTN_SHIFT) {
            MessageBoxA(hwnd, "Shift functionality not implemented yet", "Info", MB_OK);
        }

        else if (id == BTN_OFF) {
            PostQuitMessage(0);
        }

        else if (id == BTN_PLUS) {
            if (strlen(currentInput)) {
                if (currentOperator) {
                    PerformCalculation(hwnd);
                    SetOperator('+', 0);
                } else {
                    SetOperator('+', 1);
                }
            }
        }

        else if (id == BTN_MINUS) {
            if (strlen(currentInput)) {
                if (currentOperator) {
                    PerformCalculation(hwnd);
                    SetOperator('-', 0);
                } else {
                    SetOperator('-', 1);
                }
            }
        }

        else if (id == BTN_MUL) {
            if (strlen(currentInput)) {
                if (currentOperator) {
                    PerformCalculation(hwnd);
                    SetOperator('*', 0);
                } else {
                    SetOperator('*', 1);
                }
            }
        }

        else if (id == BTN_DIV) {
            if (strlen(currentInput)) {
                if (currentOperator) {
                    PerformCalculation(hwnd);
                    SetOperator('/', 0);
                } else {
                    SetOperator('/', 1);
                }
            }
        }

        else if (id == BTN_POWER) {
            if (strlen(currentInput)) {
                if (currentOperator) {
                    PerformCalculation(hwnd);
                    SetOperator('^', 0);
                } else {
                    SetOperator('^', 1);
                }
            }
        }

        else if (id == BTN_EQUAL) {
            if (perm_comb_mode) {
                double r = atof(currentInput);
                double result = 0;
                int ok = 1;
                if (perm_comb_mode == 1) { // permutation
                    if (r > n_value || r < 0 || n_value != (int)n_value || r != (int)r) {
                        MessageBoxA(hwnd, "Invalid permutation", "Error", MB_OK);
                        ok = 0;
                    } else {
                        result = factorial((int)n_value) / factorial((int)(n_value - r));
                        sprintf(expression, "P(%.0f, %.0f)", n_value, r);
                    }
                } else if (perm_comb_mode == 2) { // combination
                    if (r > n_value || r < 0 || n_value != (int)n_value || r != (int)r) {
                        MessageBoxA(hwnd, "Invalid combination", "Error", MB_OK);
                        ok = 0;
                    } else {
                        result = factorial((int)n_value) / (factorial((int)r) * factorial((int)(n_value - r)));
                        sprintf(expression, "C(%.0f, %.0f)", n_value, r);
                    }
                }
                if (ok) {
                    sprintf(display, "%.0f", result);
                    strcpy(currentInput, display);
                    resultShown = 1;
                    currentOperator = '\0';
                    perm_comb_mode = 0;
                    UpdateDisplay();
                } else {
                    ClearAll();
                }
            } else if (currentOperator) {
                PerformCalculation(hwnd);
            }
        }

        else if (id == BTN_PERM) {
            if (strlen(currentInput) == 0) break;
            n_value = atof(currentInput);
            perm_comb_mode = 1;
            strcpy(currentInput, "");
            strcpy(display, "");
            sprintf(expression, "P(%.0f, r)", n_value);
            UpdateDisplay();
        }

        else if (id == BTN_COMB) {
            if (strlen(currentInput) == 0) break;
            n_value = atof(currentInput);
            perm_comb_mode = 2;
            strcpy(currentInput, "");
            strcpy(display, "");
            sprintf(expression, "C(%.0f, r)", n_value);
            UpdateDisplay();
        }

        else if (id == BTN_DOT) {
            if (resultShown) {
                ClearAll();
            }
            if (newNumber) {
                strcpy(currentInput, "");
                newNumber = 0;
            }
            if (!strchr(currentInput, '.')) {
                if (strlen(currentInput) == 0) strcpy(currentInput, "0.");
                else strcat(currentInput, ".");
                strcpy(display, currentInput);
                UpdateDisplay();
            }
        }

        else if (id == BTN_DEG_RAD) {
            degreeMode = !degreeMode;
            MessageBoxA(hwnd, degreeMode ? "Radians mode" : "Degrees mode", "Angle Mode", MB_OK);
        }

        else if (id == BTN_SIN) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = sin(toRadians(v));
            sprintf(expression, "sin(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_COS) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = cos(toRadians(v));
            sprintf(expression, "cos(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_TAN) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (fabs(cos(toRadians(v))) < 1e-12) {
                MessageBoxA(hwnd, "Undefined tan()", "Error", MB_OK);
            } else {
                double result = tan(toRadians(v));
                sprintf(expression, "tan(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_SQRT) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v < 0) MessageBoxA(hwnd, "Invalid sqrt", "Error", MB_OK);
            else {
                double result = sqrt(v);
                sprintf(expression, "sqrt(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_LOG) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v <= 0) {
                MessageBoxA(hwnd, "Invalid log", "Error", MB_OK);
            } else {
                double result = log10(v);
                sprintf(expression, "log(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_LN) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v <= 0) {
                MessageBoxA(hwnd, "Invalid ln", "Error", MB_OK);
            } else {
                double result = log(v);
                sprintf(expression, "ln(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_ASIN) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v < -1 || v > 1) {
                MessageBoxA(hwnd, "Invalid asin", "Error", MB_OK);
            } else {
                double result = toDegrees(asin(v));
                sprintf(expression, "asin(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_ACOS) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v < -1 || v > 1) {
                MessageBoxA(hwnd, "Invalid acos", "Error", MB_OK);
            } else {
                double result = toDegrees(acos(v));
                sprintf(expression, "acos(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_ATAN) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = toDegrees(atan(v));
            sprintf(expression, "atan(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_SINH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = sinh(v);
            sprintf(expression, "sinh(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_COSH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = cosh(v);
            sprintf(expression, "cosh(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_TANH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = tanh(v);
            sprintf(expression, "tanh(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_ASINH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            double result = asinh(v);
            sprintf(expression, "asinh(%s)", currentInput);
            sprintf(display, "%.10g", result);
            strcpy(currentInput, display);
            resultShown = 1;
            currentOperator = '\0';
            UpdateDisplay();
        }

        else if (id == BTN_ACOSH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v < 1) {
                MessageBoxA(hwnd, "Invalid acosh", "Error", MB_OK);
            } else {
                double result = acosh(v);
                sprintf(expression, "acosh(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_ATANH) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v <= -1 || v >= 1) {
                MessageBoxA(hwnd, "Invalid atanh", "Error", MB_OK);
            } else {
                double result = atanh(v);
                sprintf(expression, "atanh(%s)", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        else if (id == BTN_FACT) {
            if (strlen(currentInput) == 0) break;
            double v = atof(currentInput);
            if (v < 0 || v != (int)v) {
                MessageBoxA(hwnd, "Invalid factorial", "Error", MB_OK);
            } else {
                double result = (double)factorial((int)v);
                sprintf(expression, "%s!", currentInput);
                sprintf(display, "%.10g", result);
                strcpy(currentInput, display);
                resultShown = 1;
                currentOperator = '\0';
                UpdateDisplay();
            }
        }

        break;
    }

    case WM_CTLCOLORSTATIC: {
    HDC hdc = (HDC)wParam;
    HWND ctrl = (HWND)lParam;

    static HBRUSH hDisplayBrush = NULL;

    if (!hDisplayBrush) {
        hDisplayBrush = CreateSolidBrush(COLOR_DISPLAY_BG);
    }

    if (ctrl == g_expressionDisplay || ctrl == g_resultDisplay) {
        SetBkColor(hdc, COLOR_DISPLAY_BG);
        SetTextColor(hdc, COLOR_DISPLAY_TEXT);
        return (LRESULT)hDisplayBrush;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        if (pdis->CtlType == ODT_BUTTON) {
            DrawRoundedButton(pdis);
        }
        return TRUE;
    }

    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH hBrush = CreateSolidBrush(COLOR_BG);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        return TRUE;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// ---------------- MAIN ----------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    (void)hPrev;
    (void)lpCmd;
    WNDCLASSA wc = {0};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CalcApp";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(CreateSolidBrush(COLOR_BG));

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA("CalcApp", "Calculator v1.0",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        200, 100, 760, 720,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nShow);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}
