/* QtMoko edit-focus compatibility for external Qt 4 builds. */
#ifndef QTMOKO_EDITFOCUS_COMPAT_H
#define QTMOKO_EDITFOCUS_COMPAT_H

#include <QWidget>
#include <QApplication>
#include <QVariant>

/* Qt built without keypad navigation has no QWidget edit-focus API.
 * Keep QtMoko's navigation/editing state as a widget property. */
static inline bool qtmokoHasEditFocus(const QWidget *widget)
{
    return widget && widget->property("qtopiaEditFocus").toBool();
}

static inline void qtmokoSetEditFocus(QWidget *widget, bool enabled)
{
    if (widget)
        widget->setProperty("qtopiaEditFocus", enabled);
}

static inline bool qtmokoKeypadNavigationEnabled()
{
    return qApp && qApp->property("qtopiaKeypadNavigation").toBool();
}

#endif
