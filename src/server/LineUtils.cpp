#include "LineUtils.hpp"

#include <string>
#include <vector>

void popLines(std::string &buffer, std::vector<std::string> &out)
{
    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        std::string line = buffer.substr(0, pos);
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        out.push_back(line);
        buffer.erase(0, pos + 1);
    }
}

std::string trimParam(const std::string &s)
{
    std::string r = s;
    // remove a single leading space if present
    if (!r.empty() && r[0] == ' ') r.erase(0,1);
    // remove trailing CR and spaces
    while (!r.empty() && (r[r.size() - 1] == '\r' || r[r.size() - 1] == ' '))
        r.erase(r.size() - 1, 1);
    return r;
}
