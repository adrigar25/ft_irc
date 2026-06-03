/* ************************************************************************** */
/*                                                                            */
/*   CmdKick.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_KICK_HPP
#define CMD_KICK_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdKick : public ICommand {
public:
    CmdKick() {}
    virtual ~CmdKick() {}
    virtual void execute(RequestContext &ctx);
};

#endif
