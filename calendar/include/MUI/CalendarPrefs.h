#ifndef CALENDARPREFS_H
#define CALENDARPREFS_H

/*
**  $VER: CalendarPrefs.h 18.0 (22.2.2004)
**  Includes Release 18.0
**
**  (C) Copyright 2004 Alfonso [alfie] Ranieri <alforan@tin.it>
**      All rights reserved
*/

#ifndef LIBRARIES_MUI_H
#include <libraries/mui.h>
#endif

/***********************************************************************/

#define CALENDARPREFSBASE 0xFEC90190

/***********************************************************************/
/*
** Settings
**/

#define MUICFG_Calendar_DaysNamesTextPen       (CALENDARPREFSBASE+0)  /* v15, PenSpec   */
#define MUICFG_Calendar_DaysNamesBackground    (CALENDARPREFSBASE+1)  /* v16, ImageSpec */
#define MUICFG_Calendar_DaysNamesDetailPen     (CALENDARPREFSBASE+2)  /* v15, PenSpec   */
#define MUICFG_Calendar_DaysBackground         (CALENDARPREFSBASE+3)  /* v15, ImageSpec */
#define MUICFG_Calendar_DaysTextPen            (CALENDARPREFSBASE+4)  /* v15, PenSpec   */
#define MUICFG_Calendar_DaysShinePen           (CALENDARPREFSBASE+5)  /* v15, PenSpec   */
#define MUICFG_Calendar_DaysFillPen            (CALENDARPREFSBASE+6)  /* v15, PenSpec   */
#define MUICFG_Calendar_SundaysPen             (CALENDARPREFSBASE+7)  /* v15, PenSpec   */
#define MUICFG_Calendar_AnniversaryPen         (CALENDARPREFSBASE+8)  /* v15, PenSpec   */

#define MUICFG_Calendar_UseDaysNamesBackground (CALENDARPREFSBASE+9)  /* v16, BOOL      */
#define MUICFG_Calendar_UseDaysBackground      (CALENDARPREFSBASE+10) /* v15, BOOL      */
#define MUICFG_Calendar_UseDaysFillPen         (CALENDARPREFSBASE+11) /* v15, BOOL      */

#define MUICFG_Calendar_Font                   (CALENDARPREFSBASE+12) /* v15, STRPTR    */

#define MUICFG_Calendar_WeekStartDay           (CALENDARPREFSBASE+13) /* v15, ULONG     */

#define MUICFG_Calendar_DaysFrame              (CALENDARPREFSBASE+14) /* v15, ULONG     */

/***********************************************************************/
/*
** Defaults
*/

#define MUIDEF_Calendar_DaysNamesTextPen       ((APTR)"m0")
#define MUIDEF_Calendar_DaysNamesBackground    ((APTR)"2:m6")
#define MUIDEF_Calendar_DaysNamesDetailPen     ((APTR)"m5")
#define MUIDEF_Calendar_DaysBackground         ((APTR)"2:m1")
#define MUIDEF_Calendar_DaysBackground_MUI4    NULL
#define MUIDEF_Calendar_DaysTextPen            ((APTR)"m5")
#define MUIDEF_Calendar_DaysShinePen           ((APTR)"m0")
#define MUIDEF_Calendar_DaysFillPen            ((APTR)"m6")
#define MUIDEF_Calendar_SundaysPen             ((APTR)"m7")
#define MUIDEF_Calendar_AnniversaryPen         ((APTR)"m0")

#define MUIDEF_Calendar_UseDaysNamesBackground TRUE
#define MUIDEF_Calendar_UseDaysBackground      FALSE
#define MUIDEF_Calendar_UseDaysFillPen         TRUE

#define MUIDEF_Calendar_Font                   NULL

/* Default for MUICFG_Calendar_WeekStartDay is taken from locale */

#define MUIDEF_Calendar_DaysFrame              "000000"

/***********************************************************************/

#endif /* CALENDARPREFS_H */
