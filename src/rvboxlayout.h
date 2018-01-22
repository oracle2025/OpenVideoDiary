#ifndef _RVBOXLAYOUT_H_
#define _RVBOXLAYOUT_H_


#include <QVBoxLayout>

class RVBoxLayout : public QVBoxLayout
{
	public:
		RVBoxLayout() : QVBoxLayout() {}
		RVBoxLayout(QWidget *parent) : QVBoxLayout(parent) {}
		static RVBoxLayout* create() { return new RVBoxLayout; } 
		RVBoxLayout* addStretch(int stretch = 0) {
			QVBoxLayout::addStretch(stretch);
			return this;
		}
		RVBoxLayout* setStretch(int index, int stretch) {
			QVBoxLayout::setStretch(index, stretch);
			return this;
		}
		RVBoxLayout* addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment()) {
			QVBoxLayout::addWidget(widget, stretch, alignment);
			return this;
		}
};

#endif /* _RVBOXLAYOUT_H_ */
