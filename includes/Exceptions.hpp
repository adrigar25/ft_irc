/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@local>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 16:30:00 by copilot           #+#    #+#             */
/*   Updated: 2026/05/25 16:30:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

/*
 * Excepción unificada para errores del servidor IRC.
 * Se usa una sola clase con un código entero para distinguir tipos.
 * Compatible con C++98.
 */
enum IrcErrorCode {
    IRC_ERR_UNKNOWN = 0,
    IRC_ERR_USER_ALREADY_EXISTS = 1,
    IRC_ERR_CHANNEL_ALREADY_EXISTS = 2,
    IRC_ERR_USER_NOT_FOUND = 3,
    IRC_ERR_STARTING_SERVER = 10,
    IRC_ERR_ACCEPTING_CONNECTION = 11,
    IRC_ERR_SETTING_NONBLOCK = 12,
    IRC_ERR_SETTING_CLOEXEC = 13,
    IRC_ERR_CHANNEL_FULL = 20,
    IRC_ERR_CHANNEL_INVITE_ONLY = 21,
    IRC_ERR_USER_BANNED = 22,
    IRC_ERR_ROLE_NOT_FOUND = 23,
    IRC_ERR_USER_ALREADY_IN_CHANNEL = 24,
    IRC_ERR_CANNOT_JOIN_CHANNEL = 25,
    IRC_ERR_INCORRECT_CHANNEL_KEY = 26,
};

class IrcException : public std::runtime_error
{
    private:
        int code_;
    public:
        explicit IrcException(int code = IRC_ERR_UNKNOWN, const std::string &msg = "")
            : std::runtime_error(msg), code_(code) {}
        int getCode() const { return code_; }
        virtual ~IrcException() throw() {}
};

#endif
