#include "..\include\OverworldDialog.h"

OverworldDialog::OverworldDialog()
	:
	GameDialog("Menu")
{
	GameDialog::m_options.push_back(GameDialog::RESUME_STR);
	GameDialog::m_options.push_back(GameDialog::RETURNMAINMENU_STR);
}

OverworldDialog::~OverworldDialog()
{}