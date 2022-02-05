#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
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

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
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

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
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

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int p = 0;
    for(int i = 0; i < candidate_count; i++)
    {
        if(strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            p = 1;
        }
    }
    if(p == 1)
    return true;
    else
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        int x = ranks[i];
        for(int j = 0; j < candidate_count; j++)
        {
            if(i < j)
            {
                int y = ranks[j];
                ++preferences[x][y];
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < candidate_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                if(preferences[i][j] != preferences[j][i])
                {
                    if(preferences[i][j] < preferences[j][i])
                    {
                        pairs[pair_count].winner = j;
                        pairs[pair_count].loser = i;
                    }
                    else
                    {
                        pairs[pair_count].winner = i;
                        pairs[pair_count].loser = j;
                    }
                    ++pair_count;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int X[pair_count], num, val1, val2;
    for(int i = 0; i < pair_count; i++)
    {
        X[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }
    for(int j = 0; j < pair_count; j++)
    {
        for(int k = j; k < pair_count - 1; k++)
        {
            if(X[j] < X[k + 1])
            {
                num = X[j];
                val1 = pairs[j].winner;
                val2 = pairs[j].loser;
                X[j] = X[k + 1];
                X[k + 1] = num;

                pairs[j].winner = pairs[k + 1].winner;
                pairs[j].loser = pairs[k + 1].loser;

                pairs[k + 1].winner = val1;
                pairs[k + 1].loser = val2;
            }
        }
        printf("#%i par é %i e %i com dif = %i \n", j, pairs[j].winner, pairs[j].loser, X[j]);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
bool check_tree(int w, int l)
{
    if(l == w)
    {
        return false;
    }
    for(int i = 0; i < candidate_count; i++)
    {
        if(locked[l][i] == true)
        {
            return check_tree(w, i);
        }
    }
    return true;
}


void lock_pairs(void)
{
    int A, B;
    bool C;
    for(int i = 0; i < pair_count; i++)
    {
        A = pairs[i].winner;
        B = pairs[i].loser;
        C = check_tree(A,B);
        if(C)
        {
            locked[A][B] = true;
            printf("the pair %i and %i lock result: true\n", A, B);
        }
        else
        {
            locked[A][B] = false;
            printf("the pair %i and %i lock result: false\n", A, B);
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for(int i = 0; i < pair_count; i++)
    {
        if(locked[pairs[i].winner][pairs[i].loser] == true)
        {
            int X = pairs[i].winner;
            printf("%s", candidates[X]);
            return;
        }
    }
}

