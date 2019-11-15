// This file must be saved in local encoding (Windows 1251 for Russian)

#include <cflf.h>

#ifdef LANG_EN
#   define LOCAL_SYS_earlyError "Early error in "
#   define LOCAL_UI_appName "CFLF"
#   define LOCAL_ERR_noError "No error?"
#   define LOCAL_ERR_errorNumber "Error "
#   define LOCAL_ERR_atLocation " in "
#   define LOCAL_ERR_moreInfo ". More info:\n"
#   define LOCAL_UI_start "Activate bot"
#   define LOCAL_UI_stop "Stop bot"
#   define LOCAL_UI_latency "Bot latency"
#   define LOCAL_UI_updateWindowsList "Update windows list"
#   define LOCAL_BOT_gameNotFound "Game not found, trying again..."
#   define LOCAL_BOT_gameFirstGrayTabFoundAt "Game first gray tab found at " 
#   define LOCAL_BOT_gameIsLost "Game is lost, trying to find it again..."
#   define LOCAL_UI_initialStatus "Press button under the label and click on game window to start"
#   define LOCAL_UI_sleepTime "Time between catches (ms.)"
#else
#   define LOCAL_SYS_earlyError "������ ��� ������� � "
#   define LOCAL_UI_appName "CFLF"
#   define LOCAL_ERR_noError "��� ������?"
#   define LOCAL_ERR_errorNumber "������ "
#   define LOCAL_ERR_atLocation " � "
#   define LOCAL_ERR_moreInfo ". �����������:\n"
#   define LOCAL_UI_start "��������� ����"
#   define LOCAL_UI_stop "���������� ����"
#   define LOCAL_UI_latency "����� ����� �������"
#   define LOCAL_UI_updateWindowsList "�������� ������ ����"
#   define LOCAL_BOT_gameNotFound "���� �� �������, ������ �����..."
#   define LOCAL_BOT_gameFirstGrayTabFoundAt "����� ������� ���� ������ ���������� ������� ������ � ����������� "  
#   define LOCAL_BOT_gameIsLost "���� ��������, ������ ���� � �����..."
#   define LOCAL_UI_initialStatus "����� ������ ���� � ������ �� ���� � ����� ��� ������"
#   define LOCAL_UI_sleepTime "����� ����� ������� (��.)"
#endif
