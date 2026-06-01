/* ************************************************************************** */
/*                                                                            */
/*   CmdCap.hpp                                                                */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_TOPIC_HPP
#define CMD_TOPIC_HPP

#include "commands/ICommand.hpp"
#include "RequestContext.hpp"
#include <string>

class CmdTopic : public ICommand {
public:
    CmdTopic() {}
    virtual ~CmdTopic() {}
    virtual void execute(RequestContext &ctx);
};

#endif
