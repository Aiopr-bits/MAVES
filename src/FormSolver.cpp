#include "FormSolver.h"

FormSolver::FormSolver(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormSolverClass())
{
	ui->setupUi(this);
}

FormSolver::~FormSolver()
{
	delete ui;
}
