//--------------------------------------------------------------------------------
//
// ImageLib Windows (GDI) Test Source
// Copyright (C) 2000-2001 by Denton Woods
// Last modified:  05/23/2001 <--Y2K Compliant! =]
//
// Filename: testil/windowstest/windowstest.c
//
// Description:  Full GDI test application for DevIL.
//
// Note:  This requires FluidStudio's Colour Picker library to compile
//			properly (available at http://www.fluidstudios.com/publications.html).
//
//--------------------------------------------------------------------------------

#ifdef  _DEBUG
#define IL_DEBUG
#endif//_DEBUG

#include <windows.h>
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
//#include <sdl.h>
#include "resource.h"

//#pragma comment(lib, "sdl.lib")
//#pragma comment(lib, "sdlmain.lib")


// Evil globals!
HINSTANCE hInstance;
HBITMAP hBitmap;
BITMAPINFOHEADER BmpInfo;
HDC hDC = 0, hMemDC = 0;
HWND HWnd;

#define BORDER_W	8
#define MENU_H		46
#define MIN_W		205  // Accomodate the menu bar.
#define MAX_W		400
#define MAX_H		400
#define TITLE		"DevIL Windows Test"
ILuint	NumUndosAllowed = 4, UndoSize = 0;
ILuint	Undos[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
ILuint	Width, Height, Depth;  // Main image

ILint	XOff, YOff;

ILdouble last_elapsed, cur_elapsed, elapsed;

ILuint	FilterType;
ILuint	FilterParamInt;
ILfloat	FilterParamFloat;
char	FilterEditString[255];

char	NewTitle[512];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL APIENTRY FilterDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL APIENTRY ResizeDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL APIENTRY BatchDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);
void BatchConv(char *Directory, char *ExtList, char *ConvExt, bool Recurse);
void GenFilterString(char *Out, char **Strings);
void ResizeWin(void);
void CreateGDI(void);
bool IsOpenable(char *FileName);

// Colour picker export
__declspec( dllexport ) bool WINAPI FSColorPickerDoModal(unsigned int * currentColor, unsigned int * originalColor, const int initialExpansionState);




int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG			msg;
	WNDCLASSEX	wcex;
	HACCEL		hAccelTable;

	hInstance = hInst;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDR_MENU1;
	wcex.lpszClassName	= TITLE;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);

	RegisterClassEx(&wcex);

	HWnd = CreateWindow(TITLE, TITLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						50, 50, 400, 300, NULL, NULL, hInstance, NULL);
	if (HWnd == NULL)
		return FALSE;

	//if (SDL_Init(SDL_INIT_TIMER) < 0)
	//	return FALSE;
	//atexit(SDL_Quit);

	// Display the window
	ShowWindow(HWnd, nCmdShow);
	UpdateWindow(HWnd);

	// Initialize DevIL
	ilInit();
	iluInit();
	ilutRenderer(ILUT_WIN32);

	// Is there a file to load from the command-line?
	if (__argc > 1) {
		ilGenImages(1, Undos);
		ilBindImage(Undos[0]);
		if (ilLoadImage(__argv[1])) {
			//ilConvertImage(IL_BGRA);
			ilutRenderer(ILUT_WIN32);
			ResizeWin();
			CreateGDI();
			sprintf(NewTitle, "%s - %s", TITLE, __argv[1]);
			SetWindowText(HWnd, NewTitle);
		}
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDR_MENU1);

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


void CreateGDI()
{
	ILuint CopyName, CurName, CurImg, CurMip;

	hDC = GetDC(HWnd);
	hMemDC = CreateCompatibleDC(hDC);
	CurName = ilGetInteger(IL_CUR_IMAGE);
	CurImg = ilGetInteger(IL_ACTIVE_IMAGE);
	CurMip = ilGetInteger(IL_ACTIVE_MIPMAP);
	CopyName = ilCloneCurImage();
	ilBindImage(CopyName);
	ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
	hBitmap = ilutConvertToHBitmap(hDC);
	ilutGetBmpInfo((BITMAPINFO*)&BmpInfo);
	DeleteObject(SelectObject(hMemDC, hBitmap));
	ilBindImage(CurName);
	if (CurImg)
		ilActiveImage(CurImg);
	if (CurMip)
		ilActiveMipmap(CurMip);
	ilDeleteImages(1, &CopyName);

	return;
}


void DestroyGDI()
{
	if (hMemDC) {
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);
	}
	if (hDC) {
		ReleaseDC(HWnd, hDC);
	}
	hBitmap = NULL;
	hMemDC = NULL;
	hDC = NULL;

	return;
}


void ResizeWin()
{
	static RECT Rect1, Rect2;
	static ILint NewW, NewH;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &Rect1, 0);

	GetWindowRect(HWnd, &Rect2);

	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	Depth = ilGetInteger(IL_IMAGE_DEPTH);

	NewW = Width < MIN_W ? MIN_W : Width + BORDER_W;
	if (NewW + Rect2.left > Rect1.right)
		NewW = Rect1.right - Rect2.left;
	NewH = Height + MENU_H;
	if (NewH + Rect2.top > Rect1.bottom)
		NewH = Rect1.bottom - Rect2.top;

	SetWindowPos(HWnd, HWND_TOP, Rect2.left, Rect2.top, NewW, NewH, SWP_SHOWWINDOW);
	InvalidateRect(HWnd, NULL, FALSE);

	return;
}


ILubyte *RemoveMe;

// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU		hMenu;
	static ILuint		Colours;
	static RECT			Rect;
	static PAINTSTRUCT	ps;
    static HDROP		hDrop;

	static char OpenFileName[2048];
	static char OpenFilter[2048];
	static char SaveFileName[2048];
	static char SaveFilter[2048];
	static char *OFilter[] = {
		"All Files (*.*)", "*.*",
		"Graphics Interchange Format (*.gif)", "*.gif",
		"Half-Life Model Files (*.mdl)", "*.mdl",
		"Homeworld Image Files (*.lif)", "*.lif",
		"Image Files (All Supported Types)", "*.jpe;*.jpg;*.jpeg;*.lif;*.bmp;*.ico;*.pbm;*.pgm;*.pnm;*.ppm;*.png;*.bw;*.rgb;*.rgba;*.sgi;*.tga;*.tif;*.tiff;*.pcx",
		"Jpeg Files (*.jpe, *.jpg, *.jpeg)", "*.jpe;*.jpg;*.jpeg",
		"Microsoft Bitmap Files (*.bmp)", "*.bmp",
		"Microsoft DirectDraw Surface (*.dds)", "*.dds",
		"Microsoft Icon Files (*.ico)", "*.ico",
		"DevIL Files (*.oil)", "*.oil",
		"Portable AnyMap Files (*.pbm, *.pgm, *.pnm, *.ppm)", "*.pbm;*.pgm;*.pnm;*.ppm",
		"Portable Network Graphics Files (*.png)", "*.png",
		"Sgi Files (*.sgi)", "*.bw;*.rgb;*.rgba;*.sgi",
		"Targa Files (*.tga)", "*.tga",
		"Tiff Files (*.tif)", "*.tif;*.tiff",
		"Quake Wal Files (*.wal)", "*.wal",
		"ZSoft Pcx Files (*.pcx)", "*.pcx",
		"\0\0"
	};
	static char *SFilter[] = {
		"All Files (*.*)", "*.*",
		"C-Style Header (*.h)", "*.h",
		"Jpeg Files (*.jpe, *.jpg, *.jpeg)", "*.jpe;*.jpg;*.jpeg",
		"Microsoft Bitmap Files (*.bmp)", "*.bmp",
		"DevIL Files (*.oil)", "*.oil",
		"Portable AnyMap Files (*.pbm, *.pgm, *.ppm)", "*.pbm;*.pgm;*.ppm",
		"Portable Network Graphics Files (*.png)", "*.png",
		"Sgi Files (*.sgi)", "*.bw;*.rgb;*.rgba;*.sgi",
		"Targa Files (*.tga)", "*.tga",
		"ZSoft Pcx Files (*.pcx)", "*.pcx",
		"\0\0"
	};

	static OPENFILENAME Ofn = {
		sizeof(OPENFILENAME),
		hWnd,
		NULL,
		OpenFilter,
		NULL,
		0,
		0,
		OpenFileName,
		2048,
		NULL,
		0,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		0,
		0,
		NULL,
		NULL,
		NULL,
		NULL
	};

	POINT  CurMouse;
	static POINT		PrevMouse;
	static ILboolean	MouseDown = IL_FALSE;

	static RECT			WinSize;
	static COLORREF		BackColour;
	static HBRUSH		BackBrush;

	unsigned int	currentColor = 0x80000000;
	unsigned int	originalColor = 0x80000000;
	bool		userClickedOK;
	ILclampf	Red, Green, Blue;

	switch (message)
	{
		case WM_CREATE:
			GenFilterString(OpenFilter, OFilter);
			GenFilterString(SaveFilter, SFilter);

			hDC = GetDC(hWnd);
			DragAcceptFiles(hWnd, TRUE);

			//BackColour = GetBkColor(hDC);
			BackColour = 0xff808080;
			BackBrush = CreateSolidBrush(BackColour);

			ReleaseDC(hWnd, hDC);

			/*BackHDC = CreateCompatibleDC(hDC);
			Background = CreateCompatibleBitmap(BackHDC, 0, 0);
			SelectObject(BackHDC, Background);*/
			break;

		case WM_CLOSE:
			DestroyGDI();
			/*DeleteObject(SelectObject(BackHDC, Background));
			DeleteDC(BackHDC);*/
			DestroyWindow(hWnd);
			UnregisterClass(TITLE, hInstance);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			GetWindowRect(HWnd, &WinSize);  // Shouldn't be here!
			hDC = BeginPaint(hWnd, &ps);
			/*StretchBlt(hDC, 0, 0, WinSize.right - WinSize.left,
				WinSize.bottom - WinSize.top, BackHDC, 0, 0, 1, 1, SRCCOPY);*/
			WinSize.right -= WinSize.left;
			WinSize.bottom -= WinSize.top;
			WinSize.top = 0;
			WinSize.left = 0;
			FillRect(hDC, &WinSize, BackBrush);
            BitBlt(hDC, XOff, YOff, (WORD)BmpInfo.biWidth, (WORD)BmpInfo.biHeight, 
				  hMemDC, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			ValidateRect(hWnd, NULL);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);

			InvalidateRect(hWnd, NULL, FALSE);
			break;

		// Moves the "viewport"
		case WM_MOUSEMOVE:
			if (!MouseDown)
				break;
			GetCursorPos(&CurMouse);

			XOff += CurMouse.x - PrevMouse.x;
			YOff += CurMouse.y - PrevMouse.y;

			PrevMouse.x = CurMouse.x;
			PrevMouse.y = CurMouse.y;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_LBUTTONDOWN:
			MouseDown = IL_TRUE;
			GetCursorPos(&PrevMouse);
			break;

		case WM_LBUTTONUP:
			MouseDown = IL_FALSE;
			break;

		case WM_DROPFILES:
			hDrop = (HDROP)wParam;
			DragQueryFile(hDrop, 0, OpenFileName, 512);

			DestroyGDI();
			ilDeleteImages(UndoSize, Undos);
			UndoSize = 0;

			ilGenImages(1, Undos);
			ilBindImage(Undos[0]);
			ilLoadImage(OpenFileName);

			ilutRenderer(ILUT_WIN32);
			ResizeWin();
			CreateGDI();
			
			sprintf(NewTitle, "%s - %s", TITLE, OpenFileName);
			SetWindowText(hWnd, NewTitle);

			DragFinish(hDrop);
			return 0;

		case WM_COMMAND:
			FilterType = LOWORD(wParam);
	
			switch (LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return (0L);

				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						AboutDlgProc);
					return (0L);

				case ID_BATCHCONVERT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_BATCHCONV),
						hWnd,
						BatchDlgProc);
					return (0L);

				case ID_EFFECTS_COUNTCOLORS:
					Colours = iluColoursUsed();
					char ColourString[255];
					sprintf(ColourString, "The number of colours in this image is:  %d", Colours);
					MessageBox(NULL, ColourString, "Colour Count", MB_OK);
					return (0L);

				case ID_EFFECTSTOOLS_BACKGROUNDCOLOUR:
					userClickedOK = FSColorPickerDoModal(&currentColor, &originalColor, 0);

					if (userClickedOK) {
						Red = (ILfloat)((currentColor & 0xff0000) >> 16) / 255.0f;
						Green = (ILfloat)((currentColor & 0xff00) >> 8) / 255.0f;
						Blue = (ILfloat)(currentColor & 0xff) / 255.0f;

						ilClearColour(Red, Green, Blue, 1.0f);
					}

					return (0L);

				case ID_EDIT_COPY:
					ilutSetWinClipboard();
					return (0L);

				case ID_EDIT_PASTE:
					ILuint Test;
					ilGenImages(1, &Test);
					ilBindImage(Test);

					// Check if there's anything in the clipboard.
					if (!ilutGetWinClipboard()) {
						ilDeleteImages(1, &Test);
						return (0L);
					}
					ilDeleteImages(1, &Test);

					DestroyGDI();
					ilDeleteImages(UndoSize, Undos);
					UndoSize = 0;
					XOff = 0;
					YOff = 0;

					ilGenImages(1, Undos);
					ilBindImage(Undos[0]);
					ilutGetWinClipboard();

					sprintf(NewTitle, "%s - Pasted from the Clipboard", TITLE);
					SetWindowText(hWnd, NewTitle);

					//ilConvertImage(IL_BGRA);
					ilutRenderer(ILUT_WIN32);
					ResizeWin();
					CreateGDI();
					return (0L);

				// @TODO:  Will probably fail if no image loaded!
				case ID_FILE_PRINT:
					/*PRINTDLG	Pd;
					DOCINFO		Di;
					//HDC			PrintDC;
					//HBITMAP		PrintReplace;

					memset(&Pd, 0, sizeof(PRINTDLG));
					Pd.lStructSize = sizeof(PRINTDLG);
					Pd.hwndOwner = hWnd;
					Pd.Flags = PD_RETURNDC;
					Pd.nCopies = 1;
					Pd.nFromPage = 0xFFFF;
					Pd.nToPage = 0xFFFF;
					Pd.nMinPage = 1;
					Pd.nMaxPage = 0xFFFF;

					if (!PrintDlg(&Pd))
						return (0L);

					Di.cbSize = sizeof(DOCINFO);
					Di.lpszDocName = "DevIL Printing Test";
					Di.lpszOutput = NULL;
					Di.lpszDatatype = NULL;
					Di.fwType = 0;

					StartDoc(Pd.hDC, &Di);
					StartPage(Pd.hDC);

					//PrintDC = CreateCompatibleDC(Pd.hDC);
					//PrintReplace = (HBITMAP)SelectObject(PrintDC, hBitmap);
					StretchBlt(Pd.hDC, 0, 0, Width * 2, Height * 2, hMemDC, 0, 0, Width, Height, SRCCOPY);

					EndPage(Pd.hDC);
					EndDoc(Pd.hDC);
					//DeleteObject(PrintReplace);
					//DeleteDC(PrintDC);
					DeleteDC(Pd.hDC);*/

					ilutWinPrint(0, 0, ilGetInteger(IL_IMAGE_WIDTH) * 2, ilGetInteger(IL_IMAGE_HEIGHT) * 2, hDC);

					return (0L);

				case ID_FILE_LOAD:
					sprintf(OpenFileName, "*.*");
					Ofn.lpstrFilter = OpenFilter;
					Ofn.lpstrFile = OpenFileName;
					Ofn.lpstrTitle = "Open File";
					Ofn.nFilterIndex = 1;
					Ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

					if (!GetOpenFileName(&Ofn))
						return (0L);

					DestroyGDI();
					ilDeleteImages(UndoSize, Undos);
					UndoSize = 0;
					XOff = 0;
					YOff = 0;

					ilGenImages(1, Undos);
					ilBindImage(Undos[0]);

					//last_elapsed = SDL_GetTicks();
					ilLoadImage(OpenFileName);
					//cur_elapsed = SDL_GetTicks();
					//elapsed = cur_elapsed - last_elapsed;
					//last_elapsed = cur_elapsed;
					elapsed = 0;

					ilutRenderer(ILUT_WIN32);
					ResizeWin();
					CreateGDI();
					
					sprintf(NewTitle, "%s - %s:  %g ms", TITLE, OpenFileName, elapsed);
					SetWindowText(hWnd, NewTitle);

					return (0L);

				case ID_FILE_OPENURL:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) != TRUE) {
						return (0L);
					}

					DestroyGDI();
					ilDeleteImages(UndoSize, Undos);
					UndoSize = 0;
					XOff = 0;
					YOff = 0;

					ilGenImages(1, Undos);
					ilBindImage(Undos[0]);
					if (!ilutWinLoadUrl(FilterEditString))
						return (0L);

					ilutRenderer(ILUT_WIN32);
					ResizeWin();
					CreateGDI();
					
					sprintf(NewTitle, "%s - %s", TITLE, FilterEditString);
					SetWindowText(hWnd, NewTitle);

					return (0L);

				case ID_FILE_SAVE:
					sprintf(SaveFileName, "monkey.tga");
					Ofn.lpstrFilter = SaveFilter;
					Ofn.lpstrFile = SaveFileName;
					Ofn.lpstrTitle = "Save File";
					Ofn.nFilterIndex = 1;
					Ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

					if (!GetSaveFileName(&Ofn))
						return (0L);

					ilEnable(IL_FILE_OVERWRITE);
					ilSaveImage(SaveFileName);

					sprintf(NewTitle, "%s - %s", TITLE, SaveFileName);
					SetWindowText(hWnd, NewTitle);

					return (0L);

				case ID_EDIT_UNDOLEVEL:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						NumUndosAllowed = FilterParamInt <= 10 ? FilterParamInt : 10;
					}
					return (0L);

				case ID_EDIT_UNDO:
					if (UndoSize && NumUndosAllowed) {
						ilDeleteImages(1, &Undos[UndoSize]);
						ilBindImage(Undos[--UndoSize]);
						ResizeWin();
						CreateGDI();
					}
					return (0L);

				case ID_EDIT_VIEWIMAGENUM:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						ilActiveImage(FilterParamInt);
						ilutRenderer(ILUT_WIN32);
						ResizeWin();
						CreateGDI();
					}
					return (0L);

				case ID_EDIT_VIEWMIPMAP:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						ilActiveMipmap(FilterParamInt);
						ilutRenderer(ILUT_WIN32);
						ResizeWin();
						CreateGDI();
					}
					return (0L);

			}


			if (++UndoSize > NumUndosAllowed) {
				if (NumUndosAllowed > 0) {
					UndoSize = NumUndosAllowed;
					ilDeleteImages(1, &Undos[0]);
					memcpy(Undos, Undos+1, NumUndosAllowed * sizeof(ILuint));
					ilBindImage(Undos[UndoSize]);
				}
			}

			if (NumUndosAllowed > 0) {
				ilGetIntegerv(IL_ACTIVE_IMAGE, (ILint*)&Undos[UndoSize]);
				/*ilGenImages(1, &Undos[UndoSize]);
				ilBindImage(Undos[UndoSize]);
				ilCopyImage(Undos[UndoSize-1]);*/
				Undos[UndoSize] = ilCloneCurImage();
				ilBindImage(Undos[UndoSize]);
			}

			DestroyGDI();
			switch (LOWORD(wParam))
			{
				case ID_CONVERT_PALETTE:
					ilConvertImage(IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
					break;

				case ID_CONVERT_RGB:
					ilConvertImage(IL_RGB, ilGetInteger(IL_IMAGE_TYPE));
					break;

				case ID_CONVERT_RGBA:
					ilConvertImage(IL_RGBA, ilGetInteger(IL_IMAGE_TYPE));
					break;

				case ID_CONVERT_BGR:
					ilConvertImage(IL_BGR, ilGetInteger(IL_IMAGE_TYPE));
					break;

				case ID_CONVERT_BGRA:
					ilConvertImage(IL_BGRA, ilGetInteger(IL_IMAGE_TYPE));
					break;

				case ID_CONVERT_LUMINANCE:
					ilConvertImage(IL_LUMINANCE, ilGetInteger(IL_IMAGE_TYPE));
					break;

				case ID_EFFECTS_FLIP:
					iluFlipImage();
					break;

				case ID_EFFECTS_MIRROR:
					iluMirror();
					break;

				case ID_FILTER_EMBOSS:
					iluEmboss();
					break;

				case ID_FILTER_EQUALIZE:
					iluEqualize();
					break;

				case ID_FILTER_ALIENIFY:
					iluAlienify();
					break;

				case ID_FILTER_NEGATIVE:
					iluNegative();
					break;

				case ID_EFFECTS_FILTERS_EDGEDETECT_EMBOSS:
					iluEdgeDetectE();
					break;

				case ID_EFFECTS_FILTERS_EDGEDETECT_SOBEL:
					iluEdgeDetectS();
					break;

				case ID_EFFECTS_FILTERS_EDGEDETECT_PREWITT:
					iluEdgeDetectP();
					break;

				case ID_FILTER_NOISE:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluNoisify(FilterParamFloat);
					}
					break;

				case ID_EFFECTS_FILTERS_WAVE:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluWave(FilterParamFloat);
					}
					break;

				case ID_FILTER_PIXELIZE:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluPixelize(FilterParamInt);
					}
					break;

				case ID_FILTERS_BLUR_AVERAGE:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluBlurAvg(FilterParamInt);
					}
					break;

				case ID_FILTERS_BLUR_GAUSSIAN:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluBlurGaussian(FilterParamInt);
						/*iluMatrixMode(ILU_CONVOLUTION_MATRIX);
						iluLoadFilter(ILU_FILTER_GAUSSIAN_5X5);
						iluApplyMatrix();*/
					}
					break;

				case ID_FILTER_GAMMACORRECT:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluGammaCorrect(FilterParamFloat);
					}
					break;

				case ID_FILTER_SHARPEN:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluSharpen(FilterParamFloat, 1);
					}
					break;

				case ID_EFFECTS_FILTERS_ROTATE:
					if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_FILTER),
						hWnd, FilterDlgProc) == TRUE) {
						iluRotate(FilterParamFloat);
						ResizeWin();
					}
					break;

				case ID_EFFECTS_FILTERS_SCALE:
					HWnd = hWnd;
					iluImageParameter(ILU_FILTER, ILU_BILINEAR);
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_RESIZE), hWnd, ResizeDlgProc);
					break;

			}

			CreateGDI();
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		default:
		  return (DefWindowProc(hWnd, message, wParam, lParam));
	}

    return (0L);
}


void GenFilterString(char *Out, char **Strings)
{
	int OutPos = 0, StringPos = 0;

	while (Strings[StringPos][0] != 0) {
		sprintf(Out + OutPos, Strings[StringPos]);
		OutPos += strlen(Strings[StringPos++]) + 1;
	}

	Out[OutPos++] = 0;
	Out[OutPos] = 0;

	return;
}


BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
	{
	    case WM_INITDIALOG:
		{
			int i;
			ILenum ilError;
			char VersionNum[256];

			sprintf(VersionNum, "Num:  %d", ilGetInteger(IL_VERSION_NUM));

			SetDlgItemText(hDlg, IDC_ABOUT_VENDOR, ilGetString(IL_VENDOR));
			SetDlgItemText(hDlg, IDC_ABOUT_VER_STRING, ilGetString(IL_VERSION));
			SetDlgItemText(hDlg, IDC_ABOUT_VER_NUM, VersionNum);

			for (i = 0; i < 6; i++) {
				ilError = ilGetError();
				if (ilError == IL_NO_ERROR)
					break;
				SetDlgItemText(hDlg, IDC_ERROR1+i, iluErrorString(ilError));
			}

			return (TRUE);
		}
		break;

	    case WM_COMMAND:      
		{
			if (LOWORD(wParam) == IDOK)
				EndDialog(hDlg, TRUE);
			if (LOWORD(wParam) == IDCANCEL)
				EndDialog(hDlg, FALSE);
	    }
		break;

		case WM_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
	}

	return FALSE;
}


BOOL APIENTRY FilterDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
	{
	    case WM_INITDIALOG:
		{
			switch (FilterType)
			{
				case ID_FILTER_PIXELIZE:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Width of pixelized block:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1");
					break;
				case ID_FILTER_NOISE:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Amount of noise threshold:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1.0");
					break;
				case ID_EFFECTS_FILTERS_WAVE:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Angle of wave to apply:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "0.0");
					break;
				case ID_FILTERS_BLUR_AVERAGE:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Number of iterations:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1");
					break;
				case ID_FILTERS_BLUR_GAUSSIAN:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Number of iterations:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1");
					break;
				case ID_FILTER_GAMMACORRECT:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Amount of gamma correction:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1.0");
					break;
				case ID_FILTER_SHARPEN:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Sharpening factor:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "1.0");
					break;
				case ID_EFFECTS_FILTERS_ROTATE:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Number of degress to rotate:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "0.0");
					break;

				case ID_EDIT_UNDOLEVEL:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Set level of undo:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "4");
					break;
				case ID_EDIT_VIEWIMAGENUM:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Enter image number:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "0");
					break;
				case ID_EDIT_VIEWMIPMAP:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Enter mipmap number:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "0");
					break;
				case ID_FILE_OPENURL:
					SetDlgItemText(hDlg, IDC_FILTER_DESC_TEXT, "Enter url of image:");
					SetDlgItemText(hDlg, IDC_FILTER_EDIT, "");
					break;
			}

			return TRUE;
		}
		break;

	    case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK) {
				GetDlgItemText(hDlg, IDC_FILTER_EDIT, FilterEditString, 255);
				FilterParamInt = atoi(FilterEditString);
				FilterParamFloat = (float)atof(FilterEditString);
				EndDialog(hDlg, TRUE);
			}
			if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, FALSE);
			}
	    }
		break;

		case WM_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
	}

	return FALSE;
}


BOOL APIENTRY ResizeDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	static char x[255], y[255], z[255];
	static ILuint xsize, ysize, zsize;
	static RECT Rect;

    switch (message)
	{
	    case WM_INITDIALOG:
		{
			sprintf(x, "%d", Width);
			sprintf(y, "%d", Height);
			sprintf(z, "%d", Depth);
			SetDlgItemText(hDlg, IDC_EDIT_RESIZE_X, x);
			SetDlgItemText(hDlg, IDC_EDIT_RESIZE_Y, y);
			SetDlgItemText(hDlg, IDC_EDIT_RESIZE_Z, z);
			return TRUE;
		}
		break;

	    case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK) {
				GetDlgItemText(hDlg, IDC_EDIT_RESIZE_X, x, 255);
				GetDlgItemText(hDlg, IDC_EDIT_RESIZE_Y, y, 255);
				GetDlgItemText(hDlg, IDC_EDIT_RESIZE_Z, z, 255);
				xsize = atoi(x);
				ysize = atoi(y);
				zsize = atoi(z);
				if (xsize && ysize && zsize) {
					iluScale(xsize, ysize, zsize);

					Width = ilGetInteger(IL_IMAGE_WIDTH);
					Height = ilGetInteger(IL_IMAGE_HEIGHT);
					Depth = ilGetInteger(IL_IMAGE_DEPTH);

					GetWindowRect(HWnd, &Rect);
					SetWindowPos(HWnd, HWND_TOP, Rect.left, Rect.top,
						Width < MIN_W ? MIN_W + BORDER_W : Width + BORDER_W,
						Height + MENU_H, SWP_SHOWWINDOW);

					InvalidateRect(HWnd, NULL, FALSE);
				}
				EndDialog(hDlg, TRUE);
			}
			if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, FALSE);
			}
	    }
		break;

		case WM_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
	}

	return FALSE;
}


BOOL APIENTRY BatchDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	static char	Dir[255], NewExt[255];
	static bool	Recurse;
	static RECT	Rect;

    switch (message)
	{
	    case WM_INITDIALOG:
		{
			sprintf(Dir, "");
			sprintf(NewExt, "oil");
			SetDlgItemText(hDlg, IDC_BATCH_DIR, Dir);
			SetDlgItemText(hDlg, IDC_BATCH_NEWEXT, NewExt);
			return TRUE;
		}
		break;

	    case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK) {
				GetDlgItemText(hDlg, IDC_BATCH_DIR, Dir, 255);
				GetDlgItemText(hDlg, IDC_BATCH_NEWEXT, NewExt, 255);
				Recurse = IsDlgButtonChecked(hDlg, IDC_BATCH_CHECK1) == BST_CHECKED;

				// Do shit here.

				BatchConv(Dir, NULL, NewExt, Recurse);

				EndDialog(hDlg, TRUE);
			}
			if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, FALSE);
			}
	    }
		break;

		case WM_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
	}

	return FALSE;
}
