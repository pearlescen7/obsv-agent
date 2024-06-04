#include <map>
#include <string>


#include <bpf/libbpf.h>

#include "crow.h"
#include "obsv_agent.skel.h"


class ObsvAgent {
public:
    ObsvAgent();
    ~ObsvAgent();

    void run(std::uint16_t port = 5000, bool multithreaded = false);

private:
    crow::response handle_set_config(const crow::request&);
    crow::response handle_get_config();

    void set_config(const crow::json::rvalue&);
    //crow::json::wvalue get_config();

    obsv_agent_bpf* obj_p;
    crow::SimpleApp app;

    // std::map<std::string, bpf_link*> links;
};

// auto a = { 
//     {"sched_switch"} : {false, link_ptr},
//     {}
// }