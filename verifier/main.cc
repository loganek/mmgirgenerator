#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

void trim(std::string& str)
{
    auto it = std::find_if_not(str.begin(), str.end(), ::isspace);
    if (it != str.end()) str.erase(str.begin(), it);

    it = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    if (it != str.end()) str.erase(it, str.end());
}

void remove_param_name(std::string& name)
{
    auto p1 = name.find_last_of('"');
    auto p2 = name.find_last_of('"', p1-1);

    name = name.erase(p2, p1-p2+1);
}

std::vector<std::string> read_definitions(const std::string &filename, bool ignore_param_name = true)
{
    std::ifstream file(filename);

    std::vector<std::string> definitions;
    std::string line;
    std::string def;
    int p_ctr = 0;

    bool parameters = false;;

    while (getline(file, line))
    {
        trim(line);
        if (line.empty()) continue;

        if (!p_ctr) def = "";

        bool front_p = line.front() == '(' || (line.size() > 1 && (line[0] == '\'' && line[1] == '('));
        bool back_p = line.back() == ')';

        if (parameters && back_p && !front_p)
            parameters = false;

        if (parameters && ignore_param_name)
            remove_param_name(line);


        if (line == "(parameters")
            parameters = true;


        p_ctr += front_p;
        p_ctr -= back_p;

        def += line;

        if (!p_ctr) definitions.push_back(def);
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
