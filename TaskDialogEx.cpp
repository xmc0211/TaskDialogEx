// TaskDialogEx.cpp by XMC

// * ����ͨ��#define NOUSING_NEW_COMMCTRL_PRAGMAʹͷ�ļ�
// ��ͨ��#pragmaʹ��Commctrl 6.0.0.0�����֡��޷���λ��
// ��345��������Ҹñ�ʶ�ѱ�����ʱ���Գ���ɾ��������
// * ����ͨ��#define NO_HYPERLINK_AUTORUN��ֹͷ�ļ��ڵ�
// ��������ʱ���Զ�ִ�����
// * �벻Ҫ��TaskDialog�����߳�������ʱ�����߳�������
// TaskDialogEx�������ֶ��ͷţ��˳�ʵ��������������
// ������if��for�ȣ�

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

// �������еĺ���
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
	// Ԥ�õĴ���
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
			std::string lpcHyperlink = LPW2LPC(lpwHyperlink);
			size_t fnd = _this->tmp.Link.count(lpcHyperlink);
			if (!fnd) break;
			ShellExecuteA(NULL, "open", _this->tmp.Link[lpcHyperlink].c_str(), NULL, NULL, SW_SHOWNORMAL);
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
	// �����߳���TASKDIALOGEX����ʱ�������С�
	// �����ǽ�����ǰ�رմ��ڣ���Ϊ�Զ��رջ�
	// Ӱ���Ӿ�Ч�������٣���
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

// ==============================����ǰ����==============================
// �������ԣ����û��Ƴ�������á�|�����ӣ�
void TASKDIALOGEX::SetAttrib(INT iFlag, TASKDIALOGFLAGOPTION iOpt /* = TDF_ENABLE */ ) {
	if (iOpt) tdc.dwFlags = __ADD(tdc.dwFlags, iFlag);
	else tdc.dwFlags = __REMOVE(tdc.dwFlags, iFlag);
}
// ���ô��ڿ��
void TASKDIALOGEX::SetWidth(UINT uiWidth) {
	tdc.cxWidth = uiWidth;
}
// ���ô��ڱ���
void TASKDIALOGEX::SetWindowTitle(std::string lpcWindowTitle) {
	tmp.WindowTitle = LPC2LPW(lpcWindowTitle);
	return;
}
// �����ı�����
void TASKDIALOGEX::SetContent(std::string lpcContent) {
	tmp.Content = LPC2LPW(lpcContent);
	return;
}
// �����ı�����
void TASKDIALOGEX::SetMainInstruction(std::string lpcMainIntruction) {
	tmp.MainInstruction = LPC2LPW(lpcMainIntruction);
	return;
}
// ������ͼ�꣨��Ԥ�裩
void TASKDIALOGEX::SetMainIcon(TASKDIALOGMAINICON iIconID) {
	tdc.dwFlags = __REMOVE(tdc.dwFlags, TDF_USE_HICON_MAIN);
	if (tmp.hMainIcon != NULL) DestroyIcon(tmp.hMainIcon);
	tdc.pszMainIcon = MAKEINTRESOURCEW(iIconID);
	return;
}
// ������ͼ�꣨�Զ��壬HICON��
void TASKDIALOGEX::SetMainIcon(HICON hIcon) {
	tdc.dwFlags |= TDF_USE_HICON_MAIN;
	if (tmp.hMainIcon != NULL) DestroyIcon(tmp.hMainIcon);
	tmp.hMainIcon = CopyIcon(hIcon);
}
// ������ͼ�꣨�Զ��壬ָ��dll���ƺ�ͼ��������
void TASKDIALOGEX::SetMainIcon(std::string lpcDllName, INT iIconIndx) {
	HMODULE mod = LoadLibraryA(lpcDllName.c_str());
	if (mod == NULL) return;
	SetMainIcon(LoadIconA(mod, MAKEINTRESOURCEA(iIconIndx)));
	FreeLibrary(mod);
}
// ���õײ���������
void TASKDIALOGEX::SetFooter(std::string lpcFooter) {
	tmp.Footer = LPC2LPW(lpcFooter);
	return;
}
// ���õײ�����ͼ�꣨��Ԥ�裩
void TASKDIALOGEX::SetFooterIcon(TASKDIALOGFOOTERICON iIconID) {
	tdc.dwFlags = __REMOVE(tdc.dwFlags, TDF_USE_HICON_FOOTER);
	if (tmp.hFooterIcon != NULL) DestroyIcon(tmp.hFooterIcon);
	tdc.pszFooterIcon = MAKEINTRESOURCEW(iIconID);
	return;
}
// ���õײ�����ͼ�꣨�Զ��壬HICON��
void TASKDIALOGEX::SetFooterIcon(HICON hIcon) {
	tdc.dwFlags |= TDF_USE_HICON_FOOTER;
	if (tmp.hFooterIcon != NULL) DestroyIcon(tmp.hFooterIcon);
	tmp.hFooterIcon = CopyIcon(hIcon);
}
// ���õײ�����ͼ�꣨�Զ��壬ָ��dll���ƺ�ͼ��������
void TASKDIALOGEX::SetFooterIcon(std::string lpcDllName, INT iIconIndx) {
	HMODULE mod = LoadLibraryA(lpcDllName.c_str());
	if (mod == NULL) return;
	SetFooterIcon(LoadIconA(mod, MAKEINTRESOURCEA(iIconIndx)));
	FreeLibrary(mod);
}
// ��Ӱ�ť����Ԥ�裩
void TASKDIALOGEX::AddButton(TASKDIALOG_COMMON_BUTTON_FLAGS tcbfSet) {
	tdc.dwCommonButtons |= tcbfSet;
}
// ��Ӱ�ť���Զ��壩
void TASKDIALOGEX::AddButton(INT iButtonID, std::string lpcName, std::string lpcCont /* = "" */) {
	if (tmp.tdbc >= __TD_MAX_BUTTON_NUM) return;
	if (lpcCont != "") lpcName = (lpcName + (char)10 + lpcCont);
	tmp.ButtonName[tmp.tdbc] = LPC2LPW(lpcName);
	tmp.tdb[tmp.tdbc].nButtonID = iButtonID;
	tmp.tdbc++;
}
// ����Ĭ�ϰ�ť
void TASKDIALOGEX::SetDefaultButton(INT iButtonIndx) {
	tdc.nDefaultRadioButton = iButtonIndx;
}
// ��ӵ�ѡ��ť
void TASKDIALOGEX::AddRButton(INT iButtonID, std::string lpcName) {
	if (tmp.tdrbc >= __TD_MAX_RBUTTON_NUM) return;
	tmp.RButtonName[tmp.tdrbc] = LPC2LPW(lpcName);
	tmp.tdrb[tmp.tdrbc].nButtonID = iButtonID;
	tmp.tdrbc++;
}
// ����Ĭ�ϵ�ѡ��ť
void TASKDIALOGEX::SetDefaultRButton(INT iButtonIndx) {
	tdc.nDefaultRadioButton = iButtonIndx;
}
// ���ø�ѡ������
void TASKDIALOGEX::SetVeriText(std::string lpcVeriText) {
	tmp.VeriText = LPC2LPW(lpcVeriText);
	return;
}
// ���ø�����Ϣ���ġ�����ʾ�������ʾ��
void TASKDIALOGEX::SetExpanded(std::string lpcText, std::string lpcOpenText, std::string lpcCloseText) {
	tmp.ExpText = LPC2LPW(lpcText);
	tmp.ExpOpenText = LPC2LPW(lpcOpenText);
	tmp.ExpCloseText = LPC2LPW(lpcCloseText);
	return;
}
// ������Ϣ��Ӧ�Ļص�����
void TASKDIALOGEX::SetCallbackFunc(UINT uiNoti, TASKDIALOGCBFUNC fCBFunc) {
	if (uiNoti < 0 || uiNoti > 10) return;
	if (fCBFunc.type != __TASKDIALOG_CBFUNC_CORRECTARGS[uiNoti]) return;
	cbf[uiNoti] = fCBFunc;
}
// ���ô��ݸ��ص������Ĳ���ָ��
void TASKDIALOGEX::SetCallbackArgs(LPVOID vArg) {
	if (vArg == NULL) return;
	tmp.vCBFArg.vUserArg = reinterpret_cast <LONG_PTR> (vArg);
}
// ���ô��ڳ�ʼ����
void TASKDIALOGEX::SetPoint(INT iX, INT iY) {
	tmp.iX = iX;
	tmp.iY = iY;
}
// ���ô����Ƿ��ö�
void TASKDIALOGEX::SetIsOnTop(BOOL bIsOnTop) {
	tmp.bOnTop = bIsOnTop;
}
// ��������������ֱ�ӷ��أ������ӣ�cmd����ǰ��Ҫ��cmd /c��cmd /k��
std::string TASKDIALOGEX::CreateHLink(std::string lpId, std::string lpText, std::string lpCmd /* = "" */ ) {
	std::string ret = "<a href=\"" + lpId + "\">" + lpText + "</a>";
	if (tmp.Link.count(lpId)) {
		if (lpCmd != "") tmp.Link[lpId] = lpCmd;
	} else {
		if (lpCmd == "") return "";
		tmp.Link.insert({ lpId, lpCmd });
	}
	SetAttrib(TDF_ENABLE_HYPERLINKS, TDF_ENABLE);
	return ret;
}

// ==============================����ʱ����==============================
// ��ʾ�Ի���
LONG TASKDIALOGEX::Show() {
	Update();
	LONG res = TaskDialogIndirect(&tdc, &tmp.iButtonID, &tmp.iRadioIndex, &tmp.bVeriIsOK);
	return res;
}
// ��ʾ�Ի��������߳��
void TASKDIALOGEX::ShowInNewThread() {
	//�����̶߳࿪
	if (tmp.hThread != NULL) return;
	tmp.lThResult = 0;
#ifndef DISABLE_NEW_THREAD
	tmp.hThread = CreateThread(NULL, 0, __thFunc, tmp.lpThreadThis, 0, NULL);
#else
	__thFunc(tmp.lpThreadThis);
#endif
}
// �ȴ��򿪵��߳̽�������ȡ����ֵ
LONG TASKDIALOGEX::WaitThreadEnd() const {
#ifndef DISABLE_NEW_THREAD
	WaitForSingleObject(tmp.hThread, INFINITE);
	return tmp.lThResult;
#else
	return 0;
#endif
}
// ��ȡ���ھ������ʾ֮ǰ����NULL��
HWND TASKDIALOGEX::GetHWND() const {
	return tmp.hWnd;
}
// ����ʾ�Ĵ��ڷ�����Ϣ���������߳��еĴ��ڣ�
void TASKDIALOGEX::SendWndMessage(UINT uiMsg, WPARAM wParam /* = 0 */, LPARAM lParam /* = 0*/ ) const {
	if (tmp.hWnd == NULL) return;
	SendMessageA(tmp.hWnd, uiMsg, wParam, lParam);
}
// �жϴ����Ƿ������У��������߳��еĴ��ڣ�
BOOL TASKDIALOGEX::IsActive() const {
	DWORD ret;
	GetExitCodeThread(tmp.hThread, &ret);
	return ret == STILL_ACTIVE;
}
// �������������
void TASKDIALOGEX::TopMost() const {
	SetWindowPos(GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

// ==============================���к���==============================
// ��ñ����µİ�ť��ID
INT TASKDIALOGEX::GetButtonID() const {
	return tmp.iButtonID;
}
// ��ñ�ѡ�еĵ�ѡ��ť��ID
INT TASKDIALOGEX::GetRButtonID() const {
	return tmp.iRadioIndex;
}
// ��ø�ѡ��ѡ����Ϣ��δѡ��0��ѡ��1��
BOOL TASKDIALOGEX::GetVeriState() const {
	return tmp.bVeriIsOK;
}
