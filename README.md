# Voting-Blockchain
## This is a submission to a task, which includes understanding and creating a Blockchain for the application of Voting. Using this, we will record votes transparently, securely and in tamper-evident way.

Language used to complete this task is C++.
The Problem Statement had demanded to build a simple blockchain-based voting system from scratch which should include :
- Block Structure
- Blockchain Class
- Voter and Candidate Management
- Vote Counting
- Tampering Detection
- Contract like Voting Rules
- Election Result Declaration
- Simple User Interface

### Features

1. #### Registered Voter Verification
The Voters are actually already registered hardcoded in a vector "reg_voter_id_list". This function checks if the input voter id is stored in "reg_voter_id_list" or not. If yes, then it returns true as it can be added in the block.
```
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
```
2. #### Registered Candidate Verification

3. #### Voting Rules Contract

4. #### Blockchain Based Vote Storage

5. #### Calculating Current Hash Using SHA256

6. #### Block Linking Through Previous Hash

7. #### Genesis Block Creation

8. #### Tamper Detection

9. #### Vote Counting

10. #### Winner Declaration


