#include "FormSolver.h"

FormSolver::FormSolver(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormSolverClass())
	, model(new QStandardItemModel(this))
{
	ui->setupUi(this);

	QStringList items;
	items << "buoyantBoussinesqPimpleFoam" << "rhoSimpleFoam";
	for (const QString& itemText : items) {
		QStandardItem* item = new QStandardItem(itemText);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable); 
		model->appendRow(item);
	}
	ui->listView->setModel(model);

	connect(ui->pushButton, &QPushButton::clicked, this, &FormSolver::handleButtonGroup1);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &FormSolver::handleButtonGroup1);
	connect(ui->pushButton_3, &QPushButton::clicked, this, &FormSolver::handleButtonGroup2);
	connect(ui->pushButton_4, &QPushButton::clicked, this, &FormSolver::handleButtonGroup2);
	connect(ui->checkBox, &QCheckBox::stateChanged, this, &FormSolver::handleCheckBoxGroup);
}

FormSolver::~FormSolver()
{
	delete ui;
}

void FormSolver::handleButtonGroup1()
{
	QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton == ui->pushButton) {
		if (ui->pushButton->isChecked()) {
			ui->pushButton_4->setChecked(true);
			ui->pushButton_2->setChecked(false);
			ui->pushButton_3->setChecked(false);
			ui->checkBox->setChecked(true);
		}
		else {
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_2) {
		if (ui->pushButton_2->isChecked()) {
			ui->pushButton_3->setChecked(true);
			ui->pushButton->setChecked(false);
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
		else {
			ui->pushButton_3->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::handleButtonGroup2()
{
	QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton == ui->pushButton_3) {
		if (ui->pushButton_3->isChecked()) {
			ui->pushButton_2->setChecked(true);
			ui->pushButton->setChecked(false);
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
		else {
			ui->pushButton_2->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_4) {
		if (ui->pushButton_4->isChecked()) {
			ui->pushButton->setChecked(true);
			ui->pushButton_2->setChecked(false);
			ui->pushButton_3->setChecked(false);
			ui->checkBox->setChecked(true);
		}
		else {
			ui->pushButton->setChecked(false);
			ui->checkBox->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::handleCheckBoxGroup(int state)
{
	if (state == Qt::Checked) {
		ui->pushButton->setChecked(true);
		ui->pushButton_4->setChecked(true);
		ui->pushButton_2->setChecked(false);
		ui->pushButton_3->setChecked(false);
	}
	else {
		ui->pushButton->setChecked(false);
		ui->pushButton_4->setChecked(false);
	}
	updateListView();
}

void FormSolver::updateListView()
{
	if (ui->pushButton->isChecked() && ui->pushButton_4->isChecked() && ui->checkBox->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "rhoSimpleFoam") {
				item->setEnabled(true);
			}
			else {
				item->setEnabled(false);
			}
		}
	}
	else if (ui->pushButton_2->isChecked() && ui->pushButton_3->isChecked() && !ui->checkBox->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "buoyantBoussinesqPimpleFoam") {
				item->setEnabled(true);
			}
			else {
				item->setEnabled(false);
			}
		}
	}
	else {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			item->setEnabled(true);
		}
	}
}

void FormSolver::on_pushButton_5_clicked()
{
	QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
	if (!selectedIndexes.isEmpty()) {
		QString selectedText = selectedIndexes.first().data().toString();
		ui->label_8->setText(selectedText);
	}
}
