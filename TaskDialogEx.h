// TaskDialogEx.h by XMC
// ����Convert.h by XMC

// * ����ͨ��#define NOUSING_NEW_COMMCTRL_PRAGMAʹͷ�ļ�
// ��ͨ��#pragmaʹ��Commctrl 6.0.0.0�����֡��޷���λ��
// ��345��������Ҹñ�ʶ�ѱ�����ʱ���Գ���ɾ��������
// * ����ͨ��#define NO_HYPERLINK_AUTORUN��ֹͷ�ļ��ڵ�
// ��������ʱ���Զ�ִ�����
// * �벻Ҫ��TaskDialog�����߳�������ʱ�����߳�������
// TaskDialogEx�������ֶ��ͷţ��˳�ʵ��������������
// ������if��for�ȣ�

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
	// ����
	__CBFUNCARG();
}; // ���ֻص������Ĳ����ṹ
struct TASKDIALOGCBFUNC {
	int type;
	TASKDIALOG_FNOPARAM nop;
	TASKDIALOG_FONEPARAM op;
	TASKDIALOG_FTWOPARAM tp;
	// ����
	TASKDIALOGCBFUNC();
	TASKDIALOGCBFUNC(TASKDIALOG_FNOPARAM func);
	TASKDIALOGCBFUNC(TASKDIALOG_FONEPARAM func);
	TASKDIALOGCBFUNC(TASKDIALOG_FTWOPARAM func);
	// ����
	~TASKDIALOGCBFUNC();

	// �������еĺ���
	void Execute(HWND hWnd, LONG_PTR uiP1 = 0, LONG_PTR uiP2 = 0, LONG_PTR vArg = NULL);
}; // ͨ�õĻص������ṹ
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
	// ����
	TASKDIALOGTMP();
	// ����
	TASKDIALOGTMP(const TASKDIALOGTMP& other);
	// ����
	~TASKDIALOGTMP();
}; // ���ڴ��TASKDIALOGEX����ʱ����

// TASKDIALOGCONFIG�ķ�װ
class TASKDIALOGEX {
protected:
	TASKDIALOGCONFIG tdc;
	TASKDIALOGCBFUNC cbf[11];
public:
	TASKDIALOGTMP tmp;
protected:
	// ˢ����Ϣ
	void Update();
	// Ԥ�ûص�����
	static HRESULT CALLBACK __cbFunc(HWND hWnd, UINT uiNoti, WPARAM wParam, LPARAM lParam, LONG_PTR lpArg);
	// Ԥ���߳����к�����ʹ��LPVOID��Ϊ���ʵ��ָ��
	static DWORD WINAPI __thFunc(LPVOID lpParam);
public:
	// ����
	TASKDIALOGEX();
	// ����
	TASKDIALOGEX(const TASKDIALOGEX& other);
	// ����
	~TASKDIALOGEX();

	TASKDIALOGCONFIG GetCurrentConfig();
	TASKDIALOGCBFUNC GetCurrentCBFunc(INT iIndx);

// ==============================����ǰ����==============================
	// �������ԣ����û��Ƴ�������á�|�����ӣ�
	void SetAttrib(INT iFlag, TASKDIALOGFLAGOPTION iOpt = TDF_ENABLE);
	// ���ô��ڿ��
	void SetWidth(UINT uiWidth);
	// ���ô��ڱ���
	void SetWindowTitle(std::string lpcWindowTitle);
	// �����ı�����
	void SetContent(std::string lpcContent);
	// �����ı�����
	void SetMainInstruction(std::string lpcMainIntruction);
	// ������ͼ�꣨��Ԥ�裩
	void SetMainIcon(TASKDIALOGMAINICON iIconID);
	// ������ͼ�꣨�Զ��壬HICON��
	void SetMainIcon(HICON hIcon);
	// ������ͼ�꣨�Զ��壬ָ��dll���ƺ�ͼ��������
	void SetMainIcon(std::string lpcDllName, INT iIconIndx);
	// ���õײ���������
	void SetFooter(std::string lpcFooter);
	// ���õײ�����ͼ�꣨��Ԥ�裩
	void SetFooterIcon(TASKDIALOGFOOTERICON iIconID);
	// ���õײ�����ͼ�꣨�Զ��壬HICON��
	void SetFooterIcon(HICON hIcon);
	// ���õײ�����ͼ�꣨�Զ��壬ָ��dll���ƺ�ͼ��������
	void SetFooterIcon(std::string lpcDllName, INT iIconIndx);
	// ��Ӱ�ť����Ԥ�裩
	void AddButton(TASKDIALOG_COMMON_BUTTON_FLAGS tcbfSet);
	// ��Ӱ�ť���Զ��壩
	void AddButton(INT iButtonID, std::string lpcName, std::string lpcCont = "");
	// ����Ĭ�ϰ�ť
	void SetDefaultButton(INT iButtonIndx);
	// ��ӵ�ѡ��ť
	void AddRButton(INT iButtonID, std::string lpcName);
	// ����Ĭ�ϵ�ѡ��ť
	void SetDefaultRButton(INT iButtonIndx);
	// ���ø�ѡ������
	void SetVeriText(std::string lpcVeriText);
	// ���ø�����Ϣ���ġ�����ʾ�������ʾ��
	void SetExpanded(std::string lpcText, std::string lpcOpenText, std::string lpcCloseText);
	// ������Ϣ��Ӧ�Ļص�����
	void SetCallbackFunc(UINT uiNoti, TASKDIALOGCBFUNC fCBFunc);
	// ���ô��ݸ��ص������Ĳ���ָ��
	void SetCallbackArgs(LPVOID vArg);
	// ���ô��ڳ�ʼ����
	void SetPoint(INT iX, INT iY);
	// ���ô����Ƿ��ö�
	void SetIsOnTop(BOOL bIsOnTop);
	// ��������������ֱ�ӷ��أ������ӣ�cmd����ǰ��Ҫ��cmd /c��cmd /k��
	std::string CreateHLink(std::string lpId, std::string lpText, std::string lpCmd = "");

// ==============================����ʱ����==============================
	// ��ʾ�Ի���
	LONG Show();
	// ��ʾ�Ի��������߳��
	void ShowInNewThread();
	// �ȴ��򿪵��߳̽�������ȡ����ֵ
	LONG WaitThreadEnd() const;
	// ��ȡ���ھ������ʾ֮ǰ����NULL��
	HWND GetHWND() const;
	// ����ʾ�Ĵ��ڷ�����Ϣ���������߳��еĴ��ڣ�
	void SendWndMessage(UINT uiMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	// �жϴ����Ƿ������У��������߳��еĴ��ڣ�
	BOOL IsActive() const;
	// �������������
	void TopMost() const;

// ==============================���к���==============================
	// ��ñ����µİ�ť��ID
	INT GetButtonID() const;
	// ��ñ�ѡ�еĵ�ѡ��ť��ID
	INT GetRButtonID() const;
	// ��ø�ѡ��ѡ����Ϣ��δѡ��0��ѡ��1��
	BOOL GetVeriState() const;
};

#endif /** #ifndef TASKDIALOGEX_H **/

/* ע�ͣ����Դ����︴�Ʊ�Ҫ����Ϣ��
* 
* 1.�й�TASKDIALOGCONFIG��Ա��˵��
* 
* cbSize (UINT):									�ṹ�Ĵ�С�����ֽ�Ϊ��λ����ͨ�����ڰ汾���ƣ���ȷ�����ݸ� API �Ľṹ��С����ȷ�ġ�
* hwndParent (HWND):								ӵ���ߴ��ڵľ���������Ǹ����ڡ����Ի�����ʾʱ�����������������ڶ�λ��������ȡ���� dwFlags �е� TDF_POSITION_RELATIVE_TO_WINDOW ��־����
* hInstance (HINSTANCE):							�����Ի�����ʹ�õ��ַ�����Դ��ģ��ʵ���������Щ�ַ�������ͨ�� MAKEINTRESOURCE() �������á�
* dwFlags (TASKDIALOG_FLAGS):						һ����־λ���ϣ�����ָ���Ի������Ϊ����ۡ���Щ��־λ�����Ƿ����ó����ӡ��Ƿ�ʹ�ô�ͼ���Сͼ�ꡢ�Ƿ�����ȡ���Ի���ȡ�
* dwCommonButtons (TASKDIALOG_COMMON_BUTTON_FLAGS):	һ����־λ���ϣ�����ָ���Ի�����Ҫ��ʾ�ĳ�����ť���硰ȷ��������ȡ���������ǡ������񡱵ȣ���
* pszWindowTitle (PCWSTR):							ָ��Ի��򴰿ڱ�����ַ���ָ�룬������ͨ�� MAKEINTRESOURCE() �����õ���Դ ID��
* DUMMYUNIONNAME (����):							һ�����ϣ�����ָ����Ҫ�����ͼ�ꡣ������ͼ��ľ�� (hMainIcon) ��ָ��ͼ���ļ������ַ���ָ�� (pszMainIcon)��
* pszMainInstruction (PCWSTR):						ָ����Ҫָ���ı����ַ���ָ�룬ͨ���ǶԻ����������ı�����
* pszContent (PCWSTR):								ָ�������ı����ַ���ָ�룬ͨ����ʾ����Ҫָ���ı��·���
* cButtons (UINT):									�Զ��尴ť��������
* pButtons (const TASKDIALOG_BUTTON *):				ָ��һ�� TASKDIALOG_BUTTON �ṹ�����ָ�룬���ڶ����Զ��尴ť��
* nDefaultButton (int):								Ĭ�ϰ�ť���������� pButtons �����У������û��� Enter ��ʱ�������
* cRadioButtons (UINT):								��ѡ��ť��������
* pRadioButtons (const TASKDIALOG_BUTTON *):		ָ��һ�� TASKDIALOG_BUTTON �ṹ�����ָ�룬���ڶ��嵥ѡ��ť��
* nDefaultRadioButton (int):						Ĭ�ϵ�ѡ��ť���������� pRadioButtons �����У�������ʼ��ʾ�Ի���ʱ����ѡ�С�
* pszVerificationText (PCWSTR):						ָ����֤�ı����ַ���ָ�룬ͨ���븴ѡ��һ��ʹ�á�
* pszExpandedInformation (PCWSTR):					ָ����չ��Ϣ���ַ���ָ�룬���û�չ��ҳ������ʱ��ʾ��
* pszExpandedControlText (PCWSTR):					ָ����չ�ؼ��ı����ַ���ָ�룬ͨ������������չ�����еĿؼ����縴ѡ����ı��򣩡�
* pszCollapsedControlText (PCWSTR):					ָ���۵��ؼ��ı����ַ���ָ�룬��ҳ�������۵�ʱ��ʾ��
* DUMMYUNIONNAME2 (����):							һ�����ϣ�����ָ��ҳ�������ͼ�ꡣ������ͼ��ľ�� (hFooterIcon) ��ָ��ͼ���ļ������ַ���ָ�� (pszFooterIcon)��
* pszFooter (PCWSTR):								ָ��ҳ���ı����ַ���ָ�룬ͨ����ʾ�ڶԻ���ĵײ���
* pfCallback (PFTASKDIALOGCALLBACK):				ָ��һ���ص�������ָ�룬���Ի����е�ĳЩ�¼�����ʱ���簴ť���������ʱ�������ȣ������øú�����
* lpCallbackData (LONG_PTR):						���ݸ��ص��������û��������ݡ�
* cxWidth (UINT):									�Ի���ͻ�������Ŀ�ȣ��ԶԻ����߼���λ��DLU��Ϊ��λ�����Ϊ 0����Ի��򽫼�������Ŀ�ȡ�
* 
* 2.�й�TDF_*(_TASKDIALOG_FLAGS)����Ӧ���ܵ�˵����������AddAttrib()��һ��������
* 
* TDF_ENABLE_HYPERLINKS (0x0001):			������ TaskDialog ��ʹ�ó����ӡ�
* TDF_USE_HICON_MAIN (0x0002):				ʹ������Ҫ����������ʾ�Ĵ�ͼ�꣨hMainIcon ��Աָ����ͼ�꣩��
* TDF_USE_HICON_FOOTER (0x0004):			ʹ����ҳ��������ʾ��Сͼ�꣨hFooterIcon ��Աָ����ͼ�꣩��
* TDF_ALLOW_DIALOG_CANCELLATION (0x0008):	�����û�ͨ���رհ�ť�������Ͻǵġ�X����ť���� Esc ����ȡ���Ի���
* TDF_USE_COMMAND_LINKS (0x0010):			ʹ���������ӣ�����ͼ��İ�ť��������ͨ��ť��
* TDF_USE_COMMAND_LINKS_NO_ICON (0x0020):	ʹ��û��ͼ����������Ӱ�ť��
* TDF_EXPAND_FOOTER_AREA (0x0040):			��չҳ������ĸ߶ȣ��Ա���ʾ�������Ϣ��ؼ���
* TDF_EXPANDED_BY_DEFAULT (0x0080):			Ĭ���������չҳ��������������� TDF_EXPAND_FOOTER_AREA����
* TDF_VERIFICATION_FLAG_CHECKED (0x0100):	��ʼ��ʱѡ����֤��־����ѡ�򣩡�
* TDF_SHOW_PROGRESS_BAR (0x0200):			��ʾ��������
* TDF_SHOW_MARQUEE_PROGRESS_BAR (0x0400):	��ʾ��ȷ���ģ�ѭ���ģ���������������ʽ����������
* TDF_CALLBACK_TIMER (0x0800):				���ö�ʱ���ص����Ա��ڶԻ�����ʾ�ڼ䶨��ִ��ĳЩ������
* TDF_POSITION_RELATIVE_TO_WINDOW (0x1000):	���Ի��������ָ�����ڶ�λ��ʹ�� hwndParent ��Ա����
* TDF_RTL_LAYOUT (0x2000):					ʹ�ô��ҵ���Ĳ��֣������ڰ������ϣ����������ԣ���
* TDF_NO_DEFAULT_RADIO_BUTTON (0x4000):		������Ĭ�ϵĵ�ѡ��ť��
* TDF_CAN_BE_MINIMIZED (0x8000):			����Ի�����С����
* TDF_NO_SET_FOREGROUND (0x00010000):		������Ի���ʱ�������� SetForegroundWindow()������ Windows 8 �����߰汾����Ч����
* TDF_SIZE_TO_CONTENT (0x01000000):			�������ݵ����Ի���Ĵ�С����ͨ������ģ�� MessageBox �Ĵ�С������Ϊ����
* 
* 3.�й�TDM_*(_TASKDIALOG_MESSAGES)����Ӧ���ܵ�˵�����ڶ����ǲ�����������������SendWndMessage()��һ��������
* 
* TDM_NAVIGATE_PAGE:						0	���ڵ�����ָ��ҳ�档
* TDM_CLICK_BUTTON:							1	ģ������ť��wParamΪ��ťID��
* TDM_SET_MARQUEE_PROGRESS_BAR:				1	���ý������Ƿ�Ϊ�״̬��wParamΪ0��ʾ�ǻ����0��ʾ���
* TDM_SET_PROGRESS_BAR_STATE:				1	���½�����״̬��wParamΪ�µ�״̬��
* TDM_SET_PROGRESS_BAR_RANGE:				1	���ý�������Χ��lParamΪMAKELPARAM(nMinRange, nMaxRange)��
* TDM_SET_PROGRESS_BAR_POS:					1	���½�����λ�ã�wParamΪ�µ�λ�á�
* TDM_SET_PROGRESS_BAR_MARQUEE:				2	���ƽ�����������wParamΪ0ֹͣ����0��ʼ��lParamΪ�����ٶȡ�
* TDM_SET_ELEMENT_TEXT:						2	����Ԫ���ı���wParamΪԪ��(TDE_*)��lParamΪ���ı���
* TDM_CLICK_RADIO_BUTTON:					1	ģ������ѡ��ť��wParamΪ��ѡ��ťID��
* TDM_ENABLE_BUTTON:						2	����/���ð�ť��lParamΪ0���ã���0���ã�wParamΪ��ťID��
* TDM_ENABLE_RADIO_BUTTON:					2	����/���õ�ѡ��ť��lParamΪ0���ã���0���ã�wParamΪ��ťID��
* TDM_CLICK_VERIFICATION:					1	ģ������֤��ѡ��wParamΪ0δѡ�У�1ѡ�С�
* TDM_UPDATE_ELEMENT_TEXT:					2	����Ԫ���ı���wParamΪԪ��(TDE_*)��lParamΪ���ı���
* TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE:	2	���ð�ť�Ƿ���Ҫ����Ȩ�ޣ�wParamΪ��ťID��lParamΪ0����Ҫ����0��Ҫ��
* TDM_UPDATE_ICON:							2	����ͼ�꣬wParamΪͼ��Ԫ��(TDIE_ICON_*)��lParamΪ��ͼ������·����
* 
* 4.�й�TDN_*(_TASKDIALOG_NOTIFICATIONS)����Ӧ���ܵ�˵�����ڶ����ǲ�����������������SetCallbackFunc()��һ��������
* 
* TDN_CREATED:					0	������Ի��򱻴���ʱ���͡�
* TDN_NAVIGATED:				0	���û��������Ի����еĲ�ͬҳ����ǩʱ���͡�
* TDN_BUTTON_CLICKED:			1	�û�����Ի����еİ�ťʱ���ͣ�wParamΪ��ť�� ID��
* TDN_HYPERLINK_CLICKED:		2	���û�����Ի����еĳ�����ʱ���ͣ�wParamʼ��Ϊ0��lParamΪָ�򱻵���ĳ����ӵ� URL (LPCWSTR ����)��
* TDN_TIMER:					1	��ʱ���¼�������֪ͨ�Ի����Ѿ����ڵ�ʱ�䣬wParamΪ�ԶԻ��򴴽���ʱ�����������ĺ�������
* TDN_DESTROYED:				0	������Ի�������ʱ���͡�
* TDN_RADIO_BUTTON_CLICKED:		1	���û�ѡ��Ի����еĵ�ѡ��ťʱ���ͣ�wParamΪ��ѡ��ť�� ID��
* TDN_DIALOG_CONSTRUCTED:		0	���Ի�����ȫ������׼����ʾʱ���͡�
* TDN_VERIFICATION_CLICKED:		1	���û�����Ի����е���֤��ѡ��ʱ���ͣ�wParamΪ��ѡ���״̬��1��ʾѡ�У�0��ʾδѡ�У���
* TDN_HELP:						0	���û��������ʱ���ͣ����磬���������ť�� F1 ������
* TDN_EXPANDO_BUTTON_CLICKED:	1	���û�����Ի����е���չ/�۵���ťʱ���ͣ�wParamΪ0��ʾ�Ի����������۵�״̬����0��ʾ�Ի���������չ��״̬��
* 
* 5.Ԥ�õİ�ťID(ID*)
* 
* IDOK			1   * IDNO			7
* IDCANCEL		2   * IDCLOSE		8
* IDABORT		3   * IDHELP		9
* IDRETRY		4   * IDTRYAGAIN	10
* IDIGNORE		5   * IDCONTINUE	11
* IDYES			6   * IDTIMEOUT		12
* 
* 6.Ԥ�õİ�ťѡ��(TDCBF_*)
* 
* TDCBF_OK_BUTTON		0x01
* TDCBF_YES_BUTTON		0x02
* TDCBF_NO_BUTTON		0x04
* TDCBF_CANCEL_BUTTON	0x08
* TDCBF_RETRY_BUTTON	0x10
* TDCBF_CLOSE_BUTTON	0x20
* 
*/
