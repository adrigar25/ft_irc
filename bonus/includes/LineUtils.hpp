/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LineUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 17:11:40 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/16 17:11:41 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_UTILS_H
#define LINE_UTILS_H

#include <string>
#include <vector>

void popLines(std::string &buffer, std::vector<std::string> &out);
std::string trim(const std::string &s, const std::string &chars);
std::vector<std::string> split(const std::string &s, char delimiter);

#endif
