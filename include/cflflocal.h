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
#   define LOCAL_SYS_earlyError "Ошибка при запуске в "
#   define LOCAL_UI_appName "CFLF"
#   define LOCAL_ERR_noError "Нет ошибки?"
#   define LOCAL_ERR_errorNumber "Ошибка "
#   define LOCAL_ERR_atLocation " в "
#   define LOCAL_ERR_moreInfo ". Подробности:\n"
#   define LOCAL_UI_start "Запустить бота"
#   define LOCAL_UI_stop "Остановить бота"
#   define LOCAL_UI_latency "Пауза между рывками"
#   define LOCAL_UI_updateWindowsList "Обновить список окон"
#   define LOCAL_BOT_gameNotFound "Игра не найдена, пробую снова..."
#   define LOCAL_BOT_gameFirstGrayTabFoundAt "Левый верхний угол первой коричневой вкладки найден в координатах "  
#   define LOCAL_BOT_gameIsLost "Игра потеряна, пробую наёти её снова..."
#   define LOCAL_UI_initialStatus "Нажми кнопку выше и кликни по окну с игрой для старта"
#   define LOCAL_UI_sleepTime "Пауза между рывками (мс.)"
#endif
