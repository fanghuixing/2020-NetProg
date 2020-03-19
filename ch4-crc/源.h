#pragma once

void ProcessDestinationAddr(std::fstream& outfile);

void MoveNewBitToCrc(unsigned int& crc, char temp, unsigned char i);

void CalculateCRCValue(int total, std::fstream& outfile, unsigned int& crc);

void ProcessSourceAddr(std::fstream& outfile);

void ProcessLengthAndData(std::fstream& outfile);
