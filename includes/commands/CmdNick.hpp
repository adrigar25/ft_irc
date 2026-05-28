/* ************************************************************************** */
/*                                                                            */
/*   CmdNick.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_NICK_HPP
#define CMD_NICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdNick : public ICommand {
public:
    CmdNick() {}
    virtual ~CmdNick() {}
    virtual void execute(RequestContext &ctx);
};

#endif
