#include "ProgressBar.h"
#include <QPainter>

ProgressBar::ProgressBar(QWidget *parent)
	: QWidget(parent)
{
	frame = QPixmap("images/progress_bar_frame.png");
	progress = QPixmap("images/progress_bar_without_frame.png");

	setFixedSize(frame.size());


}

ProgressBar::~ProgressBar()
{}

void ProgressBar::resetProgress()
{
	m_value = 0;
	update();
}

void ProgressBar::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	painter.drawPixmap(rect(), frame);

	float percent = m_value / 100.0f;
	int progressWidth = width() * percent;

	QRectF sourceRect(0, 0, progressWidth, progress.height());
	QRectF progressRect(0, 0, progressWidth, height());

	painter.drawPixmap(progressRect, progress, sourceRect);

	
    //       ______________________                                   
	//     //                      \\
	//     \\                      //
	//	    \\    --__    __--_   //
	//	     \\    |-|    |-|    //
	//	      \\   /_//  /_//   //
	//	       \\              //
	//	        \\    _  _    //
	//	         \\  | \/ |  //
	//	          \\  \\//  //
	//	           \\  \/  //
	//	            \\    //
	//	             \\  //
	//	              \\//

}

void ProgressBar::addProgress(int value)
{
	m_value = std::clamp(m_value + value, 0, 100);

	update();
}
