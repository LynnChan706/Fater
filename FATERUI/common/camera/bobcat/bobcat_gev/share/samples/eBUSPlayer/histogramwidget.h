// *****************************************************************************
//
//     Copyright (c) 2015, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************


#ifndef __HISTOGRAMWIDGET_H__
#define __HISTOGRAMWIDGET_H__

#include <QWidget>
#include <QTimer>
 

class PvRangeFilter;


class HistogramWidget : public QWidget
{
    Q_OBJECT

public:

    HistogramWidget( PvRangeFilter *aRangeFilter, QWidget *aParent );
    virtual ~HistogramWidget();

public slots:

	void OnTimer();

protected:

    void CreateLayout();
    void DrawHistogram();

    void paintEvent( QPaintEvent *aEvent );

private:

    PvRangeFilter *mRangeFilter;

	QTimer *mTimer;
    
};



#endif // __HISTOGRAMWIDGET_H__

