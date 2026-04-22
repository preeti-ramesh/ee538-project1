#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
void read_opinions(string filename);
void read_edges(string filename);

int total_nodes = 0;
/****************************************************************/

vector<int> opinions;
vector<vector<int>> incoming_neighbors;
vector<vector<int>> edge_list;

// Build adjacency list
void build_adj_list()
{
    incoming_neighbors = vector<vector<int>>(total_nodes);

    for (int i = 0; i < edge_list.size(); i++)
    {
        int source = edge_list[i][0];
        int target = edge_list[i][1];

        incoming_neighbors[target].push_back(source);
    }
}

double calculate_fraction_of_ones()
{
    int count = 0;

    for (int i = 0; i < total_nodes; i++)
    {
        if (opinions[i] == 1)
            count++;
    }

    return (double)count / total_nodes;
}

int get_majority_friend_opinions(int node)
{
    int count0 = 0;
    int count1 = 0;

    // Only check actual neighbors
    for (int i = 0; i < incoming_neighbors[node].size(); i++)
    {
        int neighbor = incoming_neighbors[node][i];

        if (opinions[neighbor] == 0)
            count0++;
        else
            count1++;
    }

    if (count1 > count0)
        return 1;
    return 0; // tie → 0
}

bool update_opinions()
{
    vector<int> new_opinions = opinions;
    bool changed = false;

    for (int i = 0; i < total_nodes; i++)
    {
        int majority = get_majority_friend_opinions(i);

        if (majority != opinions[i])
        {
            new_opinions[i] = majority;
            changed = true;
        }
    }

    opinions = new_opinions;
    return changed;
}

int main()
{
    read_opinions("opinions.txt");
    read_edges("edge_list.txt");

    build_adj_list();

    cout << "Total nodes: " << total_nodes << endl;

    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;

    cout << "Iteration " << iteration << ": fraction of 1's = "
         << calculate_fraction_of_ones() << endl;

    while (iteration < max_iterations && opinions_changed)
    {
        opinions_changed = update_opinions();
        iteration++;

        cout << "Iteration " << iteration << ": fraction of 1's = "
             << calculate_fraction_of_ones() << endl;
    }

    double final_fraction = calculate_fraction_of_ones();

    cout << "Iteration " << iteration << ": fraction of 1's = "
         << final_fraction << endl;

    if (final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if (final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;

    return 0;
}

/*********** File Reading ***********/
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;

    while (file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if (id >= total_nodes)
            total_nodes = id + 1;
    }

    file.close();
}

void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;

    while (file >> source >> target)
    {
        edge_list.push_back({source, target});

        if (source >= total_nodes)
            total_nodes = source + 1;
        if (target >= total_nodes)
            total_nodes = target + 1;
    }

    file.close();
}
