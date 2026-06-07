/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LineUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:36:30 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 12:36:31 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_UTILS_H
#define LINE_UTILS_H

#include <string>
#include <vector>

// Extract complete lines (ending with '\n') from `buffer`.
// For each extracted line the terminating CR (if present) is removed.
// The extracted lines are appended to `out` and removed from the front of `buffer`.
void popLines(std::string &buffer, std::vector<std::string> &out);

// Trim a command parameter: remove a single leading space (if present)
// and any trailing spaces or trailing CR ('\r').
std::string trimParam(const std::string &s);

#endif
