#include "obsv_agent.hpp"
#include "utils.hpp"

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
        utils::log_error("Failed to open/load BPF object");
        return;
    }

    //Setup program data
    prog_data_map = {
        {"sched_switch", {.prog_p = obj_p->progs.sched_switch}},
    };
    
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


//TODO: this function currently only enables/disables program
// has nothing to do with other configuration if any configuration gets added to the agent
void ObsvAgent::set_config(const crow::json::rvalue& new_config) {

    //Iterate through JSON and enable/disable necessary progs
    for(auto it = new_config.begin(); it != new_config.end(); it++) {
        if(auto prog_it = prog_data_map.find(it->key()); prog_it != prog_data_map.end())
        {
            auto [prog_name, prog_data] = *prog_it;
            if(it->b()) { 
                //Continue if already enabled
                if(prog_data.link_p) {
                    CROW_LOG_INFO << prog_name + " ALREADY enabled";
                    continue;
                }

                //Enable program
                prog_data.link_p = bpf_program__attach(prog_data.prog_p);
                if(!prog_data.link_p){
                    utils::log_error("Failed to enable program " + prog_name);
                    //TODO: can improve error handling -> maybe send something to client?
                    continue;
                }
                prog_data.enabled = true;
                CROW_LOG_INFO << prog_name + " enabled";
            } else { 
                //Continue if already disabled
                if(!prog_data.link_p) {
                    CROW_LOG_INFO << prog_name + " ALREADY disabled";
                    continue;
                }

                //Disable program
                if(bpf_link__destroy(prog_data.link_p)) {
                    utils::log_error("Failed to disable program " + prog_name);
                    //TODO: can improve error handling -> maybe send something to client?
                    continue;
                }
                prog_data.link_p = nullptr;
                prog_data.enabled = false;
                CROW_LOG_INFO << prog_name + " disabled";
            }
        }

    }
}