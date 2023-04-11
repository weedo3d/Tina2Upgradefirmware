#include "../ultralcd.h"


void wtlcd_endstop_info()
{
	if (use_click())
		return lcd_goto_previous_menu();

	START_SCREEN();
	STATIC_ITEM_MULTI(MMSG_LCD_ENDSTOPS[wtvar_language], true, true);

	if (READ(X_MAX_PIN))
	{
		STATIC_ITEM("X: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("X: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}

	if (READ(Y_MIN_PIN))
	{
		STATIC_ITEM("Y: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("Y: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}

	if (READ(Z_MAX_PIN))
	{
		STATIC_ITEM("Z: ", false, false, MMSG_LCD_ON[wtvar_language]);
	}
	else
	{
		STATIC_ITEM("Z: ", false, false, MMSG_LCD_OFF[wtvar_language]);
	}

	if (READ(Z_MIN_PIN))
	{
		STATIC_ITEM_MULTI(MMSG_ENDSTOP_PROBE[wtvar_language], false, false, MMSG_LCD_OFF[wtvar_language]);
	}
	else
	{
		STATIC_ITEM_MULTI(MMSG_ENDSTOP_PROBE[wtvar_language], false, false, MMSG_LCD_ON[wtvar_language]);
	}

	END_SCREEN();
	lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
}

