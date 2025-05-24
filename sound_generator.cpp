#include <iostream>
#include <fstream>
#include <vector>
#include <numbers>
#include <random>
#include <sstream>
#include <map>


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



int pure_sine_wave(int duration, header header_data, std::vector<uint16_t> &data) {


	int frequency = 440;
	float amplitude = 0.5;

	std::string input;


	std::cout << "Frequency (440): ";

	std::getline(std::cin, input);

	if (!input.empty()) {
		std::istringstream iss(input);
		iss >> frequency;
	}


	std::cout << "Amplitude (0.5): ";

	std::getline(std::cin, input);

	if (!input.empty()) {
		std::istringstream iss(input);
		iss >> amplitude;
	}





	float amplitude_factor = amplitude * 32768;

	float pipi = 2 * std::numbers::pi;


	for (int i = 0; i < duration * header_data.sampleRate; i++) {

		double harmony = 0;


		harmony = sin(pipi * frequency * i / header_data.sampleRate);

		uint16_t sample = static_cast<uint16_t>(amplitude_factor * harmony);



		for (int j = 0; j < header_data.numChannels; j++) {

			data[i * header_data.numChannels + j] = sample;
		}
	}

	return 0;
}
















uint32_t main() {


	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution f_dist(0, 1);
	std::uniform_int_distribution a_dist(0, 1);





	int experiment_index = 0;
	int duration = 10;

	std::string input;






	std::cout << "Experiments menu :\n\n";

	std::cout << "0 : Pure sine wave\n";
	std::cout << "1 : White noise\n";
	std::cout << "2 : Fifth\n";
	std::cout << "3 : Octave\n";
	std::cout << "4 : Harmonics\n";






	std::cout << "Experiment (0): ";

	std::getline(std::cin, input);
	if (!input.empty()) {
		std::istringstream iss(input);
		iss >> experiment_index;
	}

	std::cout << "Duration (10): ";

	std::getline(std::cin, input);
	if (!input.empty()) {
		std::istringstream iss(input);
		iss >> duration;
	}






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







	std::map<int, int> experiments = {
		{0, pure_sine_wave(duration, header_data, data)}
	};










	std::ofstream file("sound.wav", std::ios_base::binary);




	file.write(reinterpret_cast<char*>(&header_data), sizeof(header_data));

	file.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint16_t));



	file.close();






	
	system("sound.wav");


	return 0;
}