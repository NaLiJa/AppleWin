#pragma once

#include "FrameBase.h"
#include "DiskImage.h"
#include "Card.h"

class Video;

#if 0 // enable non-integral full-screen scaling
#define FULLSCREEN_SCALE_TYPE float
#else
#define FULLSCREEN_SCALE_TYPE int
#endif

// 3D border around the 560x384 Apple II display
#define  VIEWPORTX   5
#define  VIEWPORTY   5

#define  BUTTONX     (g_nViewportCX + VIEWPORTX*2)
#define  BUTTONY     0
#define  BUTTONCX    45
#define  BUTTONCY    45
#define  BUTTONS     8


class Win32Frame : public FrameBase
{
public:
	Win32Frame(void);
	virtual ~Win32Frame(void) {}

	static Win32Frame& GetWin32Frame();
	static LRESULT CALLBACK FrameWndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

	virtual void FrameDrawDiskLEDS();
	virtual void FrameDrawDiskStatus();

	virtual void FrameRefreshStatus(int drawflags);
	virtual void FrameUpdateApple2Type();
	virtual void FrameSetCursorPosByMousePos();

	virtual void SetFullScreenShowSubunitStatus(bool bShow);
	virtual void SetWindowedModeShowDiskiiStatus(bool bShow);
	virtual bool GetBestDisplayResolutionForFullScreen(UINT& bestWidth, UINT& bestHeight, UINT userSpecifiedWidth = 0, UINT userSpecifiedHeight = 0);
	virtual int SetViewportScale(int nNewScale, bool bForce = false);
	virtual void SetAltEnterToggleFullScreen(bool mode);

	virtual void SetLoadedSaveStateFlag(const bool bFlag);

	virtual void Initialize(bool resetVideoState);
	virtual void Destroy(void);
	virtual void VideoPresentScreen(void);
	virtual void ResizeWindow(void);

	virtual int FrameMessageBox(LPCSTR lpText, LPCSTR lpCaption, UINT uType);
	virtual void GetBitmap(LPCSTR lpBitmapName, LONG cb, LPVOID lpvBits);
	virtual BYTE* GetResource(WORD id, LPCSTR lpType, uint32_t expectedSize);
	virtual void Restart();

	virtual std::string Video_GetScreenShotFolder() const;

	virtual std::shared_ptr<NetworkBackend> CreateNetworkBackend(const std::string& interfaceName);

	bool GetFullScreenShowSubunitStatus(void);
	bool GetWindowedModeShowDiskiiStatus(void);
	int GetFullScreenOffsetX(void);
	int GetFullScreenOffsetY(void);
	bool IsFullScreen(void);
	void FrameRegisterClass();
	void FrameCreateWindow(void);
	void ChooseMonochromeColor(void);
	UINT Get3DBorderWidth(void);
	UINT Get3DBorderHeight(void);
	int GetViewportScale(void);
	void GetViewportCXCY(int& nViewportCX, int& nViewportCY);
	void SetFullScreenViewportScale(int nNewXScale, int nNewYScale);

	void ApplyVideoModeChange(void);

	HDC FrameGetDC();
	void FrameReleaseDC();

	bool	g_bScrollLock_FullSpeed;

private:
	static BOOL CALLBACK DDEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);
	LRESULT WndProc(HWND   window, UINT   message, WPARAM wparam, LPARAM lparam);

	void VideoCreateDIBSection(bool resetVideoState);
	void VideoDrawLogoBitmap(HDC hDstDC, int xoff, int yoff, int srcw, int srch, int scale);
	bool DDInit(void);
	void DDUninit(void);

	void Benchmark(void);
	void DisplayLogo(void);
	void GetTrackSector(UINT slot, int& drive1Track, int& drive2Track, int& activeFloppy);
	void CreateTrackSectorStrings(int track, int sector, std::string& strTrack, std::string& strSector);
	void DrawTrackSector(HDC dc, UINT slot, int drive1Track, int drive1Sector, int drive2Track, int drive2Sector);
	void FrameDrawDiskLEDS(HDC hdc);  // overloaded Win32 only, call via GetWin32Frame()
	void FrameDrawDiskStatus(HDC hdc);  // overloaded Win32 only, call via GetWin32Frame()
	void EraseButton(int number);
	void DrawButton(HDC passdc, int number);
	void DrawCrosshairs(int x, int y);
	void DrawFrameWindow(bool bPaintingWindow = false);
	void DrawStatusArea(HDC passdc, int drawflags);
	void Draw3dRect(HDC dc, int x1, int y1, int x2, int y2, BOOL out);
	void DrawBitmapRect(HDC dc, int x, int y, LPRECT rect, HBITMAP bitmap);
	void ProcessButtonClick(int button, bool bFromButtonUI = false);
	bool ConfirmReboot(bool bFromButtonUI);
	void ProcessDiskPopupMenu(HWND hwnd, POINT pt, const int iDrive);
	void RelayEvent(UINT message, WPARAM wparam, LPARAM lparam);
	void SetFullScreenMode(void);
	void SetNormalMode(void);
	void SetUsingCursor(BOOL bNewValue);
	void SetupTooltipControls(void);
	void FrameResizeWindow(int nNewScale);
	void RevealCursor();
	void ScreenWindowResize(const bool bCtrlKey);
	void UpdateMouseInAppleViewport(int iOutOfBoundsX, int iOutOfBoundsY, int x = 0, int y = 0);
	void DrawCrosshairsMouse();
	void FrameSetCursorPosByMousePos(int x, int y, int dx, int dy, bool bLeavingAppleScreen);
	void CreateGdiObjects(void);
	void DeleteGdiObjects(void);
	void FrameShowCursor(BOOL bShow);
	void FullScreenRevealCursor(void);
	void GetWidthHeight(int& nWidth, int& nHeight);
	void SetSlotUIOffsets(void);

	bool g_bAltEnter_ToggleFullScreen; // Default for ALT+ENTER is to toggle between windowed and full-screen modes
	bool    g_bIsFullScreen;
	bool g_bShowingCursor;
	bool g_bLastCursorInAppleViewport;
	UINT_PTR	g_TimerIDEvent_100msec;
	UINT		g_uCount100msec;
	COLORREF      customcolors[256];	// MONOCHROME is last custom color
	HBITMAP       g_hLogoBitmap;
	HBITMAP       g_hDeviceBitmap;
	HDC           g_hDeviceDC;
	LPBITMAPINFO  g_pFramebufferinfo;
	BOOL    g_bUsingCursor;	// TRUE = AppleWin is using (hiding) the mouse-cursor && restricting cursor to window - see SetUsingCursor()
	bool    g_bAppActive;
	bool g_bFrameActive;
	bool g_windowMinimized;
	bool g_bFullScreen_ShowSubunitStatus;
	bool m_showDiskiiStatus;
	bool m_redrawDiskiiStatus;
	int						g_win_fullscreen_offsetx;
	int						g_win_fullscreen_offsety;
	UINT m_bestWidthForFullScreen;
	UINT m_bestHeightForFullScreen;
	bool m_changedDisplaySettings;

	static const UINT MAX_DRAW_DEVICES = 10;
	char* draw_devices[MAX_DRAW_DEVICES];
	GUID draw_device_guid[MAX_DRAW_DEVICES];
	int num_draw_devices;
	LPDIRECTDRAW g_lpDD;

	HBITMAP buttonbitmap[BUTTONS];

	HBRUSH  btnfacebrush;
	HPEN    btnfacepen;
	HPEN    btnhighlightpen;
	HPEN    btnshadowpen;
	int     buttonactive;
	int     buttondown;
	int     buttonover;
	int     buttonx;
	int     buttony;
	HDC     g_hFrameDC;
	RECT    framerect;

	BOOL    helpquit;
	static const UINT smallfontHeight = 11;
	HFONT   smallfont;

	HWND    tooltipwindow;
	std::string driveTooltip;
	enum { TTID_DRIVE1_BUTTON = 0, TTID_DRIVE2_BUTTON, TTID_SLOT6_TRK_SEC_INFO, TTID_SLOT5_TRK_SEC_INFO, TTID_MAX };

	int     viewportx;	// Default to Normal (non-FullScreen) mode
	int     viewporty;	// Default to Normal (non-FullScreen) mode

	RECT	g_main_window_saved_rect;
	int		g_main_window_saved_style;
	int		g_main_window_saved_exstyle;


	HBITMAP g_hCapsLockBitmap[2];
	HBITMAP g_hHardDiskBitmap[2];

	//Pravets8 only
	HBITMAP g_hCapsBitmapP8[2];
	HBITMAP g_hCapsBitmapLat[2];
	//HBITMAP charsetbitmap [4]; //The idea was to add a charset indicator on the front panel, but it was given up. All charsetbitmap occurences must be REMOVED!
	//===========================
	HBITMAP g_hDiskWindowedLED[NUM_DISK_STATUS];

	// Y-offsets from end of last button
	static const UINT yOffsetSlot6LEDNumbers = 5;
	static const UINT yOffsetSlot6LEDs = yOffsetSlot6LEDNumbers + 1;
	static const UINT yOffsetCapsLock = yOffsetSlot6LEDs + smallfontHeight;
	static const UINT yOffsetHardDiskLED = yOffsetSlot6LEDs + smallfontHeight + 1;
	// 2x (or more) Windowed mode: Disk II LEDs and track/sector info
	struct D2FullUI	// Disk II full UI
	{
		static const UINT yOffsetSlot6TrackInfo = 35;
		static const UINT yOffsetSlot6SectorInfo = yOffsetSlot6TrackInfo + smallfontHeight;
		static const UINT yOffsetSlot5Label = yOffsetSlot6SectorInfo + smallfontHeight + 3;
		static const UINT yOffsetSlot5LEDNumbers = yOffsetSlot5Label + smallfontHeight + 1;
		static const UINT yOffsetSlot5LEDs = yOffsetSlot5LEDNumbers + 1;
		static const UINT yOffsetSlot5TrackInfo = yOffsetSlot5LEDs + smallfontHeight;
		static const UINT yOffsetSlot5SectorInfo = yOffsetSlot5TrackInfo + smallfontHeight;
	};
	// 2x (or more) Windowed mode: Disk II LEDs only (no track/sector info)
	struct D2CompactUI	// Disk II compact UI
	{
		static const UINT yOffsetSlot5Label = 35;
		static const UINT yOffsetSlot5LEDNumbers = yOffsetSlot5Label + smallfontHeight + 1;
		static const UINT yOffsetSlot5LEDs = yOffsetSlot5LEDNumbers + 1;
	};
	const UINT yOffsetSlot6TrackInfo = D2FullUI::yOffsetSlot6TrackInfo;
	const UINT yOffsetSlot6SectorInfo = D2FullUI::yOffsetSlot6SectorInfo;
	UINT yOffsetSlot5Label;
	UINT yOffsetSlot5LEDNumbers;
	UINT yOffsetSlot5LEDs;
	const UINT yOffsetSlot5TrackInfo = D2FullUI::yOffsetSlot5TrackInfo;
	const UINT yOffsetSlot5SectorInfo = D2FullUI::yOffsetSlot5SectorInfo;

	int g_nSector[NUM_SLOTS][2];
	Disk_Status_e g_eStatusDrive1;
	Disk_Status_e g_eStatusDrive2;

	static const int kDEFAULT_VIEWPORT_SCALE = 2;
	int g_nViewportCX;
	int g_nViewportCY;
	int g_nViewportScale; // saved REGSAVE
	int g_nMaxViewportScale;	// Max scale in Windowed mode with borders, buttons etc (full-screen may be +1)
};
