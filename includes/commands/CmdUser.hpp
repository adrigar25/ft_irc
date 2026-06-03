/* ************************************************************************** */
/*                                                                            */
/*   CmdUser.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_USER_HPP
#define CMD_USER_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUser : public ICommand {
public:
    CmdUser() {}
    virtual ~CmdUser() {}
    virtual void execute(RequestContext &ctx);
};

#endif
