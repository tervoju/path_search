#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

// Map class
class Map
{
public:
    const static int mapWidth = 6;
    const static int mapHeight = 5;
    vector<vector<int>> grid = {
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0}};
    vector<vector<int> > heuristic = {
        { 9, 8, 7, 6, 5, 4 },
        { 8, 7, 6, 5, 4, 3 },
        { 7, 6, 5, 4, 3, 2 },
        { 6, 5, 4, 3, 2, 1 },
        { 5, 4, 3, 2, 1, 0 }
    };
};

// Planner class
class Planner : Map
{
public:
    int start[2] = {0, 0};
    int goal[2] = {mapHeight - 1, mapWidth - 1};
    int cost = 1;

    string movements_arrows[4] = {"^", "<", "v", ">"};

    vector<vector<int>> movements{
        {-1, 0}, //up   0
        {0, -1}, //left 1
        {1, 0},  //down 2
        {0, 1}}; //right 3
};

// Template function to print 2D vectors of any type
template <typename T>
void print2DVector(T Vec)
{
    for (int i = 0; i < Vec.size(); ++i)
    {
        for (int j = 0; j < Vec[0].size(); ++j)
        {
            cout << Vec[i][j] << ' ';
        }
        cout << endl;
    }
}

// Search function which generates the expansions
void search(Map map, Planner planner)
{
    // Create a closed 2 array filled with 0s and first element 1
    vector<vector<int>> closed(map.mapHeight, vector<int>(map.mapWidth));
    closed[planner.start[0]][planner.start[1]] = 1;

    // Create a expansion matrix
    vector<vector<int>> expansion(map.mapHeight, vector<int>(map.mapWidth, -1));

    // path movements array
    vector<vector<string>> policy(map.mapHeight, vector<string>(map.mapWidth, "-"));

    // path movearray
    vector<vector<int>> move(map.mapHeight, vector<int>(map.mapWidth, -1));

    expansion[planner.start[0]][planner.start[1]] = 0;
    int expansion_counter = 0;

    // Defined the triplet values
    int x = planner.start[0];
    int y = planner.start[1];
    int g = 0;
    int f = g + map.heuristic[x][y];

    // Store the expansions
    vector<vector<int>> open;
    open.push_back({f, g, x, y});

    // Flags
    bool found = false;
    bool resign = false;

    int x2;
    int y2;

    // Store the expansions
    vector<vector<int>> candidates;

    // While I am still searching for the goal and the problem is solvable
    while (!found && !resign)
    {
        // Resign if no values in the open list and you can't expand anymore
        if (open.size() == 0)
        {
            resign = true;
            cout << "Failed to reach a goal" << endl;
        }
        // Keep expanding
        else
        {
            // Remove triplets from the open list
            sort(open.begin(), open.end());
            reverse(open.begin(), open.end());
            vector<int> next;
            // Stored the poped value into next
            next = open.back();
            open.pop_back();

            x = next[2];
            y = next[3];
            g = next[1];

            expansion[x][y] = expansion_counter++;
            // Check if we reached the goal:
            if (x == planner.goal[0] && y == planner.goal[1])
            {
                found = true;
                //cout << "[" << f << ", " << g << ", " << x << ", " << y << "]" << endl;
            }

            //else expand new elements
            else
            {
                for (int i = 0; i < planner.movements.size(); i++)
                {
                    x2 = x + planner.movements[i][0];
                    y2 = y + planner.movements[i][1];
                    if (x2 >= 0 && x2 < map.grid.size() && y2 >= 0 && y2 < map.grid[0].size())
                    {
                        if (closed[x2][y2] == 0 and map.grid[x2][y2] == 0)
                        {
                            int g2 = g + planner.cost;
                            int f = g2 + map.heuristic[x2][y2];
                            open.push_back({f, g2, x2, y2});
                            closed[x2][y2] = 1;
                            move[x2][y2] = i;
                        }
                    }
                }
            }
        }
    }

    print2DVector(expansion);
    print2DVector(move);
    while (x != planner.start[0] or y != planner.start[1])
    {
        x2 = x - planner.movements[move[x][y]][0];
        y2 = y - planner.movements[move[x][y]][1];
        policy[x2][y2] = planner.movements_arrows[move[x][y]];
        x = x2;
        y = y2;
    }
    policy[planner.goal[0]][planner.goal[1]] = "*";
    print2DVector(policy);
}

int main()
{
    // Instantiate map and planner objects
    Map map;
    Planner planner;

    // Search for the expansions
    search(map, planner);

    return 0;
}
