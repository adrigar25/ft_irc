/* ************************************************************************** */
/*                                                                            */
/*   CmdPing.hpp                                                             */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PING_HPP
#define CMD_PING_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPing : public ICommand {
public:
    CmdPing() {}
    virtual ~CmdPing() {}
    virtual void execute(RequestContext &ctx);
};

#endif
