/* ************************************************************************** */
/*                                                                            */
/*   CmdPass.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PASS_HPP
#define CMD_PASS_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPass : public ICommand {
public:
    CmdPass() {}
    virtual ~CmdPass() {}
    virtual void execute(RequestContext &ctx);
};

#endif
