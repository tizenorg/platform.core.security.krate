/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sungbae Yoo <sungbae.yoo@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file
 * @author  Sungbae Yoo (sungbae.yoo@samsung.com)
 * @brief   CLI tool to attach to existing namespace by pam-attach.so
 */
#define _XOPEN_SOURCE
#include <glib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <grp.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include <zone/package-proxy.h>
#include <zone/app-proxy.h>

#include "session.h"

#include "error.h"
#include "exception.h"
#include "filesystem.h"
#include "auth/user.h"


#define DEFAULT_SHELL "/bin/bash"

typedef std::pair<std::string, int> NamespacePair;

std::vector<NamespacePair> namespaces = {
    {"mnt",  CLONE_NEWNS},
    {"net",  CLONE_NEWNET},
    {"uts",  CLONE_NEWUTS},
    {"user", CLONE_NEWUSER},
    {"ipc",  CLONE_NEWIPC},
    {"pid",  CLONE_NEWPID}
};

extern char** environ;

static inline void usage(const std::string name)
{
    std::cout << "Usage: " << name << " [Option] -- COMMAND" << std::endl
              << "Run a program in the zone" << std::endl
              << std::endl
              << "Options :" << std::endl
              << "   -a, --attach=zone  execute command in the zone" << std::endl
              << "   -p, --pkginfo=zone show all packages in the zone" << std::endl
              << "   -q, --appinfo=zone show all applications in the zone" << std::endl
              << "   -m, --zone-monitor monitor all zone events" << std::endl
              << "   -n, --pkg-monitor  monitor all package events in the zone" << std::endl
              << "   -l, --list         show all zone instances" << std::endl
              << "   -h, --help         show this" << std::endl
              << std::endl;
}

int showZoneInstances()
{
    try {
        runtime::DirectoryIterator iter(runtime::Path("/var/run/zone")), end;

        while (iter != end) {
            const std::string& path = (*iter).getPath();
            size_t name = path.rfind('/') + 1;
            std::cout << path.substr(name) << std::endl;
            ++iter;
        }
    } catch (runtime::Exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

void attachNamespaces(pid_t pid)
{
    int fd;
    for (const NamespacePair& ns : namespaces) {
        std::string nspath = "/proc/" + std::to_string(pid) + "/ns/" + ns.first;

        do {
            fd = ::open(nspath.c_str(), O_RDONLY);
        } while (fd == -1 && errno == EINTR);

        if (fd == -1) {
            if (errno != ENOENT) {
                throw runtime::Exception("Failed to open namesapce: " + nspath);
            }
        } else {
            if (::setns(fd, ns.second)) {
                ::close(fd);
                throw runtime::Exception("Failed to attach namespace: " + nspath);
            }
            ::close(fd);
        }
    }
}

void launchProcess(runtime::User& user, char** args)
{
    int status;

    pid_t pid = ::fork();
    if (pid == -1) {
        throw runtime::Exception("Failed to create child process");
    }

    if (pid != 0) {
        ::waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != EXIT_SUCCESS) {
            throw runtime::Exception("Child process terminated abnormally");
        }
        return;
    }

    if (::initgroups(user.getName().c_str(), user.getGid()) == -1) {
        std::cerr << "Failed to initgroups()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    if (::setgid(user.getGid()) == -1) {
        std::cerr << "Failed to setgid()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    if (::setuid(user.getUid()) == -1) {
        std::cerr << "Failed to setuid()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    char *default_args[2];
    default_args[0] = ::strdup(DEFAULT_SHELL);
    default_args[1] = NULL;

    if (args == NULL) {
        args = default_args;
    }

    ::execve(*args, args, environ);
    std::cerr << "Failed to execute " << args[0] << std::endl;

    ::free(default_args[0]);

    ::exit(EXIT_FAILURE);
}

int attachToZone(const std::string& name, char* args[])
{
    try {
        runtime::User user(name);

        attachNamespaces(getSessionLeader(user));
        launchProcess(user, args);
    } catch (runtime::Exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

bool packgeListCallback(package_info_h info, void *user_data)
{
    bool checked;
    char *val;

    std::cout << "Package #" << (*(int*)user_data)++ << " :" << std::endl;

    package_info_get_package(info, &val);
    std::cout << val;
    free(val);

    package_info_get_version(info, &val);
    std::cout << " (" << val << ")" << std::endl;
    free(val);

    package_info_get_type(info, &val);
    std::cout << "    Type : " << val << " (";
    free(val);

    package_info_is_system_package(info, &checked);
    if (checked) {
        std::cout <<" System ";
    }
    package_info_is_removable_package(info, &checked);
    if (checked) {
        std::cout <<" Removable ";
    }
    package_info_is_preload_package(info, &checked);
    if (checked) {
        std::cout <<" Preload ";
    }
    std::cout << ")" << std::endl;

    package_info_get_label(info, &val);
    std::cout << "    Label : " << val << std::endl;
    free(val);

    package_info_get_icon(info, &val);
    std::cout << "    Icon : " << val << std::endl;
    free(val);

    std::cout << std::endl;

    return true;
}

int showPkgInfo(const std::string& name)
{
    int num = 0;

    zone_manager_h zoneMgr;
    zone_package_proxy_h pkgProxy;

    zone_manager_create(&zoneMgr);
    zone_package_proxy_create(zoneMgr, name.c_str(), &pkgProxy);
    zone_package_proxy_foreach_package_info(pkgProxy, packgeListCallback, &num);
    std::cout << num << " packages are found" << std::endl;
    zone_package_proxy_destroy(pkgProxy);
    zone_manager_destroy(zoneMgr);

    return 0;
}

bool applicationListCallback(app_info_h info, void *user_data)
{
    bool checked;
    char *val;

    std::cout << "Application #" << (*(int*)user_data)++ << " :" << std::endl;

    app_info_get_app_id(info, &val);
    std::cout << val;
    free(val);

    app_info_get_package(info, &val);
    std::cout << " (" << val << ")" << std::endl;
    free(val);

    app_info_get_type(info, &val);
    std::cout << "    Type : " << val << " (";
    free(val);

    app_info_is_nodisplay(info, &checked);
    if (checked) {
        std::cout <<" NoDisplay ";
    }
    std::cout << ")"<< std::endl;

    app_info_get_label(info, &val);
    std::cout << "    Label : " << val << std::endl;
    free(val);

    if (app_info_get_icon(info, &val) == APP_MANAGER_ERROR_NONE) {
        std::cout << "    Icon : " << val << std::endl;
        free(val);
    } else {
        std::cout << "App Manager error" << std::endl;
    }

    std::cout << std::endl;

    return true;
}

int showAppInfo(const std::string& name)
{
    int num = 0;

    zone_manager_h zoneMgr;
    zone_app_proxy_h appMgr;

    zone_manager_create(&zoneMgr);
    zone_app_proxy_create(zoneMgr, name.c_str(), &appMgr);
    zone_app_proxy_foreach_app_info(appMgr, applicationListCallback, &num);
    std::cout << num << " applications are found" << std::endl;
    zone_app_proxy_destroy(appMgr);
    zone_manager_destroy(zoneMgr);

    return 0;
}

GMainLoop *gmainloop = NULL;

void monitorSigHandler(int sig)
{
    g_main_loop_quit(gmainloop);
}

void zoneCallback(const char* name, const char* object, void *user_data)
{
    std::cout << "--- Zone event ---" << std::endl;
    std::cout << "Type : " << (char*)user_data << std::endl;
    std::cout << "Name : " << name <<std::endl;
    std::cout << std::endl;
}

int monitorEvent()
{
    int zoneCallbackId[2];
    zone_manager_h zoneMgr;
    zone_manager_create(&zoneMgr);

    zone_manager_add_event_cb(zoneMgr, "created", zoneCallback,
                                (void*)"created", &zoneCallbackId[0]);
    zone_manager_add_event_cb(zoneMgr, "removed", zoneCallback,
                                (void*)"removed", &zoneCallbackId[1]);

    std::cout << "=== Monitoring start ===" << std::endl << std::endl;

    signal(SIGINT, monitorSigHandler);

    gmainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(gmainloop);
    g_main_loop_unref(gmainloop);

    zone_manager_remove_event_cb(zoneMgr, zoneCallbackId[0]);
    zone_manager_remove_event_cb(zoneMgr, zoneCallbackId[1]);

    std::cout << "===  Monitoring end  ===" << std::endl;

    zone_manager_destroy(zoneMgr);

    return 0;
}

void packageEventCallback(const char *type,
                          const char *package,
                          package_manager_event_type_e event_type,
                          package_manager_event_state_e event_state,
                          int progress, package_manager_error_e error,
                          void *user_data)
{
    std::cout << "--- Package event ---" << std::endl;
    std::cout << "Pacakge : " << package << std::endl;
    std::cout << "Pacakge type : " << type << std::endl;

    std::cout << "Event state : ";
    switch (event_state) {
    case PACKAGE_MANAGER_EVENT_STATE_STARTED:
        std::cout << "started" << std::endl;
        std::cout << "Event type : ";
        switch (event_type) {
        case PACKAGE_MANAGER_EVENT_TYPE_INSTALL:
            std::cout << "install";
            break;
        case PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL:
            std::cout << "uninstall";
            break;
        case PACKAGE_MANAGER_EVENT_TYPE_UPDATE:
            std::cout << "update";
            break;
        }
        break;
    case PACKAGE_MANAGER_EVENT_STATE_PROCESSING:
        std::cout << "processing" << std::endl;
        std::cout << "Progress : " << std::to_string(progress) << "%";
        break;
    case PACKAGE_MANAGER_EVENT_STATE_FAILED:
        std::cout << "failed";
        break;
    case PACKAGE_MANAGER_EVENT_STATE_COMPLETED:
        std::cout << "completed";
        break;
    }
    std::cout << std::endl;

    std::cout << std::endl;
}

int monitorPkgEvent(const std::string& name)
{
    zone_manager_h zoneMgr;
    zone_package_proxy_h pkgProxy;

    zone_manager_create(&zoneMgr);
    zone_package_proxy_create(zoneMgr, name.c_str(), &pkgProxy);

    zone_package_proxy_set_event_cb(pkgProxy, packageEventCallback, NULL);

    std::cout << "=== Monitoring start ===" << std::endl << std::endl;

    signal(SIGINT, monitorSigHandler);

    gmainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(gmainloop);
    g_main_loop_unref(gmainloop);

    zone_package_proxy_unset_event_cb(pkgProxy);

    std::cout << "===  Monitoring end  ===" << std::endl;

    zone_package_proxy_destroy(pkgProxy);
    zone_manager_destroy(zoneMgr);

    return 0;
}

int main(int argc, char* argv[])
{
    int opt = 0, index, ret = 0;

    struct option options[] = {
        {"attach", required_argument, 0, 'a'},
        {"list", no_argument, 0, 'l'},
        {"pkginfo", required_argument, 0, 'p'},
        {"appinfo", required_argument, 0, 'q'},
        {"zone-monitor", no_argument, 0, 'm'},
        {"pkg-monitor", no_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    while ((opt = getopt_long(argc, argv, "a:p:q:mn:lh", options, &index)) != -1) {
        switch (opt) {
        case 'a':
            ret = attachToZone(optarg, optind >= argc ? NULL : argv + optind);
            break;
        case 'p':
            ret = showPkgInfo(optarg);
            break;
        case 'q':
            ret = showAppInfo(optarg);
            break;
        case 'm':
            ret = monitorEvent();
            break;
        case 'n':
            ret = monitorPkgEvent(optarg);
            break;
        case 'l':
            ret = showZoneInstances();
            break;
        case 'h':
            usage(argv[0]);
            break;
        default:
            usage(argv[0]);
            ret = -1;
        }
    }

    return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
