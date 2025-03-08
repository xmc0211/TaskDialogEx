// TaskDialogEx.h by XMC
// 依赖Convert.h by XMC

// * 可以通过#define NOUSING_NEW_COMMCTRL_PRAGMA使头文件
// 不通过#pragma使用Commctrl 6.0.0.0（出现“无法定位序
// 数345”的情况且该标识已被声明时可以尝试删除声明）
// * 可以通过#define NO_HYPERLINK_AUTORUN禁止头文件在点
// 击超链接时不自动执行命令。
// * 请不要在TaskDialog在新线程里运行时在主线程里销毁
// TaskDialogEx（包含手动释放，退出实例所在作用域如
// 函数，if，for等）

#ifndef TASKDIALOGEX_H
#define TASKDIALOGEX_H

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <string>
#include <map>
#include "Convert.h"
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")

#ifndef NOUSING_NEW_COMMCTRL_PRAGMA
	#pragma comment(linker,"\"/manifestdependency:type='win32' \
							name = 'Microsoft.Windows.Common-Controls' \
							version = '6.0.0.0' \
							processorArchitecture = '*' \
							publicKeyToken = '6595b64144ccf1df' \
							language = '*'\"")
#endif /** #ifndef NOUSING_NEW_COMMCTRL_PRAGMA **/

#define __RND(l, r) (rand() % (r - l + 1) + l)

#define __REMOVE(ORIG, OPT) (ORIG & (~OPT))
#define __HAVE(ORIG, OPT) (bool)(ORIG & OPT)
#define __ADD(ORIG, OPT) (ORIG | OPT)

const INT __TD_MAX_BUTTON_NUM = 10;
const INT __TD_MAX_RBUTTON_NUM = 10;
const INT __TD_TERMINATE_TIMEOUT = 0;
const INT __TASKDIALOG_CBFUNC_CORRECTARGS[11] = {0, 0, 1, 2, 1, 0, 1, 0, 1, 0, 1};

enum TASKDIALOGMAINICON {
	TDMI_WARNING = -1,
	TDMI_ERROR = -2,
	TDMI_INFORMATION = -3,
	TDMI_SHIELD = -4, 
	TDMI_SHIELD_BLUE = -5,
	TDMI_SHIELDWARNING_YELLOW = -6,
	TDMI_SHIELDERROR_RED = -7,
	TDMI_SHIELDSUCCESS_GREEN = -8,
	TDMI_SHIELD_GREY = -9
};
enum TASKDIALOGFOOTERICON {
	TDFI_WARNING = -1,
	TDFI_ERROR = -2,
	TDFI_INFORMATION = -3,
	TDFI_SHIELD = -4,
	TDFI_SHIELDWARNING = -6,
	TDFI_SHIELDERROR = -7,
	TDFI_SHIELDSUCCESS = -8,
};
enum TASKDIALOGFLAGOPTION {
	TDF_ENABLE = 1,
	TDF_DISABLE = 0
};

typedef void (CALLBACK* TASKDIALOG_FNOPARAM)(HWND hWnd, LPVOID vArg);
typedef void (CALLBACK* TASKDIALOG_FONEPARAM)(HWND hWnd, LONG_PTR uiP1, LPVOID vArg);
typedef void (CALLBACK* TASKDIALOG_FTWOPARAM)(HWND hWnd, LONG_PTR uiP1, LONG_PTR uiP2, LPVOID vArg);

struct __CBFUNCARG {
	LONG_PTR lpClass;
	LONG_PTR vUserArg;
	// 构造
	__CBFUNCARG();
}; // 部分回调函数的参数结构
struct TASKDIALOGCBFUNC {
	int type;
	TASKDIALOG_FNOPARAM nop;
	TASKDIALOG_FONEPARAM op;
	TASKDIALOG_FTWOPARAM tp;
	// 构造
	TASKDIALOGCBFUNC();
	TASKDIALOGCBFUNC(TASKDIALOG_FNOPARAM func);
	TASKDIALOGCBFUNC(TASKDIALOG_FONEPARAM func);
	TASKDIALOGCBFUNC(TASKDIALOG_FTWOPARAM func);
	// 析构
	~TASKDIALOGCBFUNC();

	// 调用已有的函数
	void Execute(HWND hWnd, LONG_PTR uiP1 = 0, LONG_PTR uiP2 = 0, LONG_PTR vArg = NULL);
}; // 通用的回调函数结构
struct TASKDIALOGTMP {
	HWND hWnd;
	HANDLE hThread;
	LPVOID lpThreadThis;
	INT tdbc, tdrbc;
	INT iButtonID, iRadioIndex;
	INT iX, iY;
	LONG lThResult;
	BOOL bVeriIsOK;
	BOOL bOnTop;
	std::wstring WindowTitle, Content, MainInstruction, VeriText, Footer;
	std::wstring ExpText, ExpOpenText, ExpCloseText;
	std::wstring ButtonName[__TD_MAX_BUTTON_NUM + 1], RButtonName[__TD_MAX_RBUTTON_NUM + 1];
	TASKDIALOG_BUTTON tdb[__TD_MAX_BUTTON_NUM + 1], tdrb[__TD_MAX_RBUTTON_NUM + 1];
	HICON hMainIcon, hFooterIcon;
	TASKDIALOG_FNOPARAM fCreated;
	TASKDIALOG_FNOPARAM fDestroyed;
	TASKDIALOG_FONEPARAM fBClicked;
	__CBFUNCARG vCBFArg;
	std::map <std::string, std::string> Link;
	// 构造
	TASKDIALOGTMP();
	// 拷贝
	TASKDIALOGTMP(const TASKDIALOGTMP& other);
	// 析构
	~TASKDIALOGTMP();
}; // 用于存放TASKDIALOGEX的临时数据

// TASKDIALOGCONFIG的封装
class TASKDIALOGEX {
protected:
	TASKDIALOGCONFIG tdc;
	TASKDIALOGCBFUNC cbf[11];
public:
	TASKDIALOGTMP tmp;
protected:
	// 刷新信息
	void Update();
	// 预置回调函数
	static HRESULT CALLBACK __cbFunc(HWND hWnd, UINT uiNoti, WPARAM wParam, LPARAM lParam, LONG_PTR lpArg);
	// 预置线程运行函数，使用LPVOID作为类的实例指针
	static DWORD WINAPI __thFunc(LPVOID lpParam);
public:
	// 构造
	TASKDIALOGEX();
	// 拷贝
	TASKDIALOGEX(const TASKDIALOGEX& other);
	// 析构
	~TASKDIALOGEX();

	TASKDIALOGCONFIG GetCurrentConfig();
	TASKDIALOGCBFUNC GetCurrentCBFunc(INT iIndx);

// ==============================运行前函数==============================
	// 设置属性（设置或移除多个可用“|”连接）
	void SetAttrib(INT iFlag, TASKDIALOGFLAGOPTION iOpt = TDF_ENABLE);
	// 设置窗口宽度
	void SetWidth(UINT uiWidth);
	// 设置窗口标题
	void SetWindowTitle(std::string lpcWindowTitle);
	// 设置文本正文
	void SetContent(std::string lpcContent);
	// 设置文本标题
	void SetMainInstruction(std::string lpcMainIntruction);
	// 设置主图标（用预设）
	void SetMainIcon(TASKDIALOGMAINICON iIconID);
	// 设置主图标（自定义，HICON）
	void SetMainIcon(HICON hIcon);
	// 设置主图标（自定义，指定dll名称和图标索引）
	void SetMainIcon(std::string lpcDllName, INT iIconIndx);
	// 设置底部文字正文
	void SetFooter(std::string lpcFooter);
	// 设置底部文字图标（用预设）
	void SetFooterIcon(TASKDIALOGFOOTERICON iIconID);
	// 设置底部文字图标（自定义，HICON）
	void SetFooterIcon(HICON hIcon);
	// 设置底部文字图标（自定义，指定dll名称和图标索引）
	void SetFooterIcon(std::string lpcDllName, INT iIconIndx);
	// 添加按钮（用预设）
	void AddButton(TASKDIALOG_COMMON_BUTTON_FLAGS tcbfSet);
	// 添加按钮（自定义）
	void AddButton(INT iButtonID, std::string lpcName, std::string lpcCont = "");
	// 设置默认按钮
	void SetDefaultButton(INT iButtonIndx);
	// 添加单选按钮
	void AddRButton(INT iButtonID, std::string lpcName);
	// 设置默认单选按钮
	void SetDefaultRButton(INT iButtonIndx);
	// 设置复选框正文
	void SetVeriText(std::string lpcVeriText);
	// 设置更多信息正文、打开提示语、收起提示语
	void SetExpanded(std::string lpcText, std::string lpcOpenText, std::string lpcCloseText);
	// 设置消息对应的回调函数
	void SetCallbackFunc(UINT uiNoti, TASKDIALOGCBFUNC fCBFunc);
	// 设置传递给回调函数的参数指针
	void SetCallbackArgs(LPVOID vArg);
	// 设置窗口初始坐标
	void SetPoint(INT iX, INT iY);
	// 设置窗口是否置顶
	void SetIsOnTop(BOOL bIsOnTop);
	// 创建（若已有则直接返回）超链接（cmd命令前面要加cmd /c或cmd /k）
	std::string CreateHLink(std::string lpId, std::string lpText, std::string lpCmd = "");

// ==============================运行时函数==============================
	// 显示对话框
	LONG Show();
	// 显示对话框（在新线程里）
	void ShowInNewThread();
	// 等待打开的线程结束并获取返回值
	LONG WaitThreadEnd() const;
	// 获取窗口句柄（显示之前都是NULL）
	HWND GetHWND() const;
	// 给显示的窗口发送消息（仅限新线程中的窗口）
	void SendWndMessage(UINT uiMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	// 判断窗口是否还在运行（仅限新线程中的窗口）
	BOOL IsActive() const;
	// 将窗口置于最顶层
	void TopMost() const;

// ==============================运行后函数==============================
	// 获得被按下的按钮的ID
	INT GetButtonID() const;
	// 获得被选中的单选按钮的ID
	INT GetRButtonID() const;
	// 获得复选框选中消息（未选中0，选中1）
	BOOL GetVeriState() const;
};

#endif /** #ifndef TASKDIALOGEX_H **/

/* 注释（可以从这里复制必要的信息）
* 
* 1.有关TASKDIALOGCONFIG成员的说明
* 
* cbSize (UINT):									结构的大小，以字节为单位。这通常用于版本控制，以确保传递给 API 的结构大小是正确的。
* hwndParent (HWND):								拥有者窗口的句柄，而不是父窗口。当对话框被显示时，它将相对于这个窗口定位（尽管这取决于 dwFlags 中的 TDF_POSITION_RELATIVE_TO_WINDOW 标志）。
* hInstance (HINSTANCE):							包含对话框中使用的字符串资源的模块实例句柄。这些字符串可以通过 MAKEINTRESOURCE() 宏来引用。
* dwFlags (TASKDIALOG_FLAGS):						一个标志位集合，用于指定对话框的行为和外观。这些标志位包括是否启用超链接、是否使用大图标或小图标、是否允许取消对话框等。
* dwCommonButtons (TASKDIALOG_COMMON_BUTTON_FLAGS):	一个标志位集合，用于指定对话框中要显示的常见按钮（如“确定”、“取消”、“是”、“否”等）。
* pszWindowTitle (PCWSTR):							指向对话框窗口标题的字符串指针，或者是通过 MAKEINTRESOURCE() 宏引用的资源 ID。
* DUMMYUNIONNAME (联合):							一个联合，用于指定主要区域的图标。可以是图标的句柄 (hMainIcon) 或指向图标文件名的字符串指针 (pszMainIcon)。
* pszMainInstruction (PCWSTR):						指向主要指令文本的字符串指针，通常是对话框中最大的文本区域。
* pszContent (PCWSTR):								指向内容文本的字符串指针，通常显示在主要指令文本下方。
* cButtons (UINT):									自定义按钮的数量。
* pButtons (const TASKDIALOG_BUTTON *):				指向一个 TASKDIALOG_BUTTON 结构数组的指针，用于定义自定义按钮。
* nDefaultButton (int):								默认按钮的索引（在 pButtons 数组中），当用户按 Enter 键时将被激活。
* cRadioButtons (UINT):								单选按钮的数量。
* pRadioButtons (const TASKDIALOG_BUTTON *):		指向一个 TASKDIALOG_BUTTON 结构数组的指针，用于定义单选按钮。
* nDefaultRadioButton (int):						默认单选按钮的索引（在 pRadioButtons 数组中），当初始显示对话框时将被选中。
* pszVerificationText (PCWSTR):						指向验证文本的字符串指针，通常与复选框一起使用。
* pszExpandedInformation (PCWSTR):					指向扩展信息的字符串指针，当用户展开页脚区域时显示。
* pszExpandedControlText (PCWSTR):					指向扩展控件文本的字符串指针，通常用于描述扩展区域中的控件（如复选框或文本框）。
* pszCollapsedControlText (PCWSTR):					指向折叠控件文本的字符串指针，当页脚区域被折叠时显示。
* DUMMYUNIONNAME2 (联合):							一个联合，用于指定页脚区域的图标。可以是图标的句柄 (hFooterIcon) 或指向图标文件名的字符串指针 (pszFooterIcon)。
* pszFooter (PCWSTR):								指向页脚文本的字符串指针，通常显示在对话框的底部。
* pfCallback (PFTASKDIALOGCALLBACK):				指向一个回调函数的指针，当对话框中的某些事件发生时（如按钮被点击、定时器触发等）将调用该函数。
* lpCallbackData (LONG_PTR):						传递给回调函数的用户定义数据。
* cxWidth (UINT):									对话框客户端区域的宽度，以对话框逻辑单位（DLU）为单位。如果为 0，则对话框将计算理想的宽度。
* 
* 2.有关TDF_*(_TASKDIALOG_FLAGS)及对应功能的说明（可用于AddAttrib()第一个参数）
* 
* TDF_ENABLE_HYPERLINKS (0x0001):			允许在 TaskDialog 中使用超链接。
* TDF_USE_HICON_MAIN (0x0002):				使用在主要内容区域显示的大图标（hMainIcon 成员指定的图标）。
* TDF_USE_HICON_FOOTER (0x0004):			使用在页脚区域显示的小图标（hFooterIcon 成员指定的图标）。
* TDF_ALLOW_DIALOG_CANCELLATION (0x0008):	允许用户通过关闭按钮（如右上角的“X”按钮）或按 Esc 键来取消对话框。
* TDF_USE_COMMAND_LINKS (0x0010):			使用命令链接（带有图标的按钮）代替普通按钮。
* TDF_USE_COMMAND_LINKS_NO_ICON (0x0020):	使用没有图标的命令链接按钮。
* TDF_EXPAND_FOOTER_AREA (0x0040):			扩展页脚区域的高度，以便显示更多的信息或控件。
* TDF_EXPANDED_BY_DEFAULT (0x0080):			默认情况下扩展页脚区域（如果设置了 TDF_EXPAND_FOOTER_AREA）。
* TDF_VERIFICATION_FLAG_CHECKED (0x0100):	初始化时选中验证标志（复选框）。
* TDF_SHOW_PROGRESS_BAR (0x0200):			显示进度条。
* TDF_SHOW_MARQUEE_PROGRESS_BAR (0x0400):	显示不确定的（循环的）进度条（马拉松式进度条）。
* TDF_CALLBACK_TIMER (0x0800):				启用定时器回调，以便在对话框显示期间定期执行某些操作。
* TDF_POSITION_RELATIVE_TO_WINDOW (0x1000):	将对话框相对于指定窗口定位（使用 hwndParent 成员）。
* TDF_RTL_LAYOUT (0x2000):					使用从右到左的布局（适用于阿拉伯语、希伯来语等语言）。
* TDF_NO_DEFAULT_RADIO_BUTTON (0x4000):		不设置默认的单选按钮。
* TDF_CAN_BE_MINIMIZED (0x8000):			允许对话框被最小化。
* TDF_NO_SET_FOREGROUND (0x00010000):		当激活对话框时，不调用 SetForegroundWindow()（仅在 Windows 8 及更高版本上有效）。
* TDF_SIZE_TO_CONTENT (0x01000000):			根据内容调整对话框的大小（这通常用于模仿 MessageBox 的大小调整行为）。
* 
* 3.有关TDM_*(_TASKDIALOG_MESSAGES)及对应功能的说明（第二列是参数个数）（可用于SendWndMessage()第一个参数）
* 
* TDM_NAVIGATE_PAGE:						0	用于导航到指定页面。
* TDM_CLICK_BUTTON:							1	模拟点击按钮，wParam为按钮ID。
* TDM_SET_MARQUEE_PROGRESS_BAR:				1	设置进度条是否为活动状态，wParam为0表示非活动，非0表示活动。
* TDM_SET_PROGRESS_BAR_STATE:				1	更新进度条状态，wParam为新的状态。
* TDM_SET_PROGRESS_BAR_RANGE:				1	设置进度条范围，lParam为MAKELPARAM(nMinRange, nMaxRange)。
* TDM_SET_PROGRESS_BAR_POS:					1	更新进度条位置，wParam为新的位置。
* TDM_SET_PROGRESS_BAR_MARQUEE:				2	控制进度条动画，wParam为0停止，非0开始，lParam为动画速度。
* TDM_SET_ELEMENT_TEXT:						2	设置元素文本，wParam为元素(TDE_*)，lParam为新文本。
* TDM_CLICK_RADIO_BUTTON:					1	模拟点击单选按钮，wParam为单选按钮ID。
* TDM_ENABLE_BUTTON:						2	启用/禁用按钮，lParam为0禁用，非0启用，wParam为按钮ID。
* TDM_ENABLE_RADIO_BUTTON:					2	启用/禁用单选按钮，lParam为0禁用，非0启用，wParam为按钮ID。
* TDM_CLICK_VERIFICATION:					1	模拟点击验证复选框，wParam为0未选中，1选中。
* TDM_UPDATE_ELEMENT_TEXT:					2	更新元素文本，wParam为元素(TDE_*)，lParam为新文本。
* TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE:	2	设置按钮是否需要提升权限，wParam为按钮ID，lParam为0不需要，非0需要。
* TDM_UPDATE_ICON:							2	更新图标，wParam为图标元素(TDIE_ICON_*)，lParam为新图标句柄或路径。
* 
* 4.有关TDN_*(_TASKDIALOG_NOTIFICATIONS)及对应功能的说明（第二列是参数个数）（可用于SetCallbackFunc()第一个参数）
* 
* TDN_CREATED:					0	当任务对话框被创建时发送。
* TDN_NAVIGATED:				0	当用户导航到对话框中的不同页面或标签时发送。
* TDN_BUTTON_CLICKED:			1	用户点击对话框中的按钮时发送，wParam为按钮的 ID。
* TDN_HYPERLINK_CLICKED:		2	当用户点击对话框中的超链接时发送，wParam始终为0，lParam为指向被点击的超链接的 URL (LPCWSTR 类型)。
* TDN_TIMER:					1	定时器事件，用于通知对话框已经存在的时间，wParam为自对话框创建或定时器重置以来的毫秒数。
* TDN_DESTROYED:				0	当任务对话框被销毁时发送。
* TDN_RADIO_BUTTON_CLICKED:		1	当用户选择对话框中的单选按钮时发送，wParam为单选按钮的 ID。
* TDN_DIALOG_CONSTRUCTED:		0	当对话框完全构建并准备显示时发送。
* TDN_VERIFICATION_CLICKED:		1	当用户点击对话框中的验证复选框时发送，wParam为复选框的状态（1表示选中，0表示未选中）。
* TDN_HELP:						0	当用户请求帮助时发送（例如，点击帮助按钮或按 F1 键）。
* TDN_EXPANDO_BUTTON_CLICKED:	1	当用户点击对话框中的扩展/折叠按钮时发送，wParam为0表示对话框现在是折叠状态，非0表示对话框现在是展开状态。
* 
* 5.预置的按钮ID(ID*)
* 
* IDOK			1   * IDNO			7
* IDCANCEL		2   * IDCLOSE		8
* IDABORT		3   * IDHELP		9
* IDRETRY		4   * IDTRYAGAIN	10
* IDIGNORE		5   * IDCONTINUE	11
* IDYES			6   * IDTIMEOUT		12
* 
* 6.预置的按钮选项(TDCBF_*)
* 
* TDCBF_OK_BUTTON		0x01
* TDCBF_YES_BUTTON		0x02
* TDCBF_NO_BUTTON		0x04
* TDCBF_CANCEL_BUTTON	0x08
* TDCBF_RETRY_BUTTON	0x10
* TDCBF_CLOSE_BUTTON	0x20
* 
*/
