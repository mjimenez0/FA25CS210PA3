//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int> > &maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1; // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int, int> chooseBoundaryCell(const vector<vector<int> > &maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {
            // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {
            // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {
            // left column
            r = rand() % N;
            c = 0;
        } else {
            // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int> > &maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c) {
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int, int> exitcell,
               const vector<vector<int> > &parent_r,
               const vector<vector<int> > &parent_c,
               int ent_r, int ent_c) {
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int, int> > path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
bool dfs(int r, int c,
         const vector<vector<int> > &maze,
         vector<vector<bool> > &visited,
         vector<vector<int> > &parent_r,
         vector<vector<int> > &parent_c,
         int exit_r, int exit_c) {
    // 1. checks if out of bounds
    // colloms or rows cannot be less than 0
    // N & M the bounds of the maze any number greater than them cannot happen
    // one of the 4 conditions must be met in order to be out of bounds

    // fixed* need to add the maze.size and maze[0] to prevent the inputs to go out of bounds

    if (r < 0 || c < 0 || r >= maze.size() || c >= maze[0].size()) {
        return false;
    }

    // 2. checks if bounds of maze walls , just like the prompt
    if (maze[r][c] == 1) {
        return false;
    }

    // 3.  checks visted array , that way i can visit the same one twice cuasing an infinite loop
    if (visited[r][c] == true) {
        return false;
    }

    // 4. check vist ?  after looking into it, it  cant be in the if statement as it would make it reduntent
    // did watch a youtube vid and had chat explain it to my on way this is needed outside
    visited[r][c] = true;

    // 3. check exist , so it stops once reached
    if (r == exit_r && c == exit_c) {
        return true;
    }


    // 6. exploring neighbors dr and dc
    for (int i = 0; i < 4; i++) {
        // recurssion
        int new_r = r + dr[i]; // need to add the dr[i] doesnt work with just r alone, was just doing r alone
        int new_c = c + dc[i];

        // similar to the one in main, this is the update path using the for loop, tried using the original one
        // but it made the code explode
        bool found = dfs(new_r, new_c, maze, visited, parent_r, parent_c, exit_r, exit_c);
        if (found) {
            parent_r[new_r][new_c] = r; // fixed* needs to be = to both r and c
            parent_c[new_r][new_c] = c; // create maze new bounds when the recursion.
            return true;
        }
    }

    // recurrsion isnt working ?
    // fixed* the parent_r/c needs to be inside the if statement in order for it to work
    //* the bool found needs to have the new_r/c in it


    return false;
}

    // ----------------------------------------------------------
    // MAIN PROGRAM (students add DFS calls and logic)
    // ----------------------------------------------------------
    int main(){

        int N, M;

        cout << "Enter maze dimensions N M: ";
        cin >> N >> M;

        vector<vector<int> > maze(N, vector<int>(M));
        generateMaze(maze, N, M);

        // Pick entrance and exit
        pair<int, int> entrance = chooseBoundaryCell(maze);
        pair<int, int> exitcell = chooseBoundaryCell(maze);

        while (exitcell == entrance) {
            exitcell = chooseBoundaryCell(maze);
        }

        int ent_r = entrance.first;
        int ent_c = entrance.second;
        int exit_r = exitcell.first;
        int exit_c = exitcell.second;

        // Display the maze
        printMaze(maze, ent_r, ent_c, exit_r, exit_c);

        // Students must use these
        vector<vector<bool> > visited(N, vector<bool>(M, false));
        vector<vector<int> > parent_r(N, vector<int>(M, -1));
        vector<vector<int> > parent_c(N, vector<int>(M, -1));

        // ------------------------------------------------------
        // STUDENT WORK:
        // Call your DFS, track visited, and fill parent_r and parent_c
        // ------------------------------------------------------
        bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

        // ------------------------------------------------------
        // STUDENT WORK:
        // If found, print the path
        // ------------------------------------------------------
        if (found) {
            printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
        } else {
            cout << "\nNo path exists.\n";
        }

        return 0;
    }
