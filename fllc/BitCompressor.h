#pragma once
class BitCompressor
{
    class Writer
    {
        virtual void flush(const unsigned char& data) = 0;
        virtual void write(const unsigned char& data) = 0;
        virtual bool canWrite(const unsigned char& data) = 0;
    };

    class ZerosWriter :Writer {
        const unsigned char zero = '0x00';

        // Inherited via Writer
        virtual void flush(const unsigned char & data) override;
        virtual void write(const unsigned char & data) override;
        virtual bool canWrite(const unsigned char & data) override;
    };
    class OnesWriter :Writer {
        const unsigned char ones = '0xFF';
        int totalBits;

        OnesWriter() : totalBits(0) {}

        // Inherited via Writer
        virtual void flush(const unsigned char & data) override;
        virtual void write(const unsigned char & data) override;
        virtual bool canWrite(const unsigned char & data) override;
    };
    class MixedWriter :Writer {
        // Inherited via Writer
        virtual void flush(const unsigned char & data) override;
        virtual void write(const unsigned char & data) override;
        virtual bool canWrite(const unsigned char & data) override;
    };

    Writer* currentWriter;

public:
    BitCompressor() :currentWriter(nullptr) {};

    void write(const unsigned char* data, size_t length);
    size_t bufferSize();
    void allocate(unsigned char* buffer, size_t bufferSize);
};

