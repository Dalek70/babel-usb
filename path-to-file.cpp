#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>
#include <algorithm>
#include <cstring>

using namespace std;

struct BigInt {
    vector<uint32_t> digits;

    BigInt(size_t reserve_size = 1) {
        digits.reserve(reserve_size);
    }

    void multiplyAdd(uint64_t mul, uint64_t add) {
        uint64_t carry = add;
        for (size_t i = 0; i < digits.size() || carry; ++i) {
            if (i == digits.size()) digits.push_back(0);
            unsigned __int128 cur = (unsigned __int128)digits[i] * mul + carry;
            digits[i] = (uint32_t)(cur & 0xFFFFFFFFULL);
            carry = (uint64_t)(cur >> 32);
        }
        while (!digits.empty() && digits.back() == 0) digits.pop_back();
    }

    void increment() {
        for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] < 0xFFFFFFFF) {
                digits[i]++;
                return;
            } else {
                digits[i] = 0;
            }
        }
        digits.push_back(1);
    }
};

const int BABEL_ALPHABET_LENGTH = 70;
const string alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "! &()-_+[]^=";

int get_char_index(char c) {
    size_t pos = alphabet.find(c);
    if (pos == string::npos) return -1;
    return (int)pos;
}

void print_help() {
    cout << "Usage: path-to-file <output_file>\n";
    cout << "Reads path string from stdin and converts it back to the original file.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: No output file specified!\n";
        print_help();
        return 1;
    }

    string arg = argv[1];
    if (arg == "--help" || arg == "-help" || arg == "--h" || arg == "-h") {
        print_help();
        return 0;
    }

    string path;
    getline(cin, path);

    vector<uint32_t> handles;
    for (size_t i = 0; i < path.length(); i += 3) {
        if (path[i] != '/') continue;
        int c1 = get_char_index(path[i+1]);
        int c2 = get_char_index(path[i+2]);
        handles.push_back(c1 + c2 * BABEL_ALPHABET_LENGTH);
    }

    BigInt index(handles.size() / 2 + 1);
    for (int i = (int)handles.size() - 1; i >= 0; --i) {
        index.multiplyAdd(4900, handles[i]);
        index.increment();
    }

    ofstream outfile(arg, ios::binary);
    for (uint32_t digit : index.digits) {
        for (int i = 0; i < 4; ++i) {
            outfile.put((char)((digit >> (i * 8)) & 0xFF));
        }
    }

    return 0;
}
