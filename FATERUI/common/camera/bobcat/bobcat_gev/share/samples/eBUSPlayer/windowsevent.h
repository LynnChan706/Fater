// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __QWINDOWSEVENT_H__
#define __QWINDOWSEVENT_H__

#include <QtCore/QEvent>


class QWindowsEvent : public QEvent
{
public:



    QWindowsEvent( QEvent::Type aEventID, uintptr_t wParam = 0, intptr_t lParam = 0 )
        : QEvent( aEventID )
        , mWParam( wParam )
        , mLParam( lParam )
    {
    }

    uintptr_t GetWParam() const { return mWParam; }
    intptr_t GetLParam() const { return mLParam; }

private:

    uintptr_t mWParam;
    intptr_t mLParam;
};


#endif // __QWINDOWSEVENT_H__

