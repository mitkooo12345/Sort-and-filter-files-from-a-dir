#include <windows.h>
#include <stack>
#include <assert.h>
#include "Array.h"

struct FileIterator {
private:
	std::stack<std::string> pathStack;
	std::stack<HANDLE> hFindStack;
	std::stack<WIN32_FIND_DATA*> ffdStack;

public:
	FileIterator(std::string root) {
		std::string pattern(root + "\\*");
		WIN32_FIND_DATA* ffd = new WIN32_FIND_DATA;
		HANDLE hFind = FindFirstFile(pattern.c_str(), ffd);

		ffdStack.push(ffd);
		pathStack.push(root);
		hFindStack.push(hFind);
	}

	FileIterator() {}

public:
	std::tuple<std::string,unsigned int> next() {
		assert(pathStack.size() == hFindStack.size());
		assert(pathStack.size() == ffdStack.size());
		if (pathStack.empty()) {
			return std::make_tuple(".",0);
		}

		std::string path = pathStack.top();
		HANDLE hFind = hFindStack.top();
		WIN32_FIND_DATA* ffd = ffdStack.top();

		if (hFind == INVALID_HANDLE_VALUE) {
			pathStack.pop();
			hFindStack.pop();
			delete ffdStack.top();
			ffdStack.pop();
			return next();
		}

		WIN32_FIND_DATA* nextF = new WIN32_FIND_DATA;
		bool result = FindNextFile(hFindStack.top(), nextF);
		ffdStack.top() = nextF;
		if (!result) {
			hFindStack.top() = INVALID_HANDLE_VALUE;
		}

		if (!(ffd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			//file
			unsigned int size = ffd->nFileSizeLow;
			std::string fullPath = path + "\\" + ffd->cFileName;
			delete ffd;
			return std::make_tuple(fullPath , size);
		}

		// directory
		std::string folderName = ffd->cFileName;

		delete ffd;

		if (folderName != "." && folderName != "..") {
			WIN32_FIND_DATA* firstItemForNewDir = new WIN32_FIND_DATA;

			std::string pattern(path + "\\" + folderName + "\\*");
			hFind = FindFirstFile(pattern.c_str(), firstItemForNewDir);
			ffdStack.push(firstItemForNewDir);
			pathStack.push(path + "\\" + folderName);
			hFindStack.push(hFind);

			return next();
		}
		//skip "." & ".." folders
		return next();
	}
};

struct FilterIterator {
private:
	FileIterator fi;
	std::string filterString;
	std::tuple<std::string, unsigned int> fileInfo;

public:
	FilterIterator(std::string path, std::string filter) {
		fi = FileIterator(path);
		filterString = filter;
	}

	std::tuple<std::string, unsigned int> next() {
		fileInfo = fi.next();

		std::string fName = std::get<0>(fileInfo);
		if ((fName.find(filterString) != std::string::npos) || fName == ".") {
			return fileInfo;
		} else {
			return next();
		}
	}
};

void printSubDirs(std::string path) {
	std::string pattern(path);
	pattern = pattern + "\\*";

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(pattern.c_str(), &ffd);

	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			std::string name = ffd.cFileName;

			if (name != "." && name != "..") {
				std::string newPath(path);
				newPath = newPath + "\\" + name;

				printSubDirs(newPath);
			}

		} else {
			std::cout << ffd.cFileName << ", " << ffd.nFileSizeLow << " byte(s)\n";
		}

	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

int main() {
	std::string userPath;
	std::string filter;

	std::cout << "enter path:\n";
	std::cin >> userPath;
	std::cout << "enter suffix filter(example: .pdf):\n";
	std::cin >> filter;
	//char userPath[280] = "C:\\Users\\mitko\\Documents";

	FilterIterator i(userPath, filter);

	std::tuple<std::string, unsigned int> fileInfo;
	fileInfo = i.next();
	
	Array arr;

	std::string oldFile = "o.txt";
	std::string newFile = "n.txt";

	//cleanup temp data
	std::fstream a(oldFile, std::ios::out);
	a.close();

	while (std::get<0>(fileInfo) != ".") {
		arr.add(fileInfo);
		if (arr.getSize() == 10000) {
			arr.mergeAndSort(oldFile, newFile);
			remove(oldFile.c_str());
			rename(newFile.c_str(), oldFile.c_str());

			arr.clear();
		}

		fileInfo = i.next();
	}

	if (arr.getSize() > 0) {

		arr.mergeAndSort(oldFile, newFile);
		remove(oldFile.c_str());
		rename(newFile.c_str(), oldFile.c_str());

		arr.clear();
	}

	std::ifstream result(oldFile);
	MFile mFile;
	while (!result.eof()) {
		mFile = MFile(result);
		if(mFile.getPath() != "")
		std::cout << mFile;
	}

	result.close();
	remove(oldFile.c_str());

	return 0;
}