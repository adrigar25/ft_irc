/* ************************************************************************** */
/*                                                                            */
/*   CmdQuit.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_QUIT_HPP
#define CMD_QUIT_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdQuit : public ICommand {
public:
    CmdQuit() {}
    virtual ~CmdQuit() {}
    virtual void execute(RequestContext &ctx);
};

#endif
