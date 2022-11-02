#include "..\include\IngameOptionsDialog.h"

IngameOptionsDialog::IngameOptionsDialog()
	:
	GameDialog("Paused")
{	
	GameDialog::m_options.push_back(GameDialog::RESUME_STR);
	GameDialog::m_options.push_back(GameDialog::RESTART_STR);
	GameDialog::m_options.push_back(GameDialog::RETURNOVERWORLD_STR);
}

IngameOptionsDialog::~IngameOptionsDialog()
{}