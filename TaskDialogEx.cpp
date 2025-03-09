// MIT License
//
// Copyright (c) 2025 xmc0211
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// * 可以通过#define NOUSING_NEW_COMMCTRL_PRAGMA使头文件
// 不通过#pragma使用Commctrl 6.0.0.0（出现“无法定位序
// 数345”的情况且该标识已被声明时可以尝试删除声明）
// * 可以通过#define NO_HYPERLINK_AUTORUN禁止头文件在点
// 击超链接时不自动执行命令。
// * 请不要在TaskDialog在新线程里运行时在主线程里销毁
// TaskDialogEx（包含手动释放，退出实例所在作用域如
// 函数，if，for等）

#include "TaskDialogEx.h"

__CBFUNCARG::__CBFUNCARG() : lpClass(NULL), vUserArg(NULL) {
	
}

TASKDIALOGCBFUNC::TASKDIALOGCBFUNC() : type(-1), nop(NULL), op(NULL), tp(NULL) {

}
TASKDIALOGCBFUNC::TASKDIALOGCBFUNC(TASKDIALOG_FNOPARAM func) : op(NULL), tp(NULL) {
	nop = func;
	type = 0;
}
TASKDIALOGCBFUNC::TASKDIALOGCBFUNC(TASKDIALOG_FONEPARAM func) : nop(NULL), tp(NULL) {
	op = func;
	type = 1;
}
TASKDIALOGCBFUNC::TASKDIALOGCBFUNC(TASKDIALOG_FTWOPARAM func) : nop(NULL), op(NULL) {
	tp = func;
	type = 2;
}
TASKDIALOGCBFUNC::~TASKDIALOGCBFUNC() {

}

// 调用已有的函数
void TASKDIALOGCBFUNC::Execute(HWND hWnd, LONG_PTR uiP1 /* = 0 */, LONG_PTR uiP2 /* = 0 */, LONG_PTR vArg /* = NULL */) {
	if (type == -1) return;
	LPVOID arg = reinterpret_cast <LPVOID> (vArg);
	//LPVOID arg = NULL;
	if (type == 0) nop(hWnd, arg);
	if (type == 1) op(hWnd, uiP1, arg);
	if (type == 2) tp(hWnd, uiP1, uiP2, arg);
}

TASKDIALOGTMP::TASKDIALOGTMP() :
	tdb{ NULL }, tdrb{ NULL }, ButtonName{ L"" }, RButtonName{ L"" }, Link{} {
	WindowTitle = MainInstruction = Content = VeriText = Footer = L"";
	ExpText = ExpOpenText = ExpCloseText = L"";
	tdbc = tdrbc = iButtonID = iRadioIndex = bVeriIsOK = 0;
	bOnTop = lThResult = 0;
	iX = iY = -1;
	hMainIcon = hFooterIcon = NULL;
	hWnd = NULL;
	hThread = NULL;
	fCreated = NULL;
	fDestroyed = NULL;
	fBClicked = NULL;
	lpThreadThis = NULL;
}
TASKDIALOGTMP::TASKDIALOGTMP(const TASKDIALOGTMP& other) :
	tdb{ NULL }, tdrb{ NULL } {
	Link = other.Link;
	hWnd = NULL;
	hThread = NULL;
	lpThreadThis = NULL;
	bOnTop = lThResult = 0;
	iX = iY = -1;
	iButtonID = iRadioIndex = bVeriIsOK = 0;
	tdbc = other.tdbc;
	tdrbc = other.tdrbc;
	WindowTitle = other.WindowTitle;
	MainInstruction = other.MainInstruction;
	Content = other.Content;
	VeriText = other.VeriText;
	Footer = other.Footer;
	ExpText = other.ExpText;
	ExpOpenText = other.ExpOpenText;
	ExpCloseText = other.ExpCloseText;
	if (other.hMainIcon != NULL) hMainIcon = CopyIcon(other.hMainIcon);
	else hMainIcon = NULL;
	if (other.hFooterIcon != NULL) hFooterIcon = CopyIcon(other.hFooterIcon);
	else hFooterIcon = NULL;
	fCreated = other.fCreated;
	fDestroyed = other.fDestroyed;
	fBClicked = other.fBClicked;
	memcpy(ButtonName, other.ButtonName, sizeof ButtonName);
	memcpy(RButtonName, other.RButtonName, sizeof RButtonName);
}
TASKDIALOGTMP::~TASKDIALOGTMP() {
	if (hMainIcon != NULL) DestroyIcon(hMainIcon);
	if (hFooterIcon != NULL) DestroyIcon(hFooterIcon);
	if (hThread != NULL) CloseHandle(hThread);
}


void TASKDIALOGEX::Update() {
	tdc.pszWindowTitle = tmp.WindowTitle.c_str();
	tdc.pszContent = tmp.Content.c_str();
	tdc.pszMainInstruction = tmp.MainInstruction.c_str();
	tdc.pszFooter = tmp.Footer.c_str();
	tdc.pszVerificationText = tmp.VeriText.c_str();
	tdc.pszExpandedControlText = tmp.ExpCloseText.c_str();
	tdc.pszExpandedInformation = tmp.ExpText.c_str();
	tdc.pszCollapsedControlText = tmp.ExpOpenText.c_str();
	tdc.pButtons = tmp.tdb;
	tdc.cButtons = tmp.tdbc;
	tdc.pRadioButtons = tmp.tdrb;
	tdc.cRadioButtons = tmp.tdrbc;
	if (__HAVE(tdc.dwFlags, TDF_USE_HICON_MAIN)) tdc.hMainIcon = tmp.hMainIcon;
	if (__HAVE(tdc.dwFlags, TDF_USE_HICON_FOOTER)) tdc.hFooterIcon = tmp.hFooterIcon;
	for (INT iT = 0; iT < tmp.tdbc; iT++) tmp.tdb[iT].pszButtonText = tmp.ButtonName[iT].c_str();
	for (INT iT = 0; iT < tmp.tdrbc; iT++) tmp.tdrb[iT].pszButtonText = tmp.RButtonName[iT].c_str();
}
HRESULT CALLBACK TASKDIALOGEX::__cbFunc(HWND hWnd, UINT uiNoti, WPARAM wParam, LPARAM lParam, LONG_PTR lpArg) {
	__CBFUNCARG* _cbf = reinterpret_cast <__CBFUNCARG*> (lpArg);
	TASKDIALOGEX* _this = reinterpret_cast <TASKDIALOGEX*> (_cbf->lpClass);
	// 预置的处理
	switch (uiNoti) {
		case TDN_CREATED: {
			_this->tmp.hWnd = hWnd;
			HWND top = (_this->tmp.bOnTop ? HWND_TOPMOST : HWND_NOTOPMOST);
			UINT flag = SWP_SHOWWINDOW | SWP_NOSIZE;
			INT x = _this->tmp.iX, y = _this->tmp.iY;
			if (x == -1 || y == -1) {
				flag |= SWP_NOMOVE;
				x = y = 0;
			}
			SetWindowPos(hWnd, top, x, y, 0, 0, flag);
			break;
		}
		case TDN_HYPERLINK_CLICKED: {
#ifndef NO_HYPERLINK_AUTORUN
			WCHAR* ppszHyperlink = reinterpret_cast <WCHAR*> (lParam);
			std::wstring lpwHyperlink(ppszHyperlink);
			std::_tstring lpcHyperlink = LPW2LPT(lpwHyperlink);
			size_t fnd = _this->tmp.Link.count(lpcHyperlink);
			if (!fnd) break;
			ShellExecute(NULL, TEXT("open"), _this->tmp.Link[lpcHyperlink].c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
			break;
		}
		default: {
			break;
		}
	}
	_this->GetCurrentCBFunc(uiNoti).Execute(hWnd, wParam, lParam, _cbf->vUserArg);
	return 0;
}
DWORD WINAPI TASKDIALOGEX::__thFunc(LPVOID lpParam) {
	TASKDIALOGEX* _this = reinterpret_cast <TASKDIALOGEX*> (lpParam);
	_this->tmp.lThResult = _this->Show();
	return 0;
}
TASKDIALOGEX::TASKDIALOGEX() : tdc{ 0 }, cbf{} {
	tdc.cbSize = sizeof(TASKDIALOGCONFIG);
	tdc.hwndParent = GetActiveWindow();
	tdc.hInstance = GetModuleHandleA(NULL);
	tdc.pszWindowTitle = L"";
	tdc.pszContent = L"";
	tdc.pszMainInstruction = L"";
	tdc.pfCallback = __cbFunc;
	tmp.vCBFArg.lpClass = reinterpret_cast <LONG_PTR> (this);
	__CBFUNCARG* pcbfarg = &tmp.vCBFArg;
	tdc.lpCallbackData = reinterpret_cast <LONG_PTR> (pcbfarg);
	tmp.lpThreadThis = reinterpret_cast <LPVOID> (this);
}
TASKDIALOGEX::TASKDIALOGEX(const TASKDIALOGEX& other) {
	tdc = other.tdc;
	tmp = other.tmp;
}
TASKDIALOGEX::~TASKDIALOGEX() {
	// 避免线程在TASKDIALOGEX销毁时继续运行。
	// 但还是建议提前关闭窗口，因为自动关闭会
	// 影响视觉效果（卡顿）。
	if (tmp.hThread != NULL &&
	        WaitForSingleObject(tmp.hThread, __TD_TERMINATE_TIMEOUT) == WAIT_TIMEOUT) {
		SendWndMessage(TDM_CLICK_BUTTON, IDOK);
		if (__TD_TERMINATE_TIMEOUT > 0) Sleep(__TD_TERMINATE_TIMEOUT);
	}
}
TASKDIALOGCONFIG TASKDIALOGEX::GetCurrentConfig() {
	return tdc;
}
TASKDIALOGCBFUNC TASKDIALOGEX::GetCurrentCBFunc(INT iIndx) {
	return cbf[iIndx];
}

// ==============================运行前函数==============================
// 设置属性（设置或移除多个可用“|”连接）
void TASKDIALOGEX::SetAttrib(INT iFlag, TASKDIALOGFLAGOPTION iOpt /* = TDF_ENABLE */ ) {
	if (iOpt) tdc.dwFlags = __ADD(tdc.dwFlags, iFlag);
	else tdc.dwFlags = __REMOVE(tdc.dwFlags, iFlag);
}
// 设置窗口宽度
void TASKDIALOGEX::SetWidth(UINT uiWidth) {
	tdc.cxWidth = uiWidth;
}
// 设置窗口标题
void TASKDIALOGEX::SetWindowTitle(std::_tstring lpcWindowTitle) {
	tmp.WindowTitle = LPT2LPW(lpcWindowTitle);
	return;
}
// 设置文本正文
void TASKDIALOGEX::SetContent(std::_tstring lpcContent) {
	tmp.Content = LPT2LPW(lpcContent);
	return;
}
// 设置文本标题
void TASKDIALOGEX::SetMainInstruction(std::_tstring lpcMainIntruction) {
	tmp.MainInstruction = LPT2LPW(lpcMainIntruction);
	return;
}
// 设置主图标（用预设）
void TASKDIALOGEX::SetMainIcon(TASKDIALOGMAINICON iIconID) {
	tdc.dwFlags = __REMOVE(tdc.dwFlags, TDF_USE_HICON_MAIN);
	if (tmp.hMainIcon != NULL) DestroyIcon(tmp.hMainIcon);
	tdc.pszMainIcon = MAKEINTRESOURCEW(iIconID);
	return;
}
// 设置主图标（自定义，HICON）
void TASKDIALOGEX::SetMainIcon(HICON hIcon) {
	tdc.dwFlags |= TDF_USE_HICON_MAIN;
	if (tmp.hMainIcon != NULL) DestroyIcon(tmp.hMainIcon);
	tmp.hMainIcon = CopyIcon(hIcon);
}
// 设置主图标（自定义，指定dll名称和图标索引）
void TASKDIALOGEX::SetMainIcon(std::_tstring lpcDllName, INT iIconIndx) {
	HMODULE mod = LoadLibrary(lpcDllName.c_str());
	if (mod == NULL) return;
	SetMainIcon(LoadIcon(mod, MAKEINTRESOURCE(iIconIndx)));
	FreeLibrary(mod);
}
// 设置底部文字正文
void TASKDIALOGEX::SetFooter(std::_tstring lpcFooter) {
	tmp.Footer = LPT2LPW(lpcFooter);
	return;
}
// 设置底部文字图标（用预设）
void TASKDIALOGEX::SetFooterIcon(TASKDIALOGFOOTERICON iIconID) {
	tdc.dwFlags = __REMOVE(tdc.dwFlags, TDF_USE_HICON_FOOTER);
	if (tmp.hFooterIcon != NULL) DestroyIcon(tmp.hFooterIcon);
	tdc.pszFooterIcon = MAKEINTRESOURCEW(iIconID);
	return;
}
// 设置底部文字图标（自定义，HICON）
void TASKDIALOGEX::SetFooterIcon(HICON hIcon) {
	tdc.dwFlags |= TDF_USE_HICON_FOOTER;
	if (tmp.hFooterIcon != NULL) DestroyIcon(tmp.hFooterIcon);
	tmp.hFooterIcon = CopyIcon(hIcon);
}
// 设置底部文字图标（自定义，指定dll名称和图标索引）
void TASKDIALOGEX::SetFooterIcon(std::_tstring lpcDllName, INT iIconIndx) {
	HMODULE mod = LoadLibrary(lpcDllName.c_str());
	if (mod == NULL) return;
	SetFooterIcon(LoadIcon(mod, MAKEINTRESOURCE(iIconIndx)));
	FreeLibrary(mod);
}
// 添加按钮（用预设）
void TASKDIALOGEX::AddButton(TASKDIALOG_COMMON_BUTTON_FLAGS tcbfSet) {
	tdc.dwCommonButtons |= tcbfSet;
}
// 添加按钮（自定义）
void TASKDIALOGEX::AddButton(INT iButtonID, std::_tstring lpcName, std::_tstring lpcCont /* = "" */) {
	if (tmp.tdbc >= __TD_MAX_BUTTON_NUM) return;
	if (lpcCont != TEXT("")) lpcName = (lpcName + (TCHAR)10 + lpcCont);
	tmp.ButtonName[tmp.tdbc] = LPT2LPW(lpcName);
	tmp.tdb[tmp.tdbc].nButtonID = iButtonID;
	tmp.tdbc++;
}
// 设置默认按钮
void TASKDIALOGEX::SetDefaultButton(INT iButtonIndx) {
	tdc.nDefaultRadioButton = iButtonIndx;
}
// 添加单选按钮
void TASKDIALOGEX::AddRButton(INT iButtonID, std::_tstring lpcName) {
	if (tmp.tdrbc >= __TD_MAX_RBUTTON_NUM) return;
	tmp.RButtonName[tmp.tdrbc] = LPT2LPW(lpcName);
	tmp.tdrb[tmp.tdrbc].nButtonID = iButtonID;
	tmp.tdrbc++;
}
// 设置默认单选按钮
void TASKDIALOGEX::SetDefaultRButton(INT iButtonIndx) {
	tdc.nDefaultRadioButton = iButtonIndx;
}
// 设置复选框正文
void TASKDIALOGEX::SetVeriText(std::_tstring lpcVeriText) {
	tmp.VeriText = LPT2LPW(lpcVeriText);
	return;
}
// 设置更多信息正文、打开提示语、收起提示语
void TASKDIALOGEX::SetExpanded(std::_tstring lpcText, std::_tstring lpcOpenText, std::_tstring lpcCloseText) {
	tmp.ExpText = LPT2LPW(lpcText);
	tmp.ExpOpenText = LPT2LPW(lpcOpenText);
	tmp.ExpCloseText = LPT2LPW(lpcCloseText);
	return;
}
// 设置消息对应的回调函数
void TASKDIALOGEX::SetCallbackFunc(UINT uiNoti, TASKDIALOGCBFUNC fCBFunc) {
	if (uiNoti < 0 || uiNoti > 10) return;
	if (fCBFunc.type != __TASKDIALOG_CBFUNC_CORRECTARGS[uiNoti]) return;
	cbf[uiNoti] = fCBFunc;
}
// 设置传递给回调函数的参数指针
void TASKDIALOGEX::SetCallbackArgs(LPVOID vArg) {
	if (vArg == NULL) return;
	tmp.vCBFArg.vUserArg = reinterpret_cast <LONG_PTR> (vArg);
}
// 设置窗口初始坐标
void TASKDIALOGEX::SetPoint(INT iX, INT iY) {
	tmp.iX = iX;
	tmp.iY = iY;
}
// 设置窗口是否置顶
void TASKDIALOGEX::SetIsOnTop(BOOL bIsOnTop) {
	tmp.bOnTop = bIsOnTop;
}
// 创建（若已有则直接返回）超链接（cmd命令前面要加cmd /c或cmd /k）
std::_tstring TASKDIALOGEX::CreateHLink(std::_tstring lpId, std::_tstring lpText, std::_tstring lpCmd /* = "" */ ) {
	std::_tstring ret = TEXT("<a href=\"") + lpId + TEXT("\">") + lpText + TEXT("</a>");
	if (tmp.Link.count(lpId)) {
		if (lpCmd != TEXT("")) tmp.Link[lpId] = lpCmd;
	} else {
		if (lpCmd == TEXT("")) return TEXT("");
		tmp.Link.insert({ lpId, lpCmd });
	}
	SetAttrib(TDF_ENABLE_HYPERLINKS, TDF_ENABLE);
	return ret;
}

// ==============================运行时函数==============================
// 显示对话框
LONG TASKDIALOGEX::Show() {
	Update();
	LONG res = TaskDialogIndirect(&tdc, &tmp.iButtonID, &tmp.iRadioIndex, &tmp.bVeriIsOK);
	return res;
}
// 显示对话框（在新线程里）
void TASKDIALOGEX::ShowInNewThread() {
	//避免线程多开
	if (tmp.hThread != NULL) return;
	tmp.lThResult = 0;
#ifndef DISABLE_NEW_THREAD
	tmp.hThread = CreateThread(NULL, 0, __thFunc, tmp.lpThreadThis, 0, NULL);
#else
	__thFunc(tmp.lpThreadThis);
#endif
}
// 等待打开的线程结束并获取返回值
LONG TASKDIALOGEX::WaitThreadEnd() const {
#ifndef DISABLE_NEW_THREAD
	WaitForSingleObject(tmp.hThread, INFINITE);
	return tmp.lThResult;
#else
	return 0;
#endif
}
// 获取窗口句柄（显示之前都是NULL）
HWND TASKDIALOGEX::GetHWND() const {
	return tmp.hWnd;
}
// 给显示的窗口发送消息（仅限新线程中的窗口）
void TASKDIALOGEX::SendWndMessage(UINT uiMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0*/ ) const {
	if (tmp.hWnd == NULL) return;
	SendMessage(tmp.hWnd, uiMsg, wParam, lParam);
}
// 判断窗口是否还在运行（仅限新线程中的窗口）
BOOL TASKDIALOGEX::IsActive() const {
	DWORD ret;
	GetExitCodeThread(tmp.hThread, &ret);
	return ret == STILL_ACTIVE;
}
// 将窗口置于最顶层
void TASKDIALOGEX::TopMost() const {
	SetWindowPos(GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

// ==============================运行后函数==============================
// 获得被按下的按钮的ID
INT TASKDIALOGEX::GetButtonID() const {
	return tmp.iButtonID;
}
// 获得被选中的单选按钮的ID
INT TASKDIALOGEX::GetRButtonID() const {
	return tmp.iRadioIndex;
}
// 获得复选框选中消息（未选中0，选中1）
BOOL TASKDIALOGEX::GetVeriState() const {
	return tmp.bVeriIsOK;
}
