#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

//  { 

const int Max_Puzzle_Size = 6;

int Puzzle[Max_Puzzle_Size][Max_Puzzle_Size];
int Xaxis_move[] = { 0,1,-1,0,0 }; // one-based						
int Yaxis_move[] = { 0,0,0,1,-1 };
int PuzzleSize;

//  }	O(1)

struct Parent  // location of blank at parent node
{
	int x_blank;
	int y_blank;
};

struct Current // current location of the blank
{
	int x_blank;
	int y_blank;
};

struct state
{
	Parent parent;
	Current current;
	int Hamming_Distance;
	int Num_of_Moves;
	int Puzzle_State[Max_Puzzle_Size][Max_Puzzle_Size];
};

//  {

struct compare_state
{
	bool operator()(state const& s1, state const& s2)
	{
		return s1.Num_of_Moves + s1.Hamming_Distance > s2.Num_of_Moves + s2.Hamming_Distance;
	}
};

//  }  O(1)

state Current_State;
priority_queue<state, vector<state>, compare_state> States;

//  {

int Calculate_Hamming_Distance()
{
	int count = 0;
	for (int i = 1; i <= PuzzleSize; ++i)
		for (int j = 1; j <= PuzzleSize; ++j)
			if (Puzzle[i][j] != PuzzleSize * (i - 1) + j && Puzzle[i][j] != 0)
				count++;
	return count;
}

//  }  O(S) >> S is the size of the puzzle

//  {

int A_Star()
{
	while (true)			// O(E) >> num of iterations only
	{
		for (int i = 1; i <= PuzzleSize; ++i) // copying the top of the queue into Puzzle			// O(S) 
			for (int j = 1; j <= PuzzleSize; ++j)
				Puzzle[i][j] = Current_State.Puzzle_State[i][j];

		if (Current_State.Hamming_Distance == 0)						// O(1)
			return Current_State.Num_of_Moves;

		for (int i = 1; i <= 4; ++i) // try the 4 options						
		{
			int Next_xblank, Next_yblank;
			Next_xblank = Current_State.current.x_blank + Xaxis_move[i];						// O(1)
			Next_yblank = Current_State.current.y_blank + Yaxis_move[i];
			if (Next_xblank >= 1 && Next_xblank <= PuzzleSize && Next_yblank >= 1 && Next_yblank <= PuzzleSize && (Next_xblank != Current_State.parent.x_blank || Next_yblank != Current_State.parent.y_blank))
			{
				//Do
				swap(Puzzle[Next_yblank][Next_xblank], Puzzle[Current_State.current.y_blank][Current_State.current.x_blank]);					// O(1)

				state Option_State;
				Option_State.Hamming_Distance = Calculate_Hamming_Distance();		// O(S)

				//  {

				Option_State.parent.x_blank = Current_State.current.x_blank;
				Option_State.parent.y_blank = Current_State.current.y_blank;
				Option_State.current.x_blank = Next_xblank;
				Option_State.current.y_blank = Next_yblank;
				Option_State.Num_of_Moves = Current_State.Num_of_Moves + 1;

				//  }  O(1)

				for (int k = 1; k <= PuzzleSize; ++k)								// O(S)
					for (int j = 1; j <= PuzzleSize; ++j)
						Option_State.Puzzle_State[k][j] = Puzzle[k][j];

				States.push(Option_State);						// O(logV)

				//Undo 
				swap(Puzzle[Current_State.current.y_blank][Current_State.current.x_blank], Puzzle[Next_yblank][Next_xblank]);		// O(1)
			}
		}

		Current_State = States.top();						// O(1)
		States.pop();										// O(logV)

	}
}

//  }  O( E log(V) )  the whole function  

void Read_From_File()
{
	string files[] = { "8 Puzzle (1).txt","8 Puzzle (2).txt","8 Puzzle (3).txt",
					   "15 Puzzle - 1.txt","24 Puzzle 1.txt","24 Puzzle 2.txt" };
	for (int i = 0; i < 6; ++i)
	{
		//  {

		cout << "test case " << i + 1 << " : ";
		fstream test_case;
		state intitial_state;
		intitial_state.Num_of_Moves = 0;
		intitial_state.parent.x_blank = -1;
		intitial_state.parent.y_blank = -1;
		test_case.open(files[i]);
		test_case >> PuzzleSize;
		test_case.ignore(); test_case.ignore();

		//  }  O(1)

		//  {

		for (int j = 1; j <= PuzzleSize; ++j)
		{
			for (int k = 1; k <= PuzzleSize; ++k)
			{
				test_case >> Puzzle[j][k];
				intitial_state.Puzzle_State[j][k] = Puzzle[j][k];
				if (Puzzle[j][k] == 0)
				{
					intitial_state.current.y_blank = j;							// O(1)
					intitial_state.current.x_blank = k;
				}
			}
			test_case.ignore();
		}

		//  }  O(S)

		intitial_state.Hamming_Distance = Calculate_Hamming_Distance();		// O(S)
		Current_State = intitial_state;					// O(1)
		cout << A_Star() << endl;					// O( E log(V) )
		while (!States.empty()) // clearing the queue      O( V log(V) )
			States.pop();
	}
}

int main()
{
	Read_From_File();   // O( E log(V) )
	system("pause");
}