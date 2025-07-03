#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include "Utilities.h"
#include "Chain.h"

using namespace std;

// Function to display available commands
void displayHelp()
{
  cout << "\nAvailable commands:\n";
  cout << "add <sender> <receiver> <amount> - Add a new transaction\n";
  cout << "delete <txHash> - Delete a transaction from the pool\n";
  cout << "edit <txHash> <sender> <receiver> <amount> - Edit a transaction in the pool\n";
  cout << "verify - Verify the integrity of the blockchain\n";
  cout << "view - Display the entire blockchain\n";
  cout << "save <filename> - Save the blockchain to a file\n";
  cout << "load <filename> - Load a blockchain from a file\n";
  cout << "pool - View transactions in the pool\n";
  cout << "help - Display this help message\n";
  cout << "exit - Exit the program\n";
}

// Function to parse and execute commands
void processCommand(const string &command, Chain &blockchain)
{
  vector<string> tokens = helpers::lineSplit(command, ' ');

  if (tokens.empty())
    return;

  string cmd = tokens[0];

  if (cmd == "add" && tokens.size() >= 4)
  {
    try
    {
      string sender = tokens[1];
      string receiver = tokens[2];
      double amount = stod(tokens[3]);

      Transaction tx(sender, receiver, amount);
      blockchain.addTransaction(tx);
      cout << "Transaction added successfully. Hash: " << tx.getTxHash() << endl;
    }
    catch (const exception &e)
    {
      cout << "Error adding transaction: " << e.what() << endl;
    }
  }
  else if (cmd == "delete" && tokens.size() >= 2)
  {
    string txHash = tokens[1];
    bool success = blockchain.removeTransactionFromPool(txHash);
    if (success)
    {
      cout << "Transaction removed successfully." << endl;
    }
    else
    {
      cout << "Transaction not found in pool." << endl;
    }
  }
  else if (cmd == "edit" && tokens.size() >= 5)
  {
    try
    {
      string txHash = tokens[1];
      string sender = tokens[2];
      string receiver = tokens[3];
      double amount = stod(tokens[4]);

      bool success = blockchain.editTransactionInPool(txHash, sender, receiver, amount);
      if (success)
      {
        cout << "Transaction edited successfully." << endl;
      }
      else
      {
        cout << "Transaction not found in pool." << endl;
      }
    }
    catch (const exception &e)
    {
      cout << "Error editing transaction: " << e.what() << endl;
    }
  }
  else if (cmd == "verify")
  {
    bool isValid = blockchain.verifyChain();
    if (isValid)
    {
      cout << "Blockchain is valid." << endl;
    }
    else
    {
      cout << "Blockchain verification failed!" << endl;
    }
  }
  else if (cmd == "view")
  {
    blockchain.displayChain();
  }
  else if (cmd == "save" && tokens.size() >= 2)
  {
    string filename = tokens[1];
    bool success = blockchain.writeCurrentChainToFile(filename);
    if (success)
    {
      cout << "Blockchain saved to " << filename << endl;
    }
    else
    {
      cout << "Failed to save blockchain." << endl;
    }
  }
  else if (cmd == "load" && tokens.size() >= 2)
  {
    string filename = tokens[1];
    bool success = blockchain.readChainFromFile(filename);
    if (success)
    {
      cout << "Blockchain loaded from " << filename << endl;
    }
    else
    {
      cout << "Failed to load blockchain." << endl;
    }
  }
  else if (cmd == "pool")
  {
    blockchain.displayTransactionPool();
  }
  else if (cmd == "help")
  {
    displayHelp();
  }
  else if (cmd == "exit")
  {
    cout << "Exiting program..." << endl;
    exit(0);
  }
  else
  {
    cout << "Unknown command. Type 'help' for available commands." << endl;
  }
}

int main()
{
  cout << "BlockChain Service Started\n";
  cout << "Enter difficulty level (e.g., 3): ";
  unsigned int difficulty;
  cin >> difficulty;

  cout << "Enter block size threshold (e.g., 5): ";
  unsigned int blockSize;
  cin >> blockSize;

  // Clear input buffer
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  Chain blockchain(difficulty, blockSize);
  cout << "\nBlockchain initialized with difficulty " << difficulty
       << " and block size " << blockSize << endl;

  displayHelp();

  string command;
  while (true)
  {
    cout << "\n> ";
    getline(cin, command);

    if (!command.empty())
    {
      processCommand(command, blockchain);
    }
  }

  return 0;
}