//
// evaluate.cpp
// by Jonathan Kreuzer
// 
// Computes an eval score for a Board.

#include "engine.h"
#include "kr_db.h"

// return eval relative to board.sideToMove
int Board::EvaluateBoard(int ply, SearchThreadData& search, const EvalNetInfo& netInfo, int depth) const
{
	// Game is over?        
	if ((numPieces[WHITE] == 0 && sideToMove == WHITE) || (numPieces[BLACK] == 0 && sideToMove == BLACK)) {
		return -WinScore(ply);
	}

	// Exact database value?
	if (engine.dbInfo.type == dbType::EXACT_VALUES && engine.dbInfo.InDatabase(*this))
	{
		int value = QueryEdsDatabase(*this, ply);

		if (value != INVALID_DB_VALUE)
		{
			// If we get an exact score from the database we want to return right away
			search.displayInfo.databaseNodes++;
			return (sideToMove == WHITE) ? value : -value;
		}
	}

	int eval = 0;

	if (engine.dbInfo.type == dbType::KR_WIN_LOSS_DRAW && engine.dbInfo.InDatabase(*this)) {
		EGDB_BITBOARD bb;

		gui_to_kr(Bitboards, bb);
		int result = engine.dbInfo.kr_wld->lookup(engine.dbInfo.kr_wld, &bb, gui_to_kr_color(sideToMove), depth <= 3);
		if (result == EGDB_WIN) {
			search.displayInfo.databaseNodes++;
			return(to_rel_score(FinishingEval(), sideToMove) + 400);
		}
		else if (result == EGDB_LOSS) {
			search.displayInfo.databaseNodes++;
			return(to_rel_score(FinishingEval(), sideToMove) - 400);
		}
		else if (result == EGDB_DRAW) {
			search.displayInfo.databaseNodes++;
			return(0);
		}
	}

	// Probe the W/L/D bitbase
	if (engine.dbInfo.type == dbType::WIN_LOSS_DRAW && engine.dbInfo.InDatabase(*this))
	{
		// Use a heuristic eval to help finish off won games
		eval = FinishingEval();

		int Result = QueryGuiDatabase(*this);
		if (Result <= 2) {
			search.displayInfo.databaseNodes++;
			if (Result == dbResult::WHITEWIN) eval += 400;
			if (Result == dbResult::BLACKWIN) eval -= 400;
			if (Result == dbResult::DRAW) return 0;
		}
	}
	else if (Bitboards.GetCheckers() == 0)
	{
		eval = FinishingEval(); // All kings also uses heuristic eval
	}
	else
	{
		// NEURAL NET EVAL
		assert(netInfo.firstLayerValues && netInfo.netIdx >= 0);
		eval = engine.evalNets[netInfo.netIdx]->GetNNEvalIncremental(netInfo.firstLayerValues, search.nnValues);
		eval = -SoftClamp(eval / 3, 400, 800); // move it into a better range with rest of evaluation

		// surely winning material advantage?
		if (numPieces[WHITE] >= numPieces[BLACK] + 2)
			eval += (numPieces[BLACK] == 1) ? 150 : 50;
		if (numPieces[BLACK] >= numPieces[WHITE] + 2)
			eval -= (numPieces[WHITE] == 1) ? 150 : 50;
	}

	// return sideToMove relative eval
	return (sideToMove == WHITE) ? eval : -eval;
}

// For 1. won positions in database 2. positions with all kings to help finish the game. 
// (TODO? Could train a small net to handle this using some distance to win metric in targetVal.)
// (TODO? generalize to use for any lop-sided position that should be easy win.)
int Board::FinishingEval() const
{
	const uint32_t WK = Bitboards.K & Bitboards.P[WHITE];
	const uint32_t BK = Bitboards.K & Bitboards.P[BLACK];
	const int KingCount[2] = { BitCount(BK), BitCount(WK) };

	int eval = 0;
	eval += (KingCount[WHITE] - KingCount[BLACK]) * 25;
	eval += (numPieces[WHITE] - numPieces[BLACK]) * 75;
	eval += (BitCount(WK & SINGLE_EDGE) - BitCount(BK & SINGLE_EDGE)) * -10;// encourage pushing kings to edges (except 2 corner)
	eval += (BitCount(WK & CENTER_8) - BitCount(BK & CENTER_8)) * 4; // even better to be in center
	if (TotalPieces() < 12 && Bitboards.GetCheckers() == 0)
	{
		eval += (numPieces[WHITE] - numPieces[BLACK]) * (240 - TotalPieces() * 20); // encourage trading down
	}
	if (numPieces[WHITE] > numPieces[BLACK])
	{
		if (KingCount[WHITE] == numPieces[WHITE]) eval += 60; // An extra king is usually a win I think?
		if (numPieces[WHITE] >= numPieces[BLACK] + 2) eval += (numPieces[BLACK] == 1) ? 100 : 50; 	// surely winning material advantage?
		if (BK & DOUBLE_CORNER) eval -= 12; // encourage losing side to use double corner
	}
	if (numPieces[BLACK] > numPieces[WHITE])
	{
		if (KingCount[BLACK] == numPieces[BLACK]) eval -= 60;
		if (numPieces[BLACK] >= numPieces[WHITE] + 2) eval -= (numPieces[WHITE] == 1) ? 100 : 50;
		if (WK & DOUBLE_CORNER) eval += 12;
	}

	return eval;
}


int Board::nonincremental_nn_eval(SearchThreadData& search) const
{
	EvalNetInfo &netInfo = engine.searchThreadData.stack->netInfo; 
	netInfo.netIdx = (int)CheckersNet::GetGamePhase(*this);
	assert(netInfo.firstLayerValues && netInfo.netIdx >= 0);
	engine.evalNets[netInfo.netIdx]->ComputeFirstLayerValues(*this, engine.searchThreadData.nnValues, netInfo.firstLayerValues);
	int eval = engine.evalNets[netInfo.netIdx]->GetNNEvalIncremental(netInfo.firstLayerValues, search.nnValues);
	eval = -SoftClamp(eval / 3, 400, 800); // move it into a better range with rest of evaluation

	 // return sideToMove relative eval
	return(sideToMove == WHITE) ? eval : -eval;
}

