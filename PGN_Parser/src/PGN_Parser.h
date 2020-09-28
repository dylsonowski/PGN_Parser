#pragma once
#include <array>
#include <string>
#include <vector>
#include <utility>

namespace Pale {

	class PGN_Parser {

	public:
		PGN_Parser() = default;
		~PGN_Parser() = default;

		int AddFile(std::string filePatch);
		void RemoveFile(std::string fileName);
		void SetOutputFile(std::string file) { _outputFile = file; }
		inline int GetFilesCount() const { return _filesToParse.size(); }
		void Parse(int examplePerFile);
		void Clean();
		void DisplayParsedGames() const;
		void ChangeForPossitions();
		void CreateBoardReprezentation();
		void ParseToFiles(int examplePerFile);

	private:
		std::array<std::array<const char*, 8>, 8> _boardReprezentation;
		std::vector<std::string> _parsedGames;
		std::vector<std::pair<std::array<std::array<const char*, 8>, 8>, bool>> _parsedPossitions;
		std::vector<std::string> _filesToParse;
		std::string _outputFile;
	};
}

