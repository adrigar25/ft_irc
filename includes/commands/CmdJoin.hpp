/* ************************************************************************** */
/*                                                                            */
/*   CmdJoin.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_JOIN_HPP
#define CMD_JOIN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdJoin : public ICommand {
public:
    CmdJoin() {}
    virtual ~CmdJoin() {}
    virtual void execute(RequestContext &ctx);
};

#endif
