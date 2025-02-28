/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormRun.h"
#include <QRegularExpressionValidator> 
#include <QThread>
#include <qtabbar.h>

FormRun::FormRun(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormRunClass())
	, previousIndex(0)
{
	ui->setupUi(this);

	ui->label_6->hide();
	ui->label_12->hide();
	ui->pushButton_2->hide();

	// 设置 lineEdit 只支持整数和小数输入
	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);

	connect(ui->pushButton, &CustomHoverPushButton::cursorEnter, this, &FormRun::cursorEnterPushButton);
	connect(ui->pushButton, &CustomHoverPushButton::cursorLeave, this, &FormRun::cursorLeavePushButton);
	connect(ui->pushButton_2, &CustomHoverPushButton::clicked, this, &FormRun::on_pushButton_clicked_2);
	connect(ui->radioButton_2, &QRadioButton::toggled, this, &FormRun::on_radioButton_2_toggled);
}

FormRun::~FormRun()
{
	delete ui;
}

void FormRun::importParameter()
{
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();

	// 解析controlDict文件
	QString filePath = caseDirPath + "/system/controlDict";
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	QRegularExpression startTimeRegex(R"(startTime\s+(\d*\.?\d+);)");
	QRegularExpression endTimeRegex(R"(endTime\s+(\d*\.?\d+);)");
	QRegularExpression deltaTRegex(R"(deltaT\s+(\d*\.?\d+);)");
	QRegularExpression writeIntervalRegex(R"(writeInterval\s+(\d*\.?\d+);)");

	QRegularExpressionMatch startTimeMatch = startTimeRegex.match(content);
	QRegularExpressionMatch endTimeMatch = endTimeRegex.match(content);
	QRegularExpressionMatch deltaTMatch = deltaTRegex.match(content);
	QRegularExpressionMatch writeIntervalMatch = writeIntervalRegex.match(content);

	if (startTimeMatch.hasMatch()) {
		ui->lineEdit->setText(startTimeMatch.captured(1));
	}
	if (endTimeMatch.hasMatch()) {
		ui->lineEdit_2->setText(endTimeMatch.captured(1));
	}
	if (deltaTMatch.hasMatch()) {
		ui->lineEdit_3->setText(deltaTMatch.captured(1));
	}
	if (writeIntervalMatch.hasMatch()) {
		ui->spinBox_2->setValue(writeIntervalMatch.captured(1).toDouble() / deltaTMatch.captured(1).toDouble());
	}

	//解析decomposeParDict文件
	filePath = caseDirPath + "/system/decomposeParDict";
	file.setFileName(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	in.setDevice(&file);
	content = in.readAll();
	file.close();

	QRegularExpression nProcsRegex(R"(numberOfSubdomains\s+(\d+);)");
	QRegularExpressionMatch nProcsMatch = nProcsRegex.match(content);

	if (nProcsMatch.hasMatch()) {
		ui->spinBox->setValue(nProcsMatch.captured(1).toInt());
	}
}

// 因式分解函数
std::vector<int> factorizeToThree(int nProcs) {
	std::vector<int> bestFactors{ 1, 1, nProcs };
	int minDiff = INT_MAX;

	for (int a = 1; a <= nProcs; ++a) {
		if ((long long)a * a * a > nProcs) break;
		if (nProcs % a != 0) continue;
		int remainder = nProcs / a;

		for (int b = a; b <= remainder; ++b) {
			if ((long long)a * b > nProcs) break;
			if (remainder % b != 0) continue;
			int c = remainder / b;

			// 此时 a * b * c == nProcs
			std::vector<int> current{ a, b, c };
			std::sort(current.begin(), current.end());
			int diff = current[2] - current[0];

			// 更新最优解
			if (diff < minDiff) {
				minDiff = diff;
				bestFactors = current;
			}
		}
	}

	// 将结果按大到小排序
	std::sort(bestFactors.begin(), bestFactors.end(), std::greater<int>());
	return bestFactors;
}

void FormRun::exportParameter()
{
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();

	// 从界面上读取值
	QString startTime = ui->lineEdit->text();
	QString endTime = ui->lineEdit_2->text();
	QString deltaT = ui->lineEdit_3->text();
	QString writeInterval = ui->spinBox_2->text();
	QString nProcs = ui->spinBox->text();

	// 更新controlDict文件
	QString filePath = caseDirPath + "/system/controlDict";
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	// 使用正则表达式替换对应的数值
	content.replace(QRegularExpression(R"(startTime\s+\d*\.?\d+;)"), "startTime       " + startTime + ";");
	content.replace(QRegularExpression(R"(endTime\s+\d*\.?\d+;)"), "endTime         " + endTime + ";");
	content.replace(QRegularExpression(R"(deltaT\s+\d*\.?\d+;)"), "deltaT          " + deltaT + ";");
	content.replace(QRegularExpression(R"(writeInterval\s+\d*\.?\d+;)"), "writeInterval   " + QString::number(writeInterval.toDouble() * deltaT.toDouble()) + ";");

	// 将修改后的内容写回文件
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}
	QTextStream out(&file);
	out << content;
	file.close();

	// 更新decomposeParDict文件
	filePath = caseDirPath + "/system/decomposeParDict";
	file.setFileName(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	in.setDevice(&file);
	content = in.readAll();
	file.close();

	content.replace(QRegularExpression(R"(numberOfSubdomains\s+\d+;)"), "numberOfSubdomains " + nProcs + ";");
	std::vector<int> factors = factorizeToThree(nProcs.toInt());
	QString n = "(" + QString::number(factors[0]) + " " + QString::number(factors[1]) + " " + QString::number(factors[2]) + ")";
	content.replace(QRegularExpression(R"(\bn\s+\(\d+\s+\d+\s+\d+\);)"), "n 			" + n + ";");

	// 将修改后的内容写回文件
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream out2(&file);
	out2 << content;
	file.close();
}

void FormRun::cursorEnterPushButton()
{
	ui->label_6->show();
}

void FormRun::cursorLeavePushButton()
{
	ui->label_6->hide();
}

void FormRun::on_pushButton_clicked()
{
	emit run();
}

void FormRun::on_pushButton_clicked_2()
{
	emit stopRun();
}

void FormRun::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormRun::on_tabWidget_currentChanged(int index)
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
	move->setDuration(300);

	if (previousIndex == 0) {
		if (index == 1) {
			move->setStartValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
			move->setEndValue(QRect(-pixmapWidth, bar->size().height()+10, pixmapWidth*2, pixmapHeight));
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

void FormRun::on_radioButton_2_toggled(bool checked)
{
	ui->spinBox->setEnabled(checked);
}





