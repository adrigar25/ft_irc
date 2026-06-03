/* ************************************************************************** */
/*                                                                            */
/*   CmdPart.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PART_HPP
#define CMD_PART_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdPart : public ICommand {
public:
    CmdPart() {}
    virtual ~CmdPart() {}
    virtual void execute(RequestContext &ctx);
};

#endif
