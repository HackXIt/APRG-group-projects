#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <naxos.h>

using namespace std;
using namespace naxos;

// Function to read the Hidoku grid from a file
void readHidokuFromFile(const string& filename, vector<pair<int, int>>& clues, int& gridSize) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    getline(file, line); // Read the grid size from the first line
    gridSize = stoi(line.substr(0, line.find('x')));

    int cellIndex = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        while (ss >> token) {
            if (token != "-") {
                clues.emplace_back(cellIndex, stoi(token)); // Store clues as (index, value)
            }
            cellIndex++;
        }
    }
    file.close();
}

// Function to write the solved Hidoku grid to a file
void writeHidokuToFile(const string& filename, const NsIntVarArray& cells, int gridSize) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    for (int i = 0; i < cells.size(); ++i) {
        file << (cells[i].value() + 1); // Adjust back from 0-based index to value
        if ((i + 1) % gridSize == 0) {
            file << endl; // Newline at the end of each row
        } else {
            file << " "; // Space between cells in a row
        }
    }
    file.close();
}

// Goal to assign a value to a variable
class NsGoalAssign : public NsGoal {
    NsIntVar& var;
    NsInt val;
    NsGoal* nextGoal;

public:
    NsGoalAssign(NsIntVar& var_, NsInt val_, NsGoal* nextGoal_)
        : var(var_), val(val_), nextGoal(nextGoal_) {}

    NsGoal* GOAL() {
        var.set(val);
        return nextGoal;
    }
};

// Goal to remove a value from a variable's domain
class NsGoalRemoveValue : public NsGoal {
    NsIntVar& var;
    NsInt val;
    NsGoal* nextGoal;

public:
    NsGoalRemoveValue(NsIntVar& var_, NsInt val_, NsGoal* nextGoal_)
        : var(var_), val(val_), nextGoal(nextGoal_) {}

    NsGoal* GOAL() {
        var.remove(val);
        return nextGoal;
    }
};

// Labeling goal for variable assignment
class NsgLabeling : public NsGoal {
public:
    NsIntVarArray vars;

    NsgLabeling(NsIntVarArray& vars_) : vars(vars_) {}

    NsGoal* GOAL() {
        if (vars.empty()) {
            return 0; // Success
        }

        // Select unbound variable with smallest domain
        NsIntVar* var = nullptr;
        NsIndex idx = 0;
        NsUInt min_size = UINT_MAX;
        for (NsIndex i = 0; i < vars.size(); ++i) {
            if (!vars[i].isBound()) {
                if (vars[i].size() < min_size) {
                    min_size = vars[i].size();
                    var = &vars[i];
                    idx = i;
                }
            }
        }

        if (var == nullptr) {
            return 0; // Success
        }

        // Create goals for branching
        NsInt val = var->min();

        NsIntVarArray vars_left = vars;
        vars_left.erase(vars_left.begin() + idx);

        return ALT(
            // Try assigning var = val
            new NsGoalAssign(*var, val, new NsgLabeling(vars_left)),
            // Try removing val from var's domain
            new NsGoalRemoveValue(*var, val, this)
        );
    }
};

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
            return 1;
        }

        string input_file = argv[1];
        string output_file = argv[2];

        vector<pair<int, int>> clues;
        int gridSize;
        readHidokuFromFile(input_file, clues, gridSize);

        // Initialize the problem manager
        NsProblemManager pm;

        int N = gridSize * gridSize;

        // Create variables
        NsIntVarArray cells;
        NsIntVarArray positions;

        // Create cells variables
        for (int i = 0; i < N; ++i) {
            NsIntVar cell(pm, 0, N - 1);
            cells.push_back(cell);
        }

        // Impose inverse constraint
        positions = NsInverse(cells);

        // Impose clues
        for (const auto& clue : clues) {
            int cellIndex = clue.first;
            int value = clue.second;
            cells[cellIndex].set(value - 1); // Adjust for index shift
        }

        // Generate neighbor positions
        vector<vector<int>> neighbor_positions(N);
        for (int p = 0; p < N; ++p) {
            int row = p / gridSize;
            int col = p % gridSize;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <=1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int new_row = row + dr;
                    int new_col = col + dc;
                    if (new_row >=0 && new_row < gridSize && new_col >= 0 && new_col < gridSize) {
                        int neighbor_p = new_row * gridSize + new_col;
                        neighbor_positions[p].push_back(neighbor_p);
                    }
                }
            }
        }

        // Generate allowed pairs
        NsDeque<NsDeque<NsInt>> allowed_pairs;
        for (int p = 0; p < N; ++p) {
            for (int n : neighbor_positions[p]) {
                NsDeque<NsInt> pair;
                pair.push_back(p);
                pair.push_back(n);
                allowed_pairs.push_back(pair);
            }
        }

        // Impose adjacency constraints
        for (int i = 0; i < N - 1; ++i) {
            NsIntVarArray pairVars;
            pairVars.push_back(positions[i]);
            pairVars.push_back(positions[i+1]);

            pm.add(NsSupports(pairVars, allowed_pairs));
        }

        // Add labeling goal
        pm.addGoal(new NsgLabeling(cells));

        // Start the search
        if (pm.nextSolution()) {
            // Output the solution
            writeHidokuToFile(output_file, cells, gridSize);
        } else {
            cout << "No solution found" << endl;
        }

        return 0;

    } catch (exception& exc) {
        cerr << exc.what() << "\n";
        return 1;
    } catch (...) {
        cerr << "Unknown exception\n";
        return 1;
    }
}