#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <stack>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 4
#define WHITE_KNIGHT 1
#define BLACK_KNIGHT 2
#define BLANCK 0
#define xy(x,y) y + x * BOARD_WIDTH

using namespace std;

template <typename T>
/* overloading << to print the board */
ostream& operator<<(ostream& os, const vector<T>& v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (i > 0 && i % BOARD_WIDTH == 0) os << "\n";

		os << v[i];
		if (i != v.size() - 1)
			os << " ";
	}
	os << "\n\n";
	return os;
}

/* Finding the path between the start point and the end point using DFS Algorithm */
vector<int> find_path(int start_pos, int end_pos, vector<vector<int>>& adj_list, vector<bool>& occupied)
{
	vector<bool> visited(occupied.size());
	stack<int> stk;
	vector<int> path;
	vector<int> parent_of(occupied.size());

	int parent_pos = start_pos;
	stk.push(start_pos);
	parent_of[start_pos] = -1;

	while (!visited[end_pos] && !stk.empty())
	{
		visited[parent_pos] = true;
		stk.pop();
		for (int adj_pos : adj_list[parent_pos])
		{
			if (!visited[adj_pos] && !occupied[adj_pos])
			{
				parent_of[adj_pos] = parent_pos;
				stk.push(adj_pos);
			}

		}
		/* If There's no path then we will move the knight in the middle of the board to another place */
		if (stk.empty())
			return {};
		else
			parent_pos = stk.top();
	}

	/* Constructing the path */
	int pos = end_pos;
	while (pos != -1)
	{
		path.push_back(pos);
		pos = parent_of[pos];
	}

	return path;
}
/* Applies the movements of the knight step by step */
void move_knight(int start_pos, int end_pos, vector<vector<int>>& adj_list, vector<bool>& occupied, vector<int>& board, vector<int> path)
{
	for (int i = path.size() - 1; i > 0; i--)
	{
		swap(board[path[i]], board[path[i - 1]]);
		cout << board;
	}
	swap(occupied[start_pos], occupied[end_pos]);

}
int minMoves(vector<vector<int>>& adj_list, vector<bool>& occupied, vector<int>& board)
{
	int number_of_moves = 0;

	/* we must make an empty corner first to solve the puzzel */
	vector<int> path_top_left_knight = find_path(0, 6, adj_list, occupied);
	move_knight(0, 6, adj_list, occupied, board, path_top_left_knight);

	/* adding the number of moves made by the left top black knight */
	number_of_moves += path_top_left_knight.size() - 1;

	/* Partition the top of the board */
	vector<int> board_top(board.begin(), board.begin() + BOARD_WIDTH);

	/* Partition the bottom of the board */
	vector<int> board_bottom(board.begin() + (BOARD_HEIGHT - 1) * BOARD_WIDTH, board.end());

	/* true if the bottom of the board contain only black knights */
	bool all_bottom_board_are_black = all_of(board_bottom.begin(), board_bottom.end(), [](int kngiht) {return kngiht == BLACK_KNIGHT; });

	/* true if the bottom of the board contain only white knights */
	bool all_top_board_are_white = all_of(board_top.begin(), board_top.end(), [](int kngiht) {return kngiht == WHITE_KNIGHT; });

	/* true of no movement is done in the iteration */
	bool no_move;
	/* Records the path with the least steps */
	tuple<int, int, vector<int>> best_move;

	/* Records path in one iteration */
	vector<int> path;
	while (!all_bottom_board_are_black && !all_top_board_are_white)
	{
		no_move = true;
		/* Initializing best path */
		get<0>(best_move) = -1; // start position
		get<1>(best_move) = -1;	// end position
		get<2>(best_move) = {};	// path

		/* finds the blanck position in the top part of the board */
		auto top_itr = find(board_top.begin(), board_top.end(), BLANCK);
		int top_blanck_pos = distance(board_top.begin(), top_itr);

		if (top_itr != board_top.end())
		{
			for (int i = 0; i < board_bottom.size(); i++)
			{
				if (board_bottom[i] == WHITE_KNIGHT)
				{
					path = find_path(xy(3, i), xy(0, top_blanck_pos), adj_list, occupied);

					/* choosing the path with the least number of steps */
					if (path.size() - 1 < get<0>(best_move))
					{
						get<0>(best_move) = xy(3, i);
						get<1>(best_move) = xy(0, top_blanck_pos);
						get<2>(best_move) = path;
					}
				}
			}
		}
		/* moving the knights on the path */
		if (get<1>(best_move) != -1)
		{
			move_knight(get<0>(best_move), get<1>(best_move), adj_list, occupied, board, get<2>(best_move));
			no_move = false;
			/* incrementing the number of moves */
			number_of_moves += get<2>(best_move).size() - 1;
		}


		/* Reinetializing the best path */
		get<0>(best_move) = -1;
		get<1>(best_move) = -1;
		get<2>(best_move) = {};

		board_top = vector<int>(board.begin(), board.begin() + BOARD_WIDTH);
		board_bottom = vector<int>(board.begin() + (BOARD_HEIGHT - 1) * BOARD_WIDTH, board.end());

		auto bottom_itr = find(board_bottom.begin(), board_bottom.end(), BLANCK);
		int bottom_blanck_pos = distance(board_bottom.begin(), bottom_itr);
		if (bottom_itr != board_bottom.end())
		{
			for (int i = board_top.size() - 1; i >= 0; i--)
			{
				if (board_top[i] == BLACK_KNIGHT)
				{
					path = find_path(xy(0, i), xy(3, bottom_blanck_pos), adj_list, occupied);

					/* choosing the path with the least number of steps */
					if (path.size() - 1 < get<0>(best_move))
					{
						get<0>(best_move) = xy(0, i);
						get<1>(best_move) = xy(3, bottom_blanck_pos);
						get<2>(best_move) = path;


					}
				}
			}
		}

		/* moving the knights on the path */
		if (get<1>(best_move) != -1)
		{
			move_knight(get<0>(best_move), get<1>(best_move), adj_list, occupied, board, get<2>(best_move));
			no_move = false;
			number_of_moves += get<2>(best_move).size() - 1;
		}
		/*
		*	If there's no move is done by either the black knights or the white
		*	means that the black knight in the middle portion is blocking the moves of the other knights
		*	we will move the knight in the middle of the board
		*/
		if (no_move)
		{
			swap(board[6], board[5]);
			swap(occupied[6], occupied[5]);
			number_of_moves += 1;
		}

		/* Reinitializing variables for the next iteration */
		board_top = vector<int>(board.begin(), board.begin() + BOARD_WIDTH);
		board_bottom = vector<int>(board.begin() + (BOARD_HEIGHT - 1) * BOARD_WIDTH, board.end());

		all_bottom_board_are_black = all_of(board_bottom.begin(), board_bottom.end(), [](int kngiht) {return kngiht == BLACK_KNIGHT; });
		all_top_board_are_white = all_of(board_top.begin(), board_top.end(), [](int kngiht) {return kngiht == WHITE_KNIGHT; });
	}

	/* partition the middle of the board */
	vector<int> board_middle(board.begin() + BOARD_WIDTH, board.end() - BOARD_WIDTH);

	/* find the black knight we moved in the first time */
	auto middle_itr = find(board_middle.begin(), board_middle.end(), BLACK_KNIGHT);
	int middle_knight_pos = distance(board_middle.begin(), middle_itr);

	/* find the blanck position in the bottom row */
	auto bottom_itr = find(board_bottom.begin(), board_bottom.end(), BLANCK);
	int bottom_blanck_pos = distance(board_bottom.begin(), bottom_itr);

	/* moving the black knight found in the middle of te board into bottom row */
	path = find_path(middle_knight_pos + 3, xy(3, bottom_blanck_pos), adj_list, occupied);
	move_knight(middle_knight_pos + 3, xy(3, bottom_blanck_pos), adj_list, occupied, board, path);

	/* adding the number of moves done by the middle black knight */
	number_of_moves += path.size() - 1;


	return number_of_moves;

}

int main(void)
{
	/* 4 x 3 GRID */
	vector<int> board(12);
	vector<bool> occupied(12);

	/* Black Knights = 2 */
	board[xy(0, 0)] = BLACK_KNIGHT;
	board[xy(0, 1)] = BLACK_KNIGHT;
	board[xy(0, 2)] = BLACK_KNIGHT;

	/* marking the Black Knights positions as occupied */
	occupied[xy(0, 0)] = true;
	occupied[xy(0, 1)] = true;
	occupied[xy(0, 2)] = true;

	/* White Knights = 1 */
	board[xy(3, 0)] = WHITE_KNIGHT;
	board[xy(3, 1)] = WHITE_KNIGHT;
	board[xy(3, 2)] = WHITE_KNIGHT;

	/* marking the White Knights positions as occupied */
	occupied[xy(3, 0)] = true;
	occupied[xy(3, 1)] = true;
	occupied[xy(3, 2)] = true;

	/* Adjacncy List for all possible moves in the 3X4 grid */
	vector<vector<int>> adj_list = {
		{7,5},
		{6,8},
		{3,7},
		{2,10,8},
		{11,9},
		{0,6,10},
		{1,11,5},
		{0,2},
		{1,3,9},
		{8,4},
		{5,3},
		{6,4}
	};

	cout << board;
	cout << "MINIMUM NUMBER OF MOVES = " << minMoves(adj_list, occupied, board);
}