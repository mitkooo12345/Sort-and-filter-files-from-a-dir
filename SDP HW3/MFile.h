#pragma once
#include <string>
#include <fstream>

class MFile {
public:
	MFile();
	MFile(std::string, int);
	MFile(std::ifstream&);
	bool operator<(const MFile&);

	~MFile();

public:
	std::string getPath() const;
	int getSize() const;

private:
	std::string filePath;
	int fileSize;
};

std::ofstream& operator<<(std::ofstream& os, const MFile file);
std::ostream& operator<<(std::ostream& os, const MFile file);
