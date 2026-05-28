/* ************************************************************************** */
/*                                                                            */
/*   CmdCap.hpp                                                                */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_CAP_HPP
#define CMD_CAP_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdCap : public ICommand {
public:
    CmdCap() {}
    virtual ~CmdCap() {}
    virtual void execute(RequestContext &ctx);
};

#endif
