#include "MFile.h"

MFile::MFile() {}

MFile::MFile(std::string path, int size) {
	filePath = path;
	fileSize = size;
}

MFile::MFile(std::ifstream& file) {

	std::getline(file, filePath);

	std::string fSizeString;
	std::getline(file, fSizeString);
	if (fSizeString != "") {
		fileSize = std::stoi(fSizeString);
	} else {
		fileSize = -1;
	}
}

MFile::~MFile() {
}

bool MFile::operator<(const MFile& file) {
	if (fileSize < file.getSize()) {
		return true;

	} else if (fileSize == file.getSize()) {
		if (filePath < file.getPath()) {
			return true;
		} else {
			return false;
		}

	} else {
		return false;
	}
}

std::ofstream& operator<<(std::ofstream& os, const MFile file) {

	os << file.getPath() << std::endl << file.getSize() << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const MFile file) {

	os << file.getPath() << ", " << file.getSize() << " byte(s)" << std::endl;
	return os;
}

std::string MFile::getPath() const {
	return filePath;
}

int MFile::getSize() const {
	return fileSize;
}
