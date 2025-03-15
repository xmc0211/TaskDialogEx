# TaskDialogEx
Simple encapsulation of TaskDialog on Windows.

TaskDialog is a dialog box encapsulated in commctrl.

- - -
## Example

```cpp
#include <windows.h>
#include "TaskDialogEx.h"

int main() {
    TASKDIALOGEX tdx;
    tdx.SetAttrib(TDF_ALLOW_DIALOG_CANCELLATION);
    tdx.SetCheckboxText(TEXT("Checkbox Text"));
    tdx.SetContent(TEXT("Content\nSecond Line\n" + tdx.CreateHyperlink(TEXT("ID"), TEXT("Hyperlink"), TEXT("CALC.EXE"))));
    tdx.SetExpanded(TEXT("Expanded text"), TEXT("Open text"), TEXT("Close Text"));
    tdx.SetFooter(TEXT("Footer"));
    tdx.SetMainInstruction(TEXT("Main Instruction"));
    tdx.SetWindowTitle(TEXT("Window Title"));

    tdx.AddButton(TDCBF_OK_BUTTON | TDCBF_RETRY_BUTTON);
    tdx.AddButton(201, TEXT("Custom button"));
    tdx.AddRadioButton(101, TEXT("Radio Button 1"));
    tdx.AddRadioButton(102, TEXT("Radio Button 2"));
    tdx.AddRadioButton(103, TEXT("Radio Button 3"));

    tdx.SetMainIcon(TDMI_SHIELDSUCCESS_GREEN);
    tdx.SetFooterIcon(TDFI_INFORMATION);

    tdx.Show();
    return 0;
}
```

Using this code, you can create a TaskDialog with the following effect:

![TaskDialogEx_png](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TaskDialogEx.png?raw=true)

- - -
## Explanation of TaskDialogEx enumerations

### TASKDIALOG_FLAGS

Can be used for the first parameter of AddAttrib()

| Flag | Value | Description|
| :--------------------: | :---------------------: | :---------------------: |
| TDF_ENABLE_HYPERLINKS | 0x0001 | 允许在 TaskDialog 中使用超链接。|
| TDF_USE_HICON_MAIN | 0x0002 | 使用在主要内容区域显示的大图标（hMainIcon 成员指定的图标）。|
| TDF_USE_HICON_FOOTER | 0x0004 | 使用在页脚区域显示的小图标（hFooterIcon 成员指定的图标）。|
| TDF_ALLOW_DIALOG_CANCELLATION | 0x0008 | 允许用户通过关闭按钮（如右上角的“X”按钮）或按 Esc 键来取消对话框。|
| TDF_USE_COMMAND_LINKS | 0x0010 | 使用命令链接（带有图标的按钮）代替普通按钮。|
| TDF_USE_COMMAND_LINKS_NO_ICON | 0x0020 | 使用没有图标的命令链接按钮。|
| TDF_EXPAND_FOOTER_AREA | 0x0040 | 扩展页脚区域的高度，以便显示更多的信息或控件。|
| TDF_EXPANDED_BY_DEFAULT | 0x0080 | 默认情况下扩展页脚区域（如果设置了 TDF_EXPAND_FOOTER_AREA）。|
| TDF_VERIFICATION_FLAG_CHECKED | 0x0100 | 初始化时选中验证标志（复选框）。|
| TDF_SHOW_PROGRESS_BAR | 0x0200 | 显示进度条。|
| TDF_SHOW_MARQUEE_PROGRESS_BAR | 0x0400 | 显示不确定的（循环的）进度条（马拉松式进度条）。|
| TDF_CALLBACK_TIMER | 0x0800 | 启用定时器回调，以便在对话框显示期间定期执行某些操作。|
| TDF_POSITION_RELATIVE_TO_WINDOW | 0x1000 | 将对话框相对于指定窗口定位（使用 hwndParent 成员）。|
| TDF_RTL_LAYOUT | 0x2000 | 使用从右到左的布局（适用于阿拉伯语、希伯来语等语言）。|
| TDF_NO_DEFAULT_RADIO_BUTTON | 0x4000 | 不设置默认的单选按钮。|
| TDF_CAN_BE_MINIMIZED | 0x8000 | 允许对话框被最小化。|
| TDF_NO_SET_FOREGROUND | 0x00010000 | 当激活对话框时，不调用 SetForegroundWindow()（仅在 Windows 8 及更高版本上有效）。|
| TDF_SIZE_TO_CONTENT | 0x01000000 | 根据内容调整对话框的大小（这通常用于模仿 MessageBox 的大小调整行为）。|

