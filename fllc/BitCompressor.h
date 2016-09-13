#pragma once
#include <queue>
#include <vector>

class BitCompressor
{
	class Writer
	{
	public:
		virtual int bufferSize() = 0;
		virtual bool canWrite(const unsigned char& data) = 0;
		virtual void write(const unsigned char& data) = 0;
		virtual void flush(const unsigned char& data) = 0;
		virtual size_t allocate(unsigned char* buffer, size_t size) = 0;
	};

	class ZerosOnesWriter :public Writer {

		struct BitInfo {
			const unsigned char* symbol;
			int bits;
		};

		std::vector<BitInfo> dataToWrite;

	public:
		static const unsigned char zeros;
		static const unsigned char transitionsZerosOnes[8];
		static const unsigned char ones;
		static const unsigned char transitionsOnesZeros[8];

		// Inherited via Writer
		virtual int bufferSize() override;
		virtual bool canWrite(const unsigned char & data) override;
		virtual void write(const unsigned char & data) override;
		virtual void flush(const unsigned char & data) override;
		virtual size_t allocate(unsigned char * buffer, size_t size) override;
	};

	class MixedWriter :public Writer {

		std::vector<unsigned char> dataToWrite;

		// Inherited via Writer
		virtual int bufferSize() override;
		virtual bool canWrite(const unsigned char & data) override;
		virtual void write(const unsigned char & data) override;
		virtual void flush(const unsigned char & data) override;
		virtual size_t allocate(unsigned char * buffer, size_t size) override;
	};

	std::queue<Writer*> writers;

	Writer* currentWriter;
	Writer* getWriter(const unsigned char* data, size_t length);

public:
	BitCompressor() :currentWriter(nullptr) {};

	void write(const unsigned char* data, size_t length);
	void flush();
	size_t bufferSize();
	void allocate(unsigned char* buffer, size_t bufferSize);
};

