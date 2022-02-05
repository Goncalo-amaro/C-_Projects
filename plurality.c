#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    int p = 0;
    for(int i = 0; i < candidate_count; i++)
    {
        if(strcmp(name, candidates[i].name) == 0)
        {
        ++candidates[i].votes;
        // printf("+ 1 pa %s:", candidates[i].name);
        // printf(" num de votos =  %i \n", candidates[i].votes);
        p = 1;
        }
    }
    if (p != 0 )
    return true;
    
    else
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int X = candidates[0].votes;
    string Y = candidates[0].name, Z = candidates[0].name;
    for (int i = 1; i < candidate_count; i++)
    {
        if(X < candidates[i].votes)
        {
        Y = candidates[i].name;
        X = candidates[i].votes;
        }
    }
    for ( int k = 0; k < candidate_count; k++)
    {
        if(X == candidates[k].votes && Y != candidates[k].name)
        {
            Z = candidates[k].name;
            printf("%s \n", Z);
        }
    }
    printf("%s\n", Y);
    return;
}

