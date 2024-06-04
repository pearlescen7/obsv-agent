#include "obsv_agent.hpp"
#include "utils.h"

ObsvAgent::ObsvAgent() 
{

    //Setup routes
    CROW_ROUTE(app, "/").methods("GET"_method) ([&](){ return handle_get_config(); });
    CROW_ROUTE(app, "/").methods("POST"_method) ([&](const crow::request& req){ return handle_set_config(req); });

}

ObsvAgent::~ObsvAgent()
{
    //TODO: detach links, destroy skel etc.

    if(obj_p) {
        obsv_agent_bpf__destroy(obj_p);
    }
    app.stop();
}


void ObsvAgent::run(std::uint16_t port, bool multithreaded) {
    //Load BPF object
    obj_p = obsv_agent_bpf__open_and_load();

    if(!obj_p) {
        log_error("Failed to open/load BPF object");
        return;
    }

    //Start REST application
    if(multithreaded) {
        app.port(port).multithreaded().run();
    } else {
        app.port(port).run();
    }
}

//TODO: this should return the current observability configuration
crow::response ObsvAgent::handle_get_config() {
    return crow::response{200};
}

//TODO: this should change the configuration to the one in the request
crow::response ObsvAgent::handle_set_config(const crow::request& req) {
    auto new_config = crow::json::load(req.body);

    if (!new_config)
    {
        return crow::response(400, "Invalid JSON configuration provided.");
    }

    set_config(new_config);

    // sched_link = bpf_program__attach(obj_p->progs.sched_switch);

    // std::string example_field = new_config["example"].s();

    // CROW_LOG_DEBUG << example_field;

    // crow::json::wvalue response;
    // response["received"] = example_field;

    // return crow::response{200, response};

    return crow::response{200};
}

void ObsvAgent::set_config(const crow::json::rvalue& new_config) {
    //TODO: Iterate through JSON and enable/disable necessary progs
    for(auto it = new_config.begin(); it != new_config.end(); it++) {
    }
}