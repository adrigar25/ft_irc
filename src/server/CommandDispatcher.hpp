/* ************************************************************************** */
/*                                                                            */
/*                                                    CommandDispatcher.hpp   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDDISPATCHER_HPP
#define COMMANDDISPATCHER_HPP

#include <map>
#include <string>

class ICommand;
struct RequestContext;

class CommandDispatcher {
public:
    CommandDispatcher();
    ~CommandDispatcher();
    void registerHandler(const std::string& cmd, ICommand* handler);
    bool hasHandler(const std::string& cmd) const;
    void dispatch(const std::string& cmd, RequestContext &ctx) const;
private:
    std::map<std::string, ICommand*> handlers;
};

#endif
