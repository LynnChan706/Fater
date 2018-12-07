// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "aboutbox.h"


///
/// \brief Help menu about to show. Update what needs to be updated.
///

void eBUSPlayer::OnHelpMenuAboutToShow()
{
}



///
/// \brief Menu action handler
///

void eBUSPlayer::OnHelpAboutAction()
{
    AboutBox lHelpDlg( this );
    lHelpDlg.exec();
}


