/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormBoundaryConditionsTabWidget.h"

FormBoundaryConditionsTabWidget::FormBoundaryConditionsTabWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsTabWidgetClass())
	, previousIndex(0)
{
	ui->setupUi(this);

	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);
	ui->lineEdit_7->setValidator(validator);
	ui->lineEdit_8->setValidator(validator);
	ui->lineEdit_9->setValidator(validator);
	ui->lineEdit_10->setValidator(validator);

	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
	connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_2CurrentIndexChanged(int)));
	connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_3CurrentIndexChanged(int)));
	connect(ui->comboBox_4, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_4CurrentIndexChanged(int)));
	connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_5CurrentIndexChanged(int)));
	connect(ui->comboBox_6, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_6CurrentIndexChanged(int)));
	connect(ui->comboBox_7, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_7CurrentIndexChanged(int)));

	onComboBoxCurrentIndexChanged(0);
	onComboBox_2CurrentIndexChanged(0);
	onComboBox_3CurrentIndexChanged(0);
	onComboBox_4CurrentIndexChanged(0);
	onComboBox_5CurrentIndexChanged(0);
	onComboBox_6CurrentIndexChanged(0);
	onComboBox_7CurrentIndexChanged(0);
}

FormBoundaryConditionsTabWidget::~FormBoundaryConditionsTabWidget()
{
	delete ui;
}

void FormBoundaryConditionsTabWidget::setLayoutVisibility(QLayout* layout, bool visible)
{
	for (int i = 0; i < layout->count(); ++i) {
		QLayoutItem* item = layout->itemAt(i);
		if (item != nullptr) {
			QWidget* widget = item->widget();
			if (widget != nullptr) {
				widget->setVisible(visible);
			}
			else {
				QLayout* subLayout = item->layout();
				if (subLayout != nullptr) {
					setLayoutVisibility(subLayout, visible);
				}
			}
		}
	}
}

void FormBoundaryConditionsTabWidget::updateLayoutVisibility(QGridLayout* layout, bool visible)
{
	for (int i = 0; i < layout->columnCount(); ++i) {
		QLayoutItem* item = layout->itemAtPosition(1, i);
		if (item != nullptr) {
			QWidget* widget = item->widget();
			if (widget != nullptr) {
				widget->setVisible(visible);
			}
			else {
				QLayout* subLayout = item->layout();
				if (subLayout != nullptr) {
					setLayoutVisibility(subLayout, visible);
				}
			}
		}
	}
}

void FormBoundaryConditionsTabWidget::onComboBoxCurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox->currentText();
	updateLayoutVisibility(ui->gridLayout, boundaryConditionType == "fixedValue");
}

void FormBoundaryConditionsTabWidget::onComboBox_2CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_2->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "heatFluxTemperature");
	updateLayoutVisibility(ui->gridLayout_2, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_3CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_3->currentText();
	bool visible = (boundaryConditionType == "fixedValue");
	updateLayoutVisibility(ui->gridLayout_8, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_4CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_4->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "inletOutlet" || boundaryConditionType == "kqRWallFunction");
	updateLayoutVisibility(ui->gridLayout_4, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_5CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_5->currentText();
	bool visible = (boundaryConditionType == "calculated" || boundaryConditionType == "nutkWallFunction");
	updateLayoutVisibility(ui->gridLayout_5, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_6CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_6->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "inletOutlet" || boundaryConditionType == "omegaWallFunction");
	updateLayoutVisibility(ui->gridLayout_6, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_7CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_7->currentText();
	bool visible = (boundaryConditionType == "calculated" || boundaryConditionType == "alphatWallFunction");
	updateLayoutVisibility(ui->gridLayout_7, visible);
}

void FormBoundaryConditionsTabWidget::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormBoundaryConditionsTabWidget::on_tabWidget_currentChanged(int index)
{
	QWidget* widget0 = ui->tabWidget->widget(0);
	QWidget* widget1 = ui->tabWidget->widget(1);
	QWidget* widget2 = ui->tabWidget->widget(2);
	QPixmap pixmap0 = QPixmap::grabWidget(widget0);
	QPixmap pixmap1 = QPixmap::grabWidget(widget1);
	QPixmap pixmap2 = QPixmap::grabWidget(widget2);

	int imageWidth = pixmap0.width() + pixmap1.width() + pixmap2.width();
	int imageHeight = pixmap0.height();
	QImage image(imageWidth, imageHeight, QImage::Format_ARGB32);
	image.fill(QColor(Qt::black));

	QPainter p;
	p.begin(&image);
	QBrush brush(QColor(255, 255, 0), Qt::Dense4Pattern);
	p.setBrush(brush);
	QPen pen;
	pen.setColor(QColor(Qt::red));
	p.setPen(pen);
	p.drawPixmap(0, 0, pixmap0);
	p.drawPixmap(pixmap0.width(), 0, pixmap1);
	p.drawPixmap(pixmap0.width() + pixmap1.width(), 0, pixmap2);
	p.end();

	QLabel* animationWidget = new QLabel(ui->tabWidget);
	animationWidget->setPixmap(QPixmap::fromImage(image));
	QTabBar* bar = ui->tabWidget->tabBar();
	QSize size1 = bar->size();
	QSize size2 = ui->tabWidget->size();
	int pixmapWidth = pixmap0.width();
	int pixmapHeight = pixmap0.height();

	animationWidget->show();
	animationWidget->raise();
	QPropertyAnimation* move = new QPropertyAnimation(animationWidget, "geometry");
	move->setDuration(200);

	if (previousIndex == 0) {
		if (index == 1) {
			move->setStartValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
			move->setEndValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
		}
		else if (index == 2) {
			move->setStartValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
			move->setEndValue(QRect(-2 * pixmapWidth, bar->size().height() + 10, pixmapWidth * 3, pixmapHeight));
		}
	}
	else if (previousIndex == 1) {
		if (index == 0) {
			move->setStartValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
			move->setEndValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
		}
		else if (index == 2) {
			move->setStartValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
			move->setEndValue(QRect(-2 * pixmapWidth, bar->size().height() + 10, pixmapWidth * 3, pixmapHeight));
		}
	}
	else if (previousIndex == 2) {
		if (index == 0) {
			move->setStartValue(QRect(-2 * pixmapWidth, bar->size().height() + 10, pixmapWidth * 3, pixmapHeight));
			move->setEndValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
		}
		else if (index == 1) {
			move->setStartValue(QRect(-2 * pixmapWidth, bar->size().height() + 10, pixmapWidth * 3, pixmapHeight));
			move->setEndValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
		}
	}

	move->start();
	connect(move, &QAbstractAnimation::finished, this, [=]() {
		delete animationWidget;
		delete move;
		});

	previousIndex = index;
}
