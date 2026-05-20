#include "line_utils.h"

#include <string>

void popLines(std::string &buffer, std::vector<std::string> &out)
{
    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        std::string line = buffer.substr(0, pos);
        if (!line.empty() && line.back() == '\r')
            line.erase(line.size() - 1);
        out.push_back(line);
        buffer.erase(0, pos + 1);
    }
}
