#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/process.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;
namespace ch = std::chrono;

bp::child make(int time = 0, std::string build = "Debug") {
    std::string cmd("cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=");
    cmd += "_install -DCMAKE_BUILD_TYPE=";
    cmd += build;
    std::cout << "command = " << cmd << std::endl;
    bp::child x(cmd, bp::std_out > stdout);
    if (time) {
        if (!x.wait_for(ch::seconds(time)));
        x.terminate();
    } else
        x.wait();
    return x;
}

bp::child build(int time = 0) {
    std::string cmd("cmake --build _builds");
    std::cout << "command = " << cmd << std::endl;
    bp::child c(cmd, bp::std_out > stdout);
    if (time) {
        if (!c.wait_for(ch::seconds(time)));
        c.terminate();
    } else
        c.wait();
    return c;
}

bp::child target(std::string inst) {
    std::string cmd("cmake --build _builds");
    cmd += " --target ";
    cmd += inst;
    std::cout << "command = " << cmd << std::endl;
    bp::child c(cmd, bp::std_out > stdout);
    c.wait();
    return c;
}

int main(int argc, char *argv[]) {

    po::options_description params("Allowed options");
    params.add_options()
            ("help", "produce help message")
            ("config", po::value<std::string>()->default_value("Debug"), "set build configuration")
            ("install", "add installation stage (in directory '_install')")
            ("pack", "add packaging stage (in tar.gz format archive)")
            ("timeout", po::value<int>(), "set waiting time (in seconds)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, params), vm);
    po::notify(vm);

    int res;

    if (vm.count("help")) {
        std::cout << params << std::endl;
        return 1;
    } else if (vm.count("config")) {
        std::string par(vm["config"].as<std::string>());
        res = make(0, par).exit_code();
        if (!res)
            res = build().exit_code();
        std::cout << "code == " << res << std::endl;
    } else if (vm.count("install") && vm.count("pack")) {
        res = make().exit_code();
        if (!res)
            res = build().exit_code();
        if (!res)
            res = target("install").exit_code();
        if (!res)
            res = target("pack").exit_code();
        std::cout << "code == " << res << std::endl;
    } else if (vm.count("install")) {
        res = make().exit_code();
        if (!res)
            res = build().exit_code();
        if (!res)
            res = target("install").exit_code();
        std::cout << "code == " << res << std::endl;
    } else if (vm.count("pack")) {
        res = make().exit_code();
        if (!res)
            res = build().exit_code();
        if (!res)
            res = target("package").exit_code();
        std::cout << "code == " << res << std::endl;
    } else if (vm.count("timeout")) {
        int tim = vm["timeout"].as<int>();
        res = make(tim).exit_code();
        if (!res)
            res = build(tim).exit_code();
        std::cout << "code == " << res << std::endl;
    } else {
        res = make().exit_code();
        if (!res)
            res = build().exit_code();
        std::cout << "code == " << res << std::endl;
    }
    return 0;
}