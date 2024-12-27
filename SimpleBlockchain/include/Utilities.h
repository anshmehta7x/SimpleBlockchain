
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

namespace helpers {
    // Function to split a string based on a delimiter
    inline vector<string> lineSplit(const string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;

        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
}


