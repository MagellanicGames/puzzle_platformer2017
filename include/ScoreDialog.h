#pragma once
#include "GameDialog.h"

class ScoreDialog : public GameDialog
{
public:

	ScoreDialog(int numMoves = 0, int par = 0, int levelScore = 0, int totalScore = 0);
	~ScoreDialog();

	void f_draw(float dt, sf::RenderWindow & window) override;
	virtual void f_update(float dt, sf::RenderWindow & window)override;
	void f_setShowContinueText(bool b) { m_showContinueText = b; }

private:
		
	int					m_numMoves;
	int					m_par;
	int					m_levelScore;
	int					m_totalScore;
	
	vec					m_parPos;
	vec					m_numMovesPos;
	vec					m_levelScorePos;
	vec					m_totalScorePos;
	vec					m_bonusScorePos;
	vec					m_continuePos;
	bool				m_showContinueText;

};