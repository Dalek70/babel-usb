#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>

using namespace std;

struct BigInt {
    vector<uint32_t> digits;

    BigInt(size_t reserve_size) {
        digits.reserve(reserve_size);
    }

    void decrement() {
        for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] > 0) {
                digits[i]--;
                break;
            } else {
                digits[i] = 0xFFFFFFFF;
            }
        }
        while (!digits.empty() && digits.back() == 0) digits.pop_back();
    }

    uint32_t divMod4900() {
        uint64_t rem = 0;
        for (int i = (int)digits.size() - 1; i >= 0; --i) {
            uint64_t cur = digits[i] + (rem << 32);
            digits[i] = (uint32_t)(cur / 4900);
            rem = cur % 4900;
        }
        while (!digits.empty() && digits.back() == 0) digits.pop_back();
        return (uint32_t)rem;
    }

    bool isZero() const {
        return digits.empty();
    }
};

const int BABEL_ALPHABET_LENGTH = 70;

const string alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "! &()-_+[]^=";

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "No file specified!\n";
        return 1;
    }

    ifstream file(argv[1], ios::binary | ios::ate);
    if (!file) {
        cerr << "File not found!\n";
        return 1;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> bytes(size);
    if (size > 0) {
        file.read((char*)bytes.data(), size);
    }

    BigInt index((size + 3) / 4 + 1);

    if (size > 0) {
        index.digits.resize((size + 3) / 4, 0);
        for (size_t i = 0; i < size; i++) {
            index.digits[i / 4] |= ((uint32_t)bytes[i]) << ((i % 4) * 8);
        }

        uint64_t carry = 0;
        for (size_t i = 0; i < index.digits.size(); i++) {
            uint32_t add = 0x01010101;
            if (i == index.digits.size() - 1) {
                int rem = size % 4;
                if (rem == 1) add = 0x00000001;
                else if (rem == 2) add = 0x00000101;
                else if (rem == 3) add = 0x00010101;
            }
            uint64_t cur = (uint64_t)index.digits[i] + add + carry;
            index.digits[i] = (uint32_t)(cur & 0xFFFFFFFF);
            carry = cur >> 32;
        }
        if (carry > 0) index.digits.push_back(carry);
    }

    vector<string> parts;
    parts.reserve(size); 

    while (!index.isZero()) {
        index.decrement();
        uint32_t handle = index.divMod4900();
        
        string name(3, '/');
        name[2] = alphabet[handle % BABEL_ALPHABET_LENGTH];
        handle /= BABEL_ALPHABET_LENGTH;
        name[1] = alphabet[handle % BABEL_ALPHABET_LENGTH];
        
        parts.push_back(name);
    }

    string path;
    path.reserve(parts.size() * 3);
    for (int i = (int)parts.size() - 1; i >= 0; --i) {
        path += parts[i];
    }

    cout << path << "\n";
    return 0;
}
