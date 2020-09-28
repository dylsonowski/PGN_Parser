#include "PGN_Parser.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>

namespace Pale {

	int PGN_Parser::AddFile(std::string filePatch) {

		_filesToParse.push_back(filePatch);

		return _filesToParse.size();
	}

	void PGN_Parser::RemoveFile(std::string fileName) {

		const auto it = std::remove_if(_filesToParse.begin(), _filesToParse.end(), [&](const std::string& str) {
			return str.find(fileName); });

		_filesToParse.erase(it, _filesToParse.end());
	}

	void PGN_Parser::Parse(int examplePerFile) {

		Clean();
		ChangeForPossitions();
		ParseToFiles(examplePerFile);
	}

	void PGN_Parser::Clean() {

		if (!_filesToParse.empty()) {

			std::ifstream in_File;
			for (auto iterator : _filesToParse) {

				in_File.open(iterator);

				if (!in_File.good())
					printf("Zjebales!!!!!"); //TODO: Delete later

				std::string line;
				while (std::getline(in_File, line)) {

					if (line.find("1. ") != std::string::npos) {

						printf("New match spotted!\n");
						std::string moves = line;

						int it = 1;
						while (true) {

							std::string sequenceToFind = std::to_string(it) + ".";
							auto pos = moves.find(sequenceToFind);
							if (pos != std::string::npos) {

								if (it < 10)
									moves.erase(pos, 3);
								else
									moves.erase(pos, 4);

								it++;
							}
							else {

								auto pos2 = moves.find("{");
								if (pos2 != std::string::npos)
									moves.erase(pos2, moves.find("}") + 1);

								pos = moves.find("1-0");
								pos2 = moves.find("0-1");
								if (pos != std::string::npos)
									moves.erase(pos, 3);
								else if (pos2 != std::string::npos)
									moves.erase(pos2, 3);

								break;
							}
						}

						_parsedGames.emplace_back(moves);
					}
				}

				in_File.close();
			}
		}
		else
			printf("There is no file to parse!. Try to add one first.\n");
	}

	void PGN_Parser::DisplayParsedGames() const {

		for (auto it : _parsedGames) {
			std::cout << it << "END\n\n\n";
		}
	}

	void PGN_Parser::ChangeForPossitions() {

		bool betterForBlack;
		for (auto it : _parsedGames) {

			CreateBoardReprezentation();
			int turn = 0;

			int number = 1;
			while (true) {
				
				std::string move = it.substr(0, it.find(" "));

				if (!move.empty()) {

					if (move[move.size() - 1] == '+' || move[move.size() - 1] == '#')
						move.pop_back();

					if (move == "O-O") {

						if (turn % 2 == 0) {

							_boardReprezentation[7][6] = _boardReprezentation[7][4];
							_boardReprezentation[7][4] = "x";
							_boardReprezentation[7][5] = _boardReprezentation[7][7];
							_boardReprezentation[7][7] = "x";
						}
						else {

							_boardReprezentation[0][6] = _boardReprezentation[0][4];
							_boardReprezentation[0][4] = "x";
							_boardReprezentation[0][5] = _boardReprezentation[0][7];
							_boardReprezentation[0][7] = "x";
						}
					}
					else if (move == "O-O-O") {

						if (turn % 2 == 0) {

							_boardReprezentation[7][2] = _boardReprezentation[7][4];
							_boardReprezentation[7][4] = "x";
							_boardReprezentation[7][3] = _boardReprezentation[7][0];
							_boardReprezentation[7][0] = "x";
						}
						else {

							_boardReprezentation[0][2] = _boardReprezentation[0][4];
							_boardReprezentation[0][4] = "x";
							_boardReprezentation[0][3] = _boardReprezentation[0][0];
							_boardReprezentation[0][0] = "x";
						}
					}
					else {

						int y_Cord = 0, x_Cord = 0;
						switch (move[move.size() - 2]) {

						case 'a': x_Cord = 0; break;
						case 'b': x_Cord = 1; break;
						case 'c': x_Cord = 2; break;
						case 'd': x_Cord = 3; break;
						case 'e': x_Cord = 4; break;
						case 'f': x_Cord = 5; break;
						case 'g': x_Cord = 6; break;
						case 'h': x_Cord = 7; break;
						}

						switch (move[move.size() - 1]) {

						case '1': y_Cord = 7; break;
						case '2': y_Cord = 6; break;
						case '3': y_Cord = 5; break;
						case '4': y_Cord = 4; break;
						case '5': y_Cord = 3; break;
						case '6': y_Cord = 2; break;
						case '7': y_Cord = 1; break;
						case '8': y_Cord = 0; break;
						}

						if (move[0] == 'K' || move[0] == 'Q' || move[0] == 'B' || move[0] == 'N' || move[0] == 'R') {

							for (int i = 0; i < 8; i++) {

								for (int j = 0; j < 8; j++) {

									if (move.size() > 4 && move.find("x") != std::string::npos) {

										int x_Pos_Cord = 0;
										switch (move[1]) {

										case 'a': x_Pos_Cord = 0; break;
										case 'b': x_Pos_Cord = 1; break;
										case 'c': x_Pos_Cord = 2; break;
										case 'd': x_Pos_Cord = 3; break;
										case 'e': x_Pos_Cord = 4; break;
										case 'f': x_Pos_Cord = 5; break;
										case 'g': x_Pos_Cord = 6; break;
										case 'h': x_Pos_Cord = 7; break;
										}

										if (turn % 2 == 0) {

											switch (move[0]) {

											case 'B':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wB") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'N':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wN") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'R':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wR") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											}
										}
										else {

											switch (move[0]) {

											case 'B':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bB") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'N':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bN") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'R':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bR") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											}
										}
									}
									else if (move.size() > 3 && move.find("x") == std::string::npos) {

										int x_Pos_Cord = 0;
										switch (move[1]) {

										case 'a': x_Pos_Cord = 0; break;
										case 'b': x_Pos_Cord = 1; break;
										case 'c': x_Pos_Cord = 2; break;
										case 'd': x_Pos_Cord = 3; break;
										case 'e': x_Pos_Cord = 4; break;
										case 'f': x_Pos_Cord = 5; break;
										case 'g': x_Pos_Cord = 6; break;
										case 'h': x_Pos_Cord = 7; break;
										}

										if (turn % 2 == 0) {

											switch (move[0]) {

											case 'B':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wB") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'N':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wN") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'R':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "wR") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											}
										}
										else {

											switch (move[0]) {

											case 'B':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bB") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'N':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bN") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'R':

												if (j == x_Pos_Cord && _boardReprezentation[i][j] == "bR") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											}
										}
									}
									else {

										if (turn % 2 == 0) {

											switch (move[0]) {

											case 'K':

												if (_boardReprezentation[i][j] == "wK") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'Q':

												if (_boardReprezentation[i][j] == "wQ") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'B':

												if (_boardReprezentation[i][j] == "wB") {

													if (abs(y_Cord - i) == abs(x_Cord - j)) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											case 'N':

												if (_boardReprezentation[i][j] == "wN") {

													if ((abs(y_Cord - i) == 1 && abs(x_Cord - j) == 2) || (abs(y_Cord - i) == 2 && abs(x_Cord - j) == 1)) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											case 'R':

												if (_boardReprezentation[i][j] == "wR") {

													if ((y_Cord - i) == 0 || (x_Cord - j) == 0) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											}
										}
										else {

											switch (move[0]) {

											case 'K':

												if (_boardReprezentation[i][j] == "bK") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'Q':

												if (_boardReprezentation[i][j] == "bQ") {

													_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
													_boardReprezentation[i][j] = "x";
													goto stop;
												}
												break;
											case 'B':

												if (_boardReprezentation[i][j] == "bB") {

													if (abs(y_Cord - i) == abs(x_Cord - j)) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											case 'N':

												if (_boardReprezentation[i][j] == "bN") {

													if ((abs(y_Cord - i) == 1 && abs(x_Cord - j) == 2) || (abs(y_Cord - i) == 2 && abs(x_Cord - j) == 1)) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											case 'R':

												if (_boardReprezentation[i][j] == "bR") {

													if ((y_Cord - i) == 0 || (x_Cord - j) == 0) {

														_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[i][j];
														_boardReprezentation[i][j] = "x";
														goto stop;
													}
												}
												break;
											}
										}
									}
								}
							}
						}
						else {

							if (move[1] != 'x') {

								if (turn % 2 == 0) {

									if (_boardReprezentation[y_Cord + 1][x_Cord] == "wP") {

										_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord + 1][x_Cord];
										_boardReprezentation[y_Cord + 1][x_Cord] = "x";
									}
									else if (_boardReprezentation[y_Cord + 2][x_Cord] == "wP") {

										_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord + 2][x_Cord];
										_boardReprezentation[y_Cord + 2][x_Cord] = "x";
									}
								}
								else {

									if (_boardReprezentation[y_Cord - 1][x_Cord] == "bP") {

										_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord - 1][x_Cord];
										_boardReprezentation[y_Cord - 1][x_Cord] = "x";
									}
									else if (_boardReprezentation[y_Cord - 2][x_Cord] == "bP") {

										_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord - 2][x_Cord];
										_boardReprezentation[y_Cord - 2][x_Cord] = "x";
									}
								}
							}
							else {

								int x_Pos_Cord;
								switch (move[0]) {

								case 'a': x_Pos_Cord = 0; break;
								case 'b': x_Pos_Cord = 1; break;
								case 'c': x_Pos_Cord = 2; break;
								case 'd': x_Pos_Cord = 3; break;
								case 'e': x_Pos_Cord = 4; break;
								case 'f': x_Pos_Cord = 5; break;
								case 'g': x_Pos_Cord = 6; break;
								case 'h': x_Pos_Cord = 7; break;
								}

								if (turn % 2 == 0) {

									_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord + 1][x_Pos_Cord];
									_boardReprezentation[y_Cord + 1][x_Pos_Cord] = "x";
								}
								else {

									_boardReprezentation[y_Cord][x_Cord] = _boardReprezentation[y_Cord - 1][x_Pos_Cord];
									_boardReprezentation[y_Cord - 1][x_Pos_Cord] = "x";
								}
							}
						}
					}

					stop:
					it.erase(0, it.find(" ") + 1);

					if (turn % 2 == 0)
						betterForBlack = false;
					else
						betterForBlack = true;

					turn++;
				}
				else { break; }

				_parsedPossitions.push_back(std::make_pair(_boardReprezentation, betterForBlack));
				number++;
			}
		}
	}

	void PGN_Parser::CreateBoardReprezentation() {

		for (int i = 0; i < 8; i++) {

			for (int j = 0; j < 8; j++) {

				if (i == 1)
					_boardReprezentation[i][j] = "bP";
				else if (i == 6)
					_boardReprezentation[i][j] = "wP";
				else
					_boardReprezentation[i][j] = "x";
			}
		}

		//Placeing black pieces
		_boardReprezentation[0][0] = _boardReprezentation[0][7] = "bR";
		_boardReprezentation[0][1] = _boardReprezentation[0][6] = "bN";
		_boardReprezentation[0][2] = _boardReprezentation[0][5] = "bB";
		_boardReprezentation[0][3] = "bQ";
		_boardReprezentation[0][4] = "bK";

		//Placeing white pieces
		_boardReprezentation[7][0] = _boardReprezentation[7][7] = "wR";
		_boardReprezentation[7][1] = _boardReprezentation[7][6] = "wN";
		_boardReprezentation[7][2] = _boardReprezentation[7][5] = "wB";
		_boardReprezentation[7][3] = "wQ";
		_boardReprezentation[7][4] = "wK";
	}

	void PGN_Parser::ParseToFiles(int examplePerFile) {

		if (examplePerFile != 0) {
			std::ofstream out_File;
			int vector_It = _parsedPossitions.size() - 1;

			std::random_shuffle(_parsedPossitions.begin(), _parsedPossitions.end());
			std::cout << "\n\nPozitions count: " << _parsedPossitions.size();
			int filesCount = _parsedPossitions.size() / examplePerFile;

			if (_parsedPossitions.size() % examplePerFile != 0)
				filesCount++;

			for (int i = 0; i < filesCount; i++) {

				std::string file_name = "Parsed\\Test_Example_";
				file_name += std::to_string(i) += ".txt";
				out_File.open(file_name);

				for (int j = 0; j < examplePerFile; j++) {

					for (int x = 0; x < 8; x++) {

						for (int y = 0; y < 8; y++) {

							out_File << _parsedPossitions[vector_It].first[x][y] << " ";
						}
					}
					out_File << "| " << _parsedPossitions[vector_It].second;

					_parsedPossitions.pop_back();
					vector_It--;
					out_File << "\n\n";

					if (_parsedPossitions.empty())
						break;
				}

				out_File.close();
				file_name.clear();
			}
		}
		else
			std::cout << "ERROR: Number of examples cannot be equal to 0!\n Try again!\n";
	}
}