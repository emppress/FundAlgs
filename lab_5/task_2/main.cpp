#include <iostream>
#include <cmath>
#include <vector>
#include <cstddef>
#include <bitset>
#include <string>
#include <filesystem>
#include <fstream>
#include <random>

std::ostream &operator<<(std::ostream &os, std::byte b)
{
    return os << char(std::to_integer<int>(b));
}

class encoder
{
private:
    std::vector<std::byte> S, _key;
    int x, y;

    int to_int(std::byte byte) { return std::to_integer<int>(byte); }

    void init()
    {
        int key_length = _key.size();
        for (int i = 0; i < 256; i++)
        {
            S[i] = std::byte((unsigned char)i);
        }

        int j = 0;
        for (int i = 0; i < 256; i++)

        {
            j = (j + to_int(S[i]) + to_int(_key[i % key_length])) % 256;
            std::swap(S[i], S[j]);
        }
    }

    std::byte get_s_item()

    {
        x = (x + 1) % 256;
        y = (y + to_int(S[x])) % 256;

        std::swap(S[x], S[y]);
        return S[(to_int(S[x]) + to_int(S[y])) % 256];
    }

public:
    encoder(std::vector<std::byte> &key) : S(256), _key(key), x(0), y(0) {};
    void set_key(std::vector<std::byte> const &key) { _key = key; }

    void encode(std::string const &input_name, std::string const &output_name)
    {
        std::filesystem::path p1{input_name};
        std::filesystem::path p2{output_name};
        if (std::filesystem::equivalent(p1, p2))
        {
            throw std::runtime_error("Two identical files have been transferred");
        }

        std::ifstream in;
        std::ofstream out;
        in.open(input_name, std::ios::in);
        if (!in.is_open())
        {
            in.close();
            throw std::runtime_error("Couldn't open the input file");
        }
        out.open(output_name, std::ios::out);
        if (!out.is_open())
        {
            in.close();
            out.close();
            throw std::runtime_error("Couldn't open the output file");
        }

        init();
        x = 0, y = 0;

        std::byte byte, checker{0x0};
        char ch;
        while ((ch = in.get()) != EOF)
        {
            byte = std::byte{(unsigned char)ch};
            out << (byte ^ get_s_item());
        }
        in.close();
        out.close();
    }

    static std::vector<std::byte> generate_key(size_t size)
    {
        srand((unsigned)time(NULL));
        std::vector<std::byte> key(size);
        for (int i = 0; i < size; ++i)
        {
            key[i] = std::byte(rand() % 256);
        }
        return key;
    }
};

int main()
{
    std::vector<std::byte> key;

    key = encoder::generate_key(100);

    encoder encdr(key);
    std::string a{"in.txt"}, b{"code.txt"}, c{"decode.txt"}, d{"decode_2.txt"};
    encdr.encode(a, b);
    encdr.encode(b, c);

    encdr.set_key(encoder::generate_key(256));
    encdr.encode(a, b);
    encdr.encode(b, d);
}