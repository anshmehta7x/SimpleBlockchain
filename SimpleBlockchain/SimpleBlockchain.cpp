#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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


int main() {

  //Chain attributes: difficulty, block size (threshold)
  cout << "BlockChain service Started";
  unsigned int difficulty;
  unsigned int blockSize;
	cin >> difficulty;
	cin >> blockSize;

	Chain blockchain(difficulty, blockSize);


   return 0;
}