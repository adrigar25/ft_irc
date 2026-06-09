/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LineUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:31 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:19:46 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LineUtils.hpp"

#include <string>
#include <vector>
#include <sstream>

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

std::string trim(const std::string &s, const std::string &chars)
{
    std::string r = s;
    for (size_t i = 0; i < chars.size(); ++i) {
        char c = chars[i];
        while (!r.empty() && r[0] == c)
            r.erase(0, 1);
        while (!r.empty() && r[r.size() - 1] == c)
            r.erase(r.size() - 1, 1);
    }
    return r;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> res;
    std::string tmp;
    std::stringstream ss(s);

    while (std::getline(ss, tmp, delim))
        res.push_back(tmp);
    return res;
}
