#pragma once
#include <queue>
#include <vector>

class BitCompressor
{
    class Writer
    {
    public:
        virtual bool canWrite(const unsigned char& data) = 0;
        virtual void write(const unsigned char& data) = 0;
        virtual void flush(const unsigned char& data) = 0;
    };

    class ZerosOnesWriter :public Writer {

        struct BitInfo {
            const unsigned char symbol;
            int bits;
        };

        std::vector<BitInfo> dataToWrite;

    public:
        static const unsigned char zeros;
        static const unsigned char transitionsZerosOnes[8];
        static const unsigned char ones;
        static const unsigned char transitionsOnesZeros[8];

        // Inherited via Writer
        virtual bool canWrite(const unsigned char & data) override;
        virtual void write(const unsigned char & data) override;
        virtual void flush(const unsigned char & data) override;
    };

    class MixedWriter :public Writer {
        // Inherited via Writer
        virtual bool canWrite(const unsigned char & data) override;
        virtual void write(const unsigned char & data) override;
        virtual void flush(const unsigned char & data) override;
    };

    std::queue<Writer*> writers;

    Writer* currentWriter;
    Writer* getWriter(const unsigned char* data, size_t length);

public:
    BitCompressor() :currentWriter(nullptr) {};

    void write(const unsigned char* data, size_t length);
    size_t bufferSize();
    void allocate(unsigned char* buffer, size_t bufferSize);
};

