# TaskDialogEx
Simple encapsulation of TaskDialog on Windows.

TaskDialog is a dialog box encapsulated in commctrl. It can be seen as a super enhanced version of Message Box, which can be applied in many software such as Windows registry editor.

The character set dynamically adjusts according to the Unicode macro.

**Rely on Convert.h** [https://github.com/xmc0211/Convert/](https://github.com/xmc0211/Convert/)

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

### TASKDIALOG FLAGS

Can be used for the first parameter of AddAttrib()

| Flag | Value | Description |
| :-----: | :-----: | :----- |
| TDF_ENABLE_HYPERLINKS | 0x0001 | [AUTO] Allow hyperlinks to be used in TaskDialog.|
| TDF_USE_HICON_MAIN | 0x0002 | [AUTO] Use the large icon (designated by the hMainIcon member) displayed in the main content area.|
| TDF_USE_HICON_FOOTER | 0x0004 | [AUTO] Use the small icon (designated by the hFooterIcon member) displayed in the footer area.|
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

### TASKDIALOG MESSAGES

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

### TASKDIALOG ICONS

Can be used for the first parameter of SetMainIcon() or SetFooterIcon()

**Main Icon**

| Icon Name | Value | Picture |
| :-----: | :-----: | :-----: |
| TDMI_WARNING | -1 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_WARNING.png?raw=true) |
| TDMI_ERROR | -2 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_ERROR.png?raw=true) |
| TDMI_INFORMATION | -3 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_INFORMATION.png?raw=true) |
| TDMI_SHIELD | -4 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELD.png?raw=true) |
| TDMI_SHIELD_BLUE | -5 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELD_BLUE.png?raw=true) |
| TDMI_SHIELDWARNING_YELLOW | -6 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELDWARNING_YELLOW.png?raw=true) |
| TDMI_SHIELDERROR_RED | -7 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELDERROR_RED.png?raw=true) |
| TDMI_SHIELDSUCCESS_GREEN | -8 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELDSUCCESS_GREEN.png?raw=true) |
| TDMI_SHIELD_GREY | -9 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDMI_SHIELD_GREY.png?raw=true) |

**Footer Icon**
| Icon Name | Value | Picture |
| :-----: | :-----: | :-----: |
| TDFI_WARNING | -1 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_WARNING.png?raw=true) |
| TDFI_ERROR | -2 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_ERROR.png?raw=true) |
| TDFI_INFORMATION | -3 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_INFORMATION.png?raw=true) |
| TDFI_SHIELD | -4 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_SHIELD.png?raw=true) |
| TDFI_SHIELDWARNING | -6 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_SHIELDWARNING.png?raw=true) |
| TDFI_SHIELDERROR | -7 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_SHIELDERROR.png?raw=true) |
| TDFI_SHIELDSUCCESS | -8 | ![Picture](https://github.com/xmc0211/TaskDialogEx/blob/main/Assets/TDFI_SHIELDSUCCESS.png?raw=true) |

### TASKDIALOG NOTIFICATIONS

Can be used for the first parameter of SetCallbackFunc()

| Notification | Param count | Description | wParam | lParam |
| :-----: | :-----: | :----- | :----- | :----- |
| TDN_CREATED | 0 | Send when the task dialog box is created.| | |
| TDN_NAVIGATED | 0 | Sent when the user navigates to different pages or tags in the dialog box.| | |
| TDN_BUTTON_CLICKED | 1 | Send when the user clicks the button in the dialog box.| Button ID | |
| TDN_HYPERLINK_CLICKED | 2 | Sent when the user clicks on the hyperlink in the dialog box.| Always 0 | URL pointing to the clicked hyperlink (LPCWSTR type) |
| TDN_TIMER | 1 | Timer event, used to notify the dialog box of the time it already exists.| The number of milliseconds since the creation of the dialog box or timer reset | |
| TDN_DESTROYED | 0 | Send when the task dialog box is destroyed.| | |
| TDN_RADIO_BUTTON_CLICKED | 1 | Sent when the user selects the radio button in the dialog box.| Radio button ID | |
| TDN_DIALOG_CONSTRUCTED | 0 | Send when the dialog box is fully built and ready to display.| | |
| TDN_VERIFICATION_CLICKED | 1 | Sent when the user clicks the verify checkbox in the dialog box.| The status of the checkbox (1 indicates selected, 0 indicates unselected) | |
| TDN_HELP | 0 | Send when the user requests help (such as clicking the help button or pressing the F1 key).| | |
| TDN_EXPANDO_BUTTON_CLICKED | 1 | Sent when the user clicks the expand/collapse button in the dialog box.| A value of 0 indicates that the dialog box is now in a folded state, while a value of non-zero indicates that the dialog box is now in an unfolded state | |

### Pre set button ID

Used to check the return value of GetButtonID().

| Button ID | Value |
| :-----: | :-----: |
| IDOK | 1 |
| IDCANCEL | 2 |
| IDABORT | 3 |
| IDRETRY | 4 |
| IDIGNORE | 5
| IDYES | 6 |
| IDNO | 7 |
| IDCLOSE | 8 |
| IDHELP | 9 |
| IDTRYAGAIN | 10 |
| IDCONTINUE | 11 |
| IDTIMEOUT | 12 |

### Pre configured button options

The first parameter used for AddButton() (using preset version)

| Button ID | Value |
| :-----: | :-----: |
| TDCBF_OK_BUTTON | 0x01 |
| TDCBF_YES_BUTTON | 0x02 |
| TDCBF_NO_BUTTON | 0x04 |
| TDCBF_CANCEL_BUTTON | 0x08 |
| TDCBF_RETRY_BUTTON | 0x10 |
| TDCBF_CLOSE_BUTTON | 0x20 |

- - -
