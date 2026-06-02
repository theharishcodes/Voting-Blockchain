#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "sha256.h" // Downloaded from B-Con/CryptoAlgorithm repository on github.
using namespace std;

// SHA256.h Generator Function
string generate_sha256(string input)
{
    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx,
                  reinterpret_cast<const BYTE*>(input.c_str()),
                  input.length());
    sha256_final(&ctx, hash);

    string result;

    char buffer[3];

    for(int i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        sprintf(buffer, "%02x", hash[i]);
        result += buffer;
    }

    return result;
    // This one Function was generated through AI, as I was unable to use SHA256 in my code. 
}

// Registered Voters
vector<string> reg_voter_id_list{"Voter1124", "Voter2345", "Voter4354", "Voter2355", "Voter0977"};
// Registered Candidates
vector<string> reg_candidates_list{"Raj", "Sarthak", "Mohan"};

// To check if the voter id in input is registered or not.
bool valid_reg_voter_id(string votrid)
{
    for(int i = 0; i < reg_voter_id_list.size(); i++)
    {   
        if (votrid == reg_voter_id_list[i])
        {
            return true;
        }
    }
    cout << "Invalid Voter ID!\n";
    return false;
}
// To check if the Cadidate Name in input is registered or not.
bool valid_candidate(string candt)
{
    for(int i = 0; i < reg_candidates_list.size(); i++)
    {
        if (candt == reg_candidates_list[i])
        {
            return true;
        }
    }
    cout << "Candidate Chosen by you is not registered!\n";
    return false;
}

// Voting Data - We will take this two as input.
class voting_data
{
    private :
    string voter_id_00;
    string vote_to_candidate_00;

    public :
    
    // Setter and Getter voter_id_00.
    void set_voter_id_00(string votid)
    {
        voter_id_00 = votid;
    }
    string get_voter_id_00()
    {
        return voter_id_00;
    }

    // Setter and Getter vote_to_candidate_00.
    void set_vote_to_candidate_00(string vocan)
    {
        vote_to_candidate_00 = vocan;
    }
    string get_vote_to_candidate_00()
    {
        return vote_to_candidate_00;
    }

    // Constructor
    voting_data(string vid, string voc)
    {
        set_voter_id_00(vid);
        set_vote_to_candidate_00(voc);
    }
};

// A Block in BlockChain, it consist of index, timestamp, votedata-voter id and vote to candidate, previous hash and current hash.
class block
{
    private :
        int index;
        string timestamp;
        voting_data vote_data;
        string previousHash;
        string currentHash;
        
    public :

    // Getters
    int get_index()
    {
        return index;
    }
    voting_data& get_vote_data()
    {
        return vote_data;
    }
    string get_previousHash()
    {
        return previousHash;
    }
    string get_currentHash()
    {
        return currentHash;
    }
    string get_timestamp()
    {
        return timestamp;
    }

    // Calculate Current hash using SHA256.
    string calculatehash()
    {
        string input = (to_string(index) + timestamp + vote_data.get_voter_id_00() + vote_data.get_vote_to_candidate_00() + previousHash);
        return generate_sha256(input);
        
    }

    //  Constructor
    block(int idx, voting_data vodata, string prevhash) : vote_data(vodata)
    {
        index = idx;
        previousHash = prevhash;
        time_t now = time(0); // Epoch time of the current time.
        timestamp = ctime(&now); // Converted to Readable time, as a string.
        currentHash = calculatehash(); // CurrentHash Calculated using index, timestamp, voterid, candidate voted, previous hash.
    }

};

// A Contract which we will use while adding new vote.
/*
    It checks
        1. Is Input Voter ID is registered?
        2. Is the Candidate, which is voted, registered?
        3. Has this voter already voted? - Checking duplicate Votes - No means True and Yes means False.
*/
bool Voting_Rules_Contract(vector<block>& voteblock, string voterid, string candidate)
{
    if (valid_reg_voter_id(voterid) && valid_candidate(candidate)) // Checks 1. and 2.
    {
        // Its yes to 1. and 2., then proceeds for Duplicate Voting Check.

        // If it's the first vote, then of course there is no duplicate, so return true.
        if(voteblock.size()==1)
        {
            return true;
        }

        // if its not the index 1 block, i.e if it is not the first block after genesis.
        bool is_duplicate = false;
        for(int i = 1; i < voteblock.size(); i++)
        {
            // Run a loop in existing blockchain.
            if (voteblock[i].get_vote_data().get_voter_id_00() == voterid)
            {
                // Check if new vote's voterid is already in blockchain, if yes, then its a duplicate, he/she is voting again.
                is_duplicate = true;
                break; // If we find duplicate even once, we don't go further as there's no need. We already got the theif(Just for the example, hehe).
            }
        }

        if(!is_duplicate)
        {
            // If there is no duplicate, then return true.
            return true;
        }
        else
        {
            // If there's a duplicate, return false and don't count the vote.
            cout << "You have already voted! We will not count this vote.\n";
            return false;
        }
    }
    else
    {
        // If it doesn't satisfy any of the 1. and 2. conditions.
        return false;
    }
}

// The collection of Blocks - Blockchain. Major role of this class is to define a vector, as we will use vector data type for our blockchain.
class blockchain
{
    private :
    vector<block> voteblockchain;
    int indexforuse;
    public :

    // To add new vote/block to blockchain.
    void add_vote(string voter_id, string candidate)
    {
        if (indexforuse != 0) // When it's not genesis block.
        {
            if(Voting_Rules_Contract(voteblockchain, voter_id, candidate))
            {
                // Here voter_id and candidate name, come from user.
                voting_data vdata{voter_id, candidate};
                block bl{indexforuse++, vdata, voteblockchain.back().get_currentHash()}; // The third parameter is previous hash. We assign "current_hash" of previous block to "previous_hash" of current/new block.
                voteblockchain.push_back(bl);
            }
        }
        else // For Genesis
        {
            // Here, voter_id and candidate name is provided by create_genesis_block().
            voting_data vdata{voter_id, candidate};
            block bl{indexforuse++, vdata, "0"}; // Here, previous_hash is provided as zero. Why? Just for fun, we can assign anything to it.
            voteblockchain.push_back(bl);
        }
    }

    // To Create Genesis Block.
    void create_genesis_block()
    {
        if(voteblockchain.size() != 0)
        {
            // If there is already a block present in blockchain, then we don't need another genesis.
            return;
        }
        else
        {
            // When there is no block present in blockchain, create genesis.
            indexforuse = 0;
            add_vote("Zeroth_User", "No vote");
        }
    }

    // Getters.
    block& get_block_at_idx(int idx)
    {
        return voteblockchain[idx];
    }
    int get_blockchainsize()
    {
        return voteblockchain.size();
    }

    // IsValid Function - Used for Tampering Detection
    bool isvalid()
    {   
        
        for(int i = 1; i < voteblockchain.size(); i++)
        {
            if(voteblockchain[i].get_currentHash() != voteblockchain[i].calculatehash() || voteblockchain[i].get_previousHash() != voteblockchain[i-1].get_currentHash() )
            {
                /* Two conditions checked
                    1. Calculate the current hash of block and see if it is actually equal to current_hash of that block. If not, then the data is changed.
                    2. If previousHash is equal to currentHash, then no new block is added in the middle of blockchain.
                */
                return false;
            }
            else
            {
                for(int j = 1; j < voteblockchain.size(); j ++)
                {
                    if ( j != i && voteblockchain[i].get_vote_data().get_voter_id_00() == voteblockchain[j].get_vote_data().get_voter_id_00())
                    {
                        // Checking if the voter has already voted.
                        return false;
                    }
                }
                
            }
            
        }
        return true;
    }
};

// Printing Complete Block - used to View Blockchain.
void print_block(blockchain& blch, int idx)
{
    block& block_at_idx = blch.get_block_at_idx(idx);
    cout << "Index : " << block_at_idx.get_index() << "\n";
    cout << "Voter Id : " << block_at_idx.get_vote_data().get_voter_id_00() << "\n";
    cout << "Canditate Voted : " << block_at_idx.get_vote_data().get_vote_to_candidate_00() << "\n";
    cout << "Previous Hash : " << block_at_idx.get_previousHash() << "\n";
    cout << "Current Hash : " << block_at_idx.get_currentHash() << "\n";
    cout << "Time : " << block_at_idx.get_timestamp() << "\n";
}

// Printing Complete Blockchain - used to View Blockchain.
void print_blockchain(blockchain& blkch)
{
    for(int i = 1; i< blkch.get_blockchainsize(); i++)
    {
        print_block(blkch, i);
    }
}

// To Count Votes.
vector<int> count_votes(blockchain& blkchn)
{
    vector<int> votes{0, 0, 0}; // It is vector of votes for candidates like {votes of Raj, votes of Sarthak, votes of Mohan}
    for(int i = 1; i < blkchn.get_blockchainsize(); i++)
    {
        // increasing vote count in vector.
        if(blkchn.get_block_at_idx(i).get_vote_data().get_vote_to_candidate_00() == "Raj")
        {
            votes[0]++; // Raj
        }
        else if(blkchn.get_block_at_idx(i).get_vote_data().get_vote_to_candidate_00() == "Sarthak")
        {
            votes[1]++; // Sarthak
        }
        else if(blkchn.get_block_at_idx(i).get_vote_data().get_vote_to_candidate_00() == "Mohan")
        {
            votes[2]++; // Mohan
        }
    }
    
    return votes;
}

string declare_winner(blockchain& blkchn)
{
    int g = 0;
    vector<int> votes = count_votes(blkchn);
    for(int i = 0; i < 3; i++)
    {
        // Finding Maximum number of votes.
        if(votes[i] > g)
        {
            g = votes[i];
            
        }
    }
    vector<int> winners{};
    for(int i = 0; i < 3; i++)
    {
        // Finding who has that maximum number of votes and assigning his/her number(candidate number, 1, 2, 3, etc.) in the 'winner' Vector.
        if(g==votes[i])
        {
            winners.push_back(i);
        }
    }
    if (winners.size() == 1) // If size == 1, there is one winner.
    {
        if (winners[0] ==0)
        {
            return "Raj";
        }
        else if (winners[0] ==1)
        {
            return "Sarthak";
        }
        else if (winners[0] ==2)
        {
            return "Mohan";
        }
    }
    else if (winners.size() == 2) // If size == 2, there is tie between 2 winners..
    {
        if (winners[0] == 0 && winners[1] == 1)
        {
            return "Tie Between Raj and Sarthak!";
        }
        else if (winners[0] == 1 && winners[1] == 2)
        {
            return "Tie Between Sarthak and Mohan!";
        }
        else if (winners[0] == 0 && winners[1] == 2)
        {
            return "Tie Between Raj and Mohan!";
        }
    }
    else if (winners.size() == 3) // 3 Candidates - 3 Winners, A Global Tie.
    {
        return "Tie!";
    }
    return "Error!";
}

// Exit Option in Command Line Interface Menu.
void Exit()
{
    // This is the End of the program.
    // And this "End" is the signature step of me(Harish) in all the programs.
    cout << "\n" << "End!" << "\n";
}

// Register Vote Option in Command Line Interface Menu.
void Register_Vote(blockchain& voting_blkchn)
{
    string voter_id;
    string candidate_voted;
    int voting_vector_before_size = voting_blkchn.get_blockchainsize();

    // Taking Voting Data as input from Voter/user.
    cout << "Enter Your Voter ID : ";
    cin >> voter_id;
    cout << "Enter the name of Candidate, you want to vote for : ";
    cin >> candidate_voted;

    // add block
    voting_blkchn.add_vote(voter_id, candidate_voted);

    // if the contract is checked and a new block is added, size of blockchain will added and this way we can know that the Vote is Added.
    if(voting_blkchn.get_blockchainsize() > voting_vector_before_size)
    {
        cout << "Vote Added!\n";

        // cout << "Vote Block Hash : " << voting_blkchn.get_block_at_idx(voting_blkchn.get_blockchainsize() - 1).get_currentHash() << "\n";
        // This current hash can be used as reciept for common user.

        // system("cls"); // This can be used so the current voter don't see the vote of previous voter on screen.
    }
}

// Start function to show the menu and ask for 'Menu number' as input. And accordingly run the option.
void start(blockchain& voting_blkchn)
{
    int input_menu;
    
    // Show Menu
    cout << "\nMenu :\n1. Register Vote\n2. View Blockchain\n3. Count Votes\n4. Check Chain Validity\n5. Declare Winner\n6. Exit\nChoose a menu to proceed with and Enter its number : ";
    cin >> input_menu;

    // If statements based on users input.
    if(input_menu == 1)
    {
        int input_submenu = 0;
        do
        {
            cout << "\n";
            Register_Vote(voting_blkchn);

            // Show Submenu
            cout << "\nSubmenu :\n1. Register New Vote\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else // IF wrong submenu number is choosed, exit the complete system.
        {
            Exit();
        }
    }
    else if(input_menu == 2)
    {

        int input_submenu = 0;
        do
        {
            cout << "\n";
            print_blockchain(voting_blkchn);

            cout << "\nSubmenu :\n1. View Blockchain Again\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else
        {
            Exit();
        }
    }
    else if(input_menu == 3)
    {
        int input_submenu = 0;
        do
        {
            cout << "\n";
            vector<int> count = count_votes(voting_blkchn);
            cout << "Total Vote Counting :\n";
            cout << "Raj : " << count[0] << "\n";
            cout << "Sarthak : " << count[1] << "\n";
            cout << "Mohan : " << count[2] << "\n";

            cout << "\nSubmenu :\n1. Count Votes Again\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else
        {
            Exit();
        }
    }
    else if(input_menu == 4)
    {
        int input_submenu = 0;
        do
        {
            cout << "\n";
            cout << "Validity : ";
            if(voting_blkchn.isvalid())
            {
                cout << "true\n";
            }
            else
            {
                cout << "false\n";
            };

            cout << "\nSubmenu :\n1. Check Validity Again\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else
        {
            Exit();
        }
    }
    else if(input_menu == 5)
    {
        int input_submenu = 0;
        do
        {
            cout << "\n";
            cout << "Winner : " << declare_winner(voting_blkchn) << "\n";
            cout << "\nSubmenu :\n1. Declare Winner Again\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else
        {
            Exit();
        }
    }
    else if(input_menu == 6)
    {
        Exit();
    }
    else
    {
        // if wrong menu number is choosen, ask again.
        cout << "Invalid Choice!\n";
        start(voting_blkchn);
    }
}

// To invoke the start() function.
void Display(blockchain& voting_blkchn)
{
    
    int input_start = 1;
    while(input_start != 0) // Till the input is not what is asked, just ask again and again.
    {
        cout << "Enter '0' to start voting : ";
        cin >> input_start;
    }
    start(voting_blkchn);
}

int main()
{
    // Show Candidates to user.
    cout << "Your Candidates are :\n1. Raj\n2. Sarthak\n3. Mohan\n";

    // Create a blockchain.
    blockchain Voting;
    Voting.create_genesis_block(); // Create a genesis.
    Display(Voting); // Start the mechanism of Command Line Interface Menu.


    /* Manual Internal Methods used to see if the code runs.
    
    Voting.create_genesis_block();

    Voting.add_vote("Voter1124", "Mohan");

    Voting.add_vote("Voter2345", "Raj");

    Voting.add_vote("Voter255", "Raj"); // Not registered Voter ID.

    Voting.add_vote("Voter2355", "Ram"); // Not registered Candidate.

    Voting.add_vote("Voter2345", "Mohan"); // Duplicate Vote.

    Voting.add_vote("Voter0977", "Sarthak");

    
    print_blockchain(Voting);

    cout << "Total Vote Counting :\n";
    cout << "Raj : " << count_votes(Voting)[0] << "\n";
    cout << "Sarthak : " << count_votes(Voting)[1] << "\n";
    cout << "Mohan : " << count_votes(Voting)[2] << "\n\n";

    cout << "Winner : " << declare_winner(Voting) << "\n";

    // Tempering Attempt
    //Voting.get_block_at_idx(1).get_vote_data().set_vote_to_candidate_00("Raj");

    cout << "\nValidity : ";
    if(Voting.isvalid())
    {
        cout << "true\n";
    }
    else
    {
        cout << "false\n";
    } */


    
    return 0;
}
