#include <iostream>
#include <fstream>
#include <vector>
#include <numbers>


struct header {
	uint32_t chunkID;
	uint32_t chunkSize;
	uint32_t format;
	uint32_t subChunk1ID;
	uint32_t subChunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	uint32_t subChunk2ID;
	uint32_t subChunk2Size;
};








uint32_t main() {

	float duration;
	float frequency;
	float amplitude;


	std::cout << "Duration : ";

	std::cin >> duration;



	std::cout << "Frequency : ";

	std::cin >> frequency;


	std::cout << "Amplitude : ";

	std::cin >> amplitude;




	header header_data;

	header_data.chunkID = 0x46464952;
	header_data.format = 0x45564157;
	header_data.subChunk1ID = 0x20746d66;
	header_data.subChunk1Size = 16;
	header_data.audioFormat = 1;
	header_data.numChannels = 2;
	header_data.sampleRate = 44100;
	header_data.bitsPerSample = 16;
	header_data.byteRate = header_data.sampleRate * header_data.numChannels * header_data.bitsPerSample / 8;
	header_data.blockAlign = header_data.numChannels * header_data.bitsPerSample / 8;
	header_data.subChunk2ID = 0x61746164;
	header_data.subChunk2Size = static_cast<uint32_t>(duration * header_data.sampleRate * header_data.numChannels * header_data.bitsPerSample / 8);
	header_data.chunkSize = 36 + header_data.subChunk2Size;


	std::vector<uint16_t> data(static_cast<size_t>(duration * header_data.sampleRate * header_data.numChannels));

	for (int i = 0; i < duration * header_data.sampleRate; i++) {
		uint16_t sample = static_cast<uint16_t>(amplitude * 32767 * sin(2 * std::numbers::pi * frequency * i / header_data.sampleRate));
		for (int j = 0; j < header_data.numChannels; j++) {
			data[i * header_data.numChannels + j] = sample;
		}
	}




	std::ofstream file("sound.wav", std::ios_base::binary);




	file.write(reinterpret_cast<char*>(&header_data), sizeof(header_data));

	file.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint16_t));



	file.close();






	
	system("sound.wav");


	return 0;
}