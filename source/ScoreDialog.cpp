#include "..\include\ScoreDialog.h"

ScoreDialog::ScoreDialog(int numMoves, int par, int levelScore, int totalScore)
	:
	m_numMoves(numMoves),
	m_par(par),
	m_levelScore(levelScore),
	m_totalScore(totalScore),	
	m_showContinueText(false),
	GameDialog("Level Complete")
{
	m_parPos = vec(m_size.x * 0.1f, m_size.y * 0.3f);
	m_numMovesPos = vec(m_size.x * 0.1f, m_size.y * 0.4f);
	m_levelScorePos = vec(m_size.x * 0.1f, m_size.y * 0.5f);
	m_bonusScorePos = vec(m_size.x * 0.1f, m_size.y * 0.6f);
	m_totalScorePos = vec(m_size.x * 0.1f, m_size.y * 0.7f);	
	m_continuePos = vec(m_size.x * 0.1f, m_size.y * 0.85f);
}

ScoreDialog::~ScoreDialog()
{

}


void ScoreDialog::f_draw(float dt, sf::RenderWindow & window)
{
	static int titleSize = 30;
	static int textSize = 20;

	GameDialog::f_draw(dt, window);
	SFML_utils::Text::draw((vecf)(m_pos + m_parPos), textSize, "Par: " + std::to_string(ScoreKeeper::par), window);
	SFML_utils::Text::draw((vecf)(m_pos + m_numMovesPos), textSize, "Moves Taken: " + std::to_string(ScoreKeeper::movesTotal), window);
	SFML_utils::Text::draw((vecf)(m_pos + m_levelScorePos), textSize, "Level Score: " + std::to_string(ScoreKeeper::scoreTotal), window);
	SFML_utils::Text::draw((vecf)(m_pos + m_bonusScorePos), textSize, "Bonus Score: " + std::to_string(ScoreKeeper::bonusPoints), window);
	SFML_utils::Text::draw((vecf)(m_pos + m_totalScorePos), textSize, "Total Score: " + std::to_string(ScoreKeeper::scoreTotal + ScoreKeeper::bonusPoints), window);

	if(m_showContinueText)
		SFML_utils::Text::draw((vecf)(m_pos + m_continuePos), textSize, "Press 'space' to continue.", window);
}

void ScoreDialog::f_update(float dt, sf::RenderWindow & window)
{

}