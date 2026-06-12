/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LineUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:38:31 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/09 17:14:09 by agarcia          ###   ########.fr       */
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
        if (!line.empty())
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
    std::string token;

    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == delim)
        {
            if (!token.empty())
            {
                res.push_back(token);
                token.clear();
            }
        }
        else
            token += s[i];
    }
    
    if (!token.empty())
        res.push_back(token);

    return res;
}
