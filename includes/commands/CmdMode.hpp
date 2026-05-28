/* ************************************************************************** */
/*                                                                            */
/*   CmdMode.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_MODE_HPP
#define CMD_MODE_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdMode : public ICommand {
public:
    CmdMode() {}
    virtual ~CmdMode() {}
    virtual void execute(RequestContext &ctx);
};

#endif
