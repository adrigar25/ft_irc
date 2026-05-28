/* ************************************************************************** */
/*                                                                            */
/*   CmdInvite.hpp                                                             */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_INVITE_HPP
#define CMD_INVITE_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdInvite : public ICommand {
public:
    CmdInvite() {}
    virtual ~CmdInvite() {}
    virtual void execute(RequestContext &ctx);
};

#endif
