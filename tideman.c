#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Maximum number of candidates
#define MAX 9

// preferences[i][j] is the number of voters who prefer candidate i to candidate j.
int preferences[MAX][MAX];

// locked[i][j] indicates that there is a directed arrow (graph edge) from i to j.
bool locked[MAX][MAX];

// The structure that holds each pairing (match)
typedef struct
{
    int winner;
    int loser;
} pair;

// The names of the candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cycle(int start, int loser);

int main(int argc, string argv[])
{
    // Command-line control
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Fill in the candidate list
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Reset graphics matrices
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            preferences[i][j] = 0;
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Collect the votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is the voter's i-th preference.
        int ranks[candidate_count];

        // Get all ranked preferences from each voter.
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// 1. TASK: Records the candidate's number on the ranking list established by the voter.
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// 2. TASK: Updates the pairwise preference matrix.
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// 3. TASK: Adds candidates that outperform one another to a series of pairwise matches.
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// 4. TASK: Sorts the matches from largest to smallest based on the winner's margin of victory
// (Selection Sort).
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        int max_idx = i;
        int max_strength = preferences[pairs[i].winner][pairs[i].loser] -
                           preferences[pairs[i].loser][pairs[i].winner];

        for (int j = i + 1; j < pair_count; j++)
        {
            int current_strength = preferences[pairs[j].winner][pairs[j].loser] -
                                   preferences[pairs[j].loser][pairs[j].winner];
            if (current_strength > max_strength)
            {
                max_strength = current_strength;
                max_idx = j;
            }
        }

        // Swap transaction
        pair temp = pairs[i];
        pairs[i] = pairs[max_idx];
        pairs[max_idx] = temp;
    }
}

// Helper Function: Checks whether a cycle has formed in the graph using the DFS algorithm.
bool cycle(int start, int loser)
{
    if (loser == start)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (cycle(start, i))
            {
                return true;
            }
        }
    }
    return false;
}

// 5. TASK: Locks strong matches that do not form a vicious cycle to the graph (locked = true)
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// 6. TASK: Find the leader in the graph that has no incoming arrows—the "source" candidate—and
// declare it the winner.
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_source = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                has_source = false;
                break;
            }
        }
        if (has_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
