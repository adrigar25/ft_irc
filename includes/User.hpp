/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 00:08:12 by agarcia           #+#    #+#             */
/*   Updated: 2026/06/07 14:48:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <map>

class Channel;

class User
{
    private:
        int                                 socket;
        std::string                         nickname;
        std::string                         username;
        std::string                         realname;
        std::string                         inBuffer;
        std::string                         outBuffer;
        size_t                              outOffset;
        bool                                nickSet;
        bool                                userSet;
        bool                                passGiven;
        bool                                authenticated;
        std::map<std::string, Channel*>     channels;
    public:
                    User(int socket, const std::string &name);
                    ~User();
        bool        joinChannel(Channel *channel, const std::string &key);
        void        leaveChannel(Channel *channel);
        std::string getNickname() const;
        std::string getRealName() const;
        int         getSocket() const;
        const std::map<std::string, Channel*>& getChannels() const;
        
        // Authentication / identity helpers
        void setNickname(const std::string &name);
        void setUsername(const std::string &name);
        void setRealName(const std::string &name);
        void setPass(bool val);
        void setAuthenticated(bool val);
        std::string getUsername() const;
        bool isPassSet() const;
        bool isNickSet() const;
        bool isUserSet() const;
        bool isAuthenticated() const;

        /* Buffer accessors for I/O */
        std::string& getInBuffer();
        std::string& getOutBuffer();
        size_t&      getOutOffset();
        void         appendToOutBuffer(const std::string &data);
};

#endif
