/* ************************************************************************** */
/*                                                                            */
/*   CmdList.hpp                                                               */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_HPP
#define CMD_LIST_HPP

#include "commands/ICommand.hpp"
#include "../RequestContext.hpp"
#include <string>

class CmdList : public ICommand {
public:
    CmdList() {}
    virtual ~CmdList() {}
    virtual void execute(RequestContext &ctx);
};

#endif
