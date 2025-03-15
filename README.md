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

| Flag | Value | Description |
| :-----: | :-----: | :----- |
| TDF_ENABLE_HYPERLINKS | 0x0001 | Allow hyperlinks to be used in TaskDialog.|
| TDF_USE_HICON_MAIN | 0x0002 | Use the large icon (designated by the hMainIcon member) displayed in the main content area.|
| TDF_USE_HICON_FOOTER | 0x0004 | Use the small icon (designated by the hFooterIcon member) displayed in the footer area.|
| TDF_ALLOW_DIALOG_CANCELLATION | 0x0008 | Allow users to cancel the dialog box by pressing a close button (such as the "X" button in the upper right corner) or by pressing the Esc key.|
| TDF_USE_COMMAND_LINKS | 0x0010 | Use command links (buttons with icons) instead of regular buttons.|
| TDF_USE_COMMAND_LINKS_NO_ICON | 0x0020 | Use command link buttons without icons.|
| TDF_EXPAND_FOOTER_AREA | 0x0040 | Expand the height of the footer area to display more information or controls.|
| TDF_EXPANDED_BY_DEFAULT | 0x0080 | By default, the footer area is expanded (if TDF_EXPAND-FOOTER Area is set).|
| TDF_VERIFICATION_FLAG_CHECKED | 0x0100 | Select the validation flag (checkbox) during initialization.|
| TDF_SHOW_PROGRESS_BAR | 0x0200 | Display progress bar.|
| TDF_SHOW_MARQUEE_PROGRESS_BAR | 0x0400 | Display an uncertain (looping) progress bar (marathon style progress bar).|
| TDF_CALLBACK_TIMER | 0x0800 | Enable timer callbacks to perform certain operations periodically during the display of the dialog box.|
| TDF_POSITION_RELATIVE_TO_WINDOW | 0x1000 | Position the dialog box relative to the specified window using the hwndParent member.|
| TDF_RTL_LAYOUT | 0x2000 | Use a right to left layout (applicable to languages such as Arabic and Hebrew).|
| TDF_NO_DEFAULT_RADIO_BUTTON | 0x4000 | Do not set default radio buttons.|
| TDF_CAN_BE_MINIMIZED | 0x8000 | Allow the dialog box to be minimized.|
| TDF_NO_SET_FOREGROUND | 0x00010000 | When activating the dialog box, do not call SetForegroundWindow() (only valid on Windows 8 and later versions).|
| TDF_SIZE_TO_CONTENT | 0x01000000 | Adjust the size of the dialog box based on its content (this is typically used to mimic the resizing behavior of a Message Box).|

### TASKDIALOG_MESSAGES

Can be used for the first parameter of SendWndMessage()

| Message | Param count | Description | wParam | lParam |
| :-----: | :-----: | :----- | :----- | :----- |
| TDM_NAVIGATE_PAGE | 0 | Used to navigate to a specified page.| | |
| TDM_CLICK_BUTTON | 1 | Simulate clicking a button.| Button ID | |
| TDM_SET_MARQUEE_PROGRESS_BAR | 1 | Set whether the progress bar is in an active state. | 0 indicates inactive, non-zero indicates active | |
| TDM_SET_PROGRESS_BAR_STATE | 1 | Update the status of the progress bar.| New state | |
| TDM_SET_PROGRESS_BAR_RANGE | 1 | Set the scope of the progress bar.| | MAKELPARAM(nMinRange, nMaxRange) |
| TDM_SET_PROGRESS_BAR_POS | 1 | Update the position of the progress bar.| New Location | |
| TDM_SET_PROGRESS_BAR_MARQUEE | 2 | Control progress bar animation.| Stop at 0, start at non-zero | animation speed |
| TDM_SET_ELEMENT_TEXT | 2 | Set element text.| element(TDE_*) | new text |
| TDM_CLICK_RADIO_BUTTON | 1 | Simulate clicking the radio button.| Radio button ID | |
| TDM_ENABLE_BUTTON | 2 | Enable/Disable button.| Button ID | Disabled for 0, enabled for non-zero |
| TDM_ENABLE_RADIO_BUTTON | 2 | Enable/disable radio button.| Button ID | Disabled for 0, enabled for non-zero |
| TDM_CLICK_VERIFICATION | 1 | Simulate clicking the verification checkbox.| 0 not selected, 1 selected | |
| TDM_UPDATE_ELEMENT_TEXT | 2 | Update element text.| element(TDE_*) | new text |
| TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE | 2 | Do you need to elevate the permissions of the setting button.| Button ID | Not required for 0, required for non-zero |
| TDM_UPDATE_ICON | 2 | Update icon.| icon element(TDIE_ICON_*) | New icon handle or path |
