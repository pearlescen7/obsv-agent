#pragma once

#include <map>
#include <string>
#include <functional>

#include <bpf/libbpf.h>

#include "crow.h"
#include "obsv_agent.skel.h"

struct prog_data_t {
    bool enabled{};             //Enable/disable status
    bpf_link* link_p{};         //Link pointer to attached program
    bpf_program* prog_p{};      //Program pointer loaded from bpf object

    //In case custom action needed for each program while enabling/disabling
    //Use something like this => std::function<...> enabler, disabler;
    //And remove prog_p possibly
};

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

    std::unordered_map<std::string, prog_data_t> prog_data_map;
};