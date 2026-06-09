/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LineUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:30 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/08 21:20:04 by agarcia          ###   ########.fr       */
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
