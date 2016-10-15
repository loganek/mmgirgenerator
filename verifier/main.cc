#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> read_definitions(const std::string &filename)
{
  std::ifstream file(filename);

  std::vector<std::string> definitions;
  std::string line;
  std::string def;
  int p_ctr = 0;
  bool count = true;

  while (getline(file, line)) {
    for (auto c : line) {
      if (c == '(' && count) {
	p_ctr++;
      }
      if (c == ')' && count) {
	def += c;
	p_ctr--;
	if (p_ctr == 0) {
	  definitions.push_back(def);
	  def = "";
	}
      } else if (p_ctr > 0 && c != ' ') {
	def += c;
      }
      if (c == '"') {
	count = !count;
      }
    }
  }

  std::sort(definitions.begin(), definitions.end());

  return definitions;
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <defs_file_1> <defs_file_2>" << std::endl;
    return 1;
  }

  std::string defs_file_1 = argv[1];
  std::string defs_file_2 = argv[2];

  std::vector<std::string> definitions1 = read_definitions(defs_file_1);
  std::vector<std::string> definitions2 = read_definitions(defs_file_2);

  std::size_t i = 0, j = 0;
  while (i < definitions1.size() && j < definitions2.size()) {
    if (definitions1[i] == definitions2[j]) {
      i++; j++;
    }
    if (definitions1[i] < definitions2[j]) {
      std::cout << "From definitions1: " << definitions1[i++] << std::endl;
    } else if (definitions1[i] > definitions2[j]) {
      std::cout << "From definitions2: " << definitions2[j++] << std::endl;
    }
  }

  std::cout << "Operation completed!" << std::endl;

  return 0;
}
