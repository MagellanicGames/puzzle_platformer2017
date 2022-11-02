#include "..\include\DeadDialog.h"

DeadDialog::DeadDialog()
	:
	GameDialog("You Died")
{
	GameDialog::m_options.push_back(GameDialog::RESTART_STR);
	GameDialog::m_options.push_back(GameDialog::RETURNOVERWORLD_STR);
}

DeadDialog::~DeadDialog()
{}