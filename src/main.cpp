#include <argp.h>

#include "utils.h"
#include "obsv_agent.hpp"

static struct env {
    std::uint16_t port;
    bool multithreaded;
} env;

const char *argp_program_version = "obsv_agent 0.0.1";
const char *argp_program_bug_address = "<uzayuysal18@gmail.com>";
const char argp_program_doc[] = "Adaptive observability agent built with eBPF.\n"
				"\n"
				"This agent provides a REST interface to dynamically "
                "enable/disable observability.  \n"
				"\n"
				"USAGE: ./obsv_agent [-p <port>] [-m] \n";

static const struct argp_option opts[] = {
	{ "port", 'p', "PORT", 0, "Port number for the REST interface" },
	{ "multi", 'm', 0, 0, "Enable multithreaded processing for REST interface" },
	{},
};

static error_t parse_arg(int key, char *arg, struct argp_state *state)
{
	switch (key) {
	case 'p':
		env.port = std::uint16_t(std::stoi(arg));
        if(env.port < 0) {
            log_error("Invalid port number");
            argp_usage(state);
        }
		break;
    case 'm':
        env.multithreaded = true;
        break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static const struct argp argp = {
	.options = opts,
	.parser = parse_arg,
	.doc = argp_program_doc,
};


int main(int argc, char **argv)
{
	/* Parse command line arguments */
	int err = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (err)
		return err;

    ObsvAgent agent;
    agent.run(env.port, env.multithreaded);

    return 0;
}
