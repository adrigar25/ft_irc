/* ************************************************************************** */
/*                                                                            */
/*   CmdUnknown.hpp                                                                */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_UNKNOWN_HPP
#define CMD_UNKNOWN_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdUnknown : public ICommand {
public:
    CmdUnknown() {}
    virtual ~CmdUnknown() {}
    virtual void execute(RequestContext &ctx);
};

#endif
