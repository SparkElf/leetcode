#include "huffman.cpp"
using namespace std;
using namespace Huffman;
int main() {
    auto inOriginal = ifstream("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\originalfile.txt");
    auto inEncodedfile = ifstream("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\encodingfile.txt");
    auto outDecodedfile = ofstream("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\decodingfile.txt");

    auto compressor = Compressor(inOriginal);
    compressor
        .encode()
        .toFile(filesystem::path("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\encodingfile.txt"));
    print(compressor.nodes, filesystem::path("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\huffmanfile.txt"));
    print(compressor.codingTable, filesystem::path("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\codewordbook.txt"));
    print(compressor.info, filesystem::path("C:\\Users\\37465\\Desktop\\project\\leetcode\\c++\\analysis.txt"));
    auto decoder = Decompressor(inEncodedfile);
    decoder.decode(outDecodedfile);
    return 0;
}