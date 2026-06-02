#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "sha256.h" // Downloaded from B-Con/CryptoAlgorithm repository on github.
using namespace std;

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

vector<string> reg_voter_id_list{"Voter1124", "Voter2345", "Voter4354", "Voter2355", "Voter0977"};
vector<string> reg_candidates_list{"Raj", "Sarthak", "Mohan"};
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

class voting_data
{
    private :
    string voter_id_00;
    string vote_to_candidate_00;

    public :
    void set_voter_id_00(string votid)
    {
        voter_id_00 = votid;
    }
    string get_voter_id_00()
    {
        return voter_id_00;
    }

    void set_vote_to_candidate_00(string vocan)
    {
        vote_to_candidate_00 = vocan;
    }
    string get_vote_to_candidate_00()
    {
        return vote_to_candidate_00;
    }

    voting_data(string vid, string voc)
    {
        set_voter_id_00(vid);
        set_vote_to_candidate_00(voc);
    }
};

class block
{
    private :
        int index;
        string timestamp;
        voting_data vote_data;
        string previousHash;
        string currentHash;
        
    public :
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
    string calculatehash()
    {
        string input = (to_string(index) + timestamp + vote_data.get_voter_id_00() + vote_data.get_vote_to_candidate_00() + previousHash);
        return generate_sha256(input);
        
    }
    block(int idx, voting_data vodata, string prevhash) : vote_data(vodata)
    {
        index = idx;
        previousHash = prevhash;
        time_t now = time(0);
        timestamp = ctime(&now);
        currentHash = calculatehash();
    }

};

class blockchain
{
    private :
    vector<block> voteblockchain;
    int initial_index = 0;
    int indexforuse;
    public :
    void add_vote(string voter_id, string candidate)
    {
        
        if (indexforuse != 0)
        {
            if(valid_reg_voter_id(voter_id) && valid_candidate(candidate))
            {   
                if(indexforuse != 1)
                {
                    bool is_duplicate = false;
                    for(int i = 1; i < voteblockchain.size(); i++)
                    {
                        if (voteblockchain[i].get_vote_data().get_voter_id_00() == voter_id)
                        {
                            is_duplicate = true;
                            break;
                        }
                    }
                    if(!is_duplicate)
                    {
                        voting_data vdata{voter_id, candidate};
                        block bl{indexforuse++, vdata, voteblockchain.back().get_currentHash()};
                        voteblockchain.push_back(bl);
                    }
                    else
                    {
                        cout << "You have already voted!\n\n";
                    }
                }
                else
                {
                    voting_data vdata{voter_id, candidate};
                    block bl{indexforuse++, vdata, voteblockchain.back().get_currentHash()};
                    voteblockchain.push_back(bl);
                }
            }
            
        }
        else // For Genesis
        {
            voting_data vdata{voter_id, candidate};
            block bl{indexforuse++, vdata, "0"};
            voteblockchain.push_back(bl);
        }
    }
    void create_genesis_block()
    {
        indexforuse = initial_index;
        add_vote("Zeroth_User", "No vote");
    }
    block& get_block_at_idx(int idx)
    {
        return voteblockchain[idx];
    }
    int get_blockchainsize()
    {
        return voteblockchain.size();
    }
    bool isvalid()
    {   
        
        for(int i = 1; i < voteblockchain.size(); i++)
        {
            if(voteblockchain[i].get_currentHash() != voteblockchain[i].calculatehash() || voteblockchain[i].get_previousHash() != voteblockchain[i-1].get_currentHash() )
            {
                return false;
            }
            else
            {
                for(int j = 1; j < voteblockchain.size(); j ++)
                {
                    if ( j != i)
                    {
                        if (voteblockchain[i].get_vote_data().get_voter_id_00() == voteblockchain[j].get_vote_data().get_voter_id_00())
                        {
                            return false;
                        }
                    }
                }
                
            }
            
        }
        return true;
    }
};

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

void print_blockchain(blockchain& blkch)
{
    for(int i = 1; i< blkch.get_blockchainsize(); i++)
    {
        print_block(blkch, i);
    }
}

vector<int> count_votes(blockchain& blkchn)
{
    vector<int> votes{0, 0, 0};
    for(int i = 1; i < blkchn.get_blockchainsize(); i++)
    {
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
        if(votes[i] > g)
        {
            g = votes[i];
            
        }
    }
    vector<int> winners{};
    for(int i = 0; i < 3; i++)
    {
        if(g==votes[i])
        {
            winners.push_back(i);
        }
    }
    if (winners.size() == 1)
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
    else if (winners.size() == 2)
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
    else if (winners.size() == 3)
    {
        return "Tie!";
    }
    return "Error!";
}

void Exit()
{
    //This is the End of the program.
    //And this "End" is the signature step of me(Harish) in all the programs.
    cout << "\n" << "End!" << "\n";
}
void Register_Vote(blockchain& voting_blkchn)
{
    string voter_id;
    string candidate_voted;
    int voting_vector_before_size = voting_blkchn.get_blockchainsize();
    cout << "Enter Your Voter ID : ";
    cin >> voter_id;
    cout << "Enter the name of Candidate, you want to vote for : ";
    cin >> candidate_voted;
    voting_blkchn.add_vote(voter_id, candidate_voted);
    if(voting_blkchn.get_blockchainsize() > voting_vector_before_size)
    {
        cout << "Vote Added!\n";
        // cout << "Vote Block Hash : " << voting_blkchn.get_block_at_idx(voting_blkchn.get_blockchainsize() - 1).get_currentHash() << "\n";
        // This current hash can be used as reciept for common user.
    }
}
void start(blockchain& voting_blkchn)
{
    int input_menu;
    cout << "\nMenu :\n1. Register Vote\n2. View Blockchain\n3. Count Votes\n4. Check Chain Validity\n5. Declare Winner\n6. Exit\nChoose a menu to proceed with and Enter its number : ";
    cin >> input_menu;
    if(input_menu == 1)
    {
        int input_submenu = 0;
        do
        {
            cout << "\n";
            Register_Vote(voting_blkchn);

            cout << "\nSubmenu :\n1. Register New Vote\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else if (input_submenu == 3)
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
        else if (input_submenu == 3)
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
            cout << "Mohan : " << count[2] << "\n\n";

            cout << "\nSubmenu :\n1. Count Votes Again\n2. Return To Main Menu\n3. Exit\nChoose a menu to proceed with and Enter its number : ";
            cin >> input_submenu;
        } while (input_submenu == 1);

        if (input_submenu == 2)
        {
            start(voting_blkchn);
        }
        else if (input_submenu == 3)
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
        else if (input_submenu == 3)
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
        else if (input_submenu == 3)
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
        cout << "Invalid Choice!\n";
        start(voting_blkchn);
    }
}
void Display(blockchain& voting_blkchn)
{
    
    int input_start = 1;
    while(input_start != 0)
    {
        cout << "Enter '0' to start voting : ";
        cin >> input_start;
    }
    start(voting_blkchn);
}

int main()
{
    cout << "Your Candidates are :\n1. Raj\n2. Sarthak\n3. Mohan\n";
    blockchain Voting;
    Voting.create_genesis_block();
    Display(Voting); // This will start User Interface.


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
