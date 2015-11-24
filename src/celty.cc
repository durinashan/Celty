/*! \file celty.cc
	Main entrypoint
*/
#include <celty-config.hh>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <syslog.h>
#include <cerrno>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <getopt.h>

#include <iostream>
#include <chrono>
#include <thread>

#include <ModuleLoader.hh>

#include <Bencode.hh>


static void sighndl(int sid);
static void daemonize(const char* lockfile);
static void dispatch(const char* signame);

static int lockfp = 0;
static int _daemonize = 1;

using namespace Celty;

int main(int argc, char* argv[]) {
	int c;
	std::string cfgfile;
	while(true) {
		static struct option long_opts[] = {
			{"keep-head", no_argument, &_daemonize, 0},
			{"sig", required_argument, 0,		  's'},
			{"cfg", required_argument, 0,         'c'},
			{"help", no_argument,      0,         'h'},
			{0, 0, 0, 0}
		};
		int opt_index = 0;
		c = getopt_long(argc, argv, "s:c:h", long_opts, &opt_index);
		if(c == -1)
			break;
		switch(c) {
			case 0: {
				if(long_opts[opt_index].flag != 0)
					break;
			} case 's': {
				if (optarg)
					dispatch(optarg);
				exit(0);
			} case 'c': {
				if (optarg)
					cfgfile = optarg;
				break;
			} case 'h': {
				exit(0);
			} case '?': {
				exit(0);
			} default: {
				break;
			}
		}
	}

	if(_daemonize) {
		openlog(DAEMON_NAME, LOG_PID, LOG_LOCAL5);
		syslog(LOG_INFO, "Celty %s Spinning up...", VERSION_STRING);
		daemonize(DEFAULT_LOCKDIR DEFAULT_LOCKFILE);
	} else {
		std::cout << "Celty v" << VERSION_STRING << std::endl;
		std::cout << "[@] NOTICE: Not daemonized" << std::endl;
	}
	Celty::Configuration* cfg = Celty::Configuration::GetInstance();
	Celty::ModuleLoader* modl = Celty::ModuleLoader::GetInstance();

	bool loadedcfg = false;
	if(!cfgfile.empty()) {
		loadedcfg = cfg->LoadConfig(cfgfile);
	} else {
		loadedcfg = cfg->LoadConfig();
	}

	if(!loadedcfg) {
		syslog(LOG_ERR, "Unable to load configuration file!");
		std::cerr << "[@] Unable to find configuration file!" << std::endl;
		syslog(LOG_INFO, "Releasing lock file %s", DEFAULT_LOCKDIR DEFAULT_LOCKFILE);
		close(lockfp);
		syslog(LOG_NOTICE, "Terminated");
		closelog();
		exit(-1);
	}

	syslog(LOG_INFO, "Loading Modules");
	modl->LoadAll(DEFAULT_MODULEDIR);
	modl->Foreach([=](Celty::Module* mod){
		mod->AnnounceSettings(cfg->ActiveConfig);
		mod->Run();
	});
	syslog(LOG_INFO, "Loaded %d module(s)", modl->GetLoadedModuleCount());
	int workers;
	if(cfg->ActiveConfig.find("workers") == cfg->ActiveConfig.end()) {
		workers = sysconf(_SC_NPROCESSORS_ONLN);
	} else {
		workers = std::stoi(cfg->ActiveConfig["workers"]);
	}

	if(_daemonize)
		syslog(LOG_INFO, "Starting %d worker(s)", workers);
	else
		std::cout << "[@] Starting " << workers << " worker(s)" << std::endl;

	for(; workers > 0; workers--) {
		// Do the thing
	}

	// Idle around

	syslog(LOG_INFO, "Unloading Modules");
	modl->Foreach([](Celty::Module* mod){
		mod->Halt();
	});
	modl->UnloadAll();

	delete modl;
	delete cfg;
	if(lockfp < 0) {
		syslog(LOG_INFO, "Releasing lock file %s", DEFAULT_LOCKDIR DEFAULT_LOCKFILE);
		close(lockfp);
	}

	if(_daemonize) {
		syslog(LOG_NOTICE, "Terminated");
		closelog();
	}
	return 0;
}


static void daemonize(const char* lockfile) {
	pid_t pid, sid, parent;
	/* In the case we are already daemonized */
	if(getppid() == 1) return;
	/* Try to obtain a lock */
	if(lockfile && lockfile[0]) {
		lockfp = open(lockfile, O_RDWR | O_CREAT, 0640);
		if(lockfp < 0) {
			syslog(LOG_ERR, "Unable to create lock file %s, error code: %d (%s)", lockfile, errno, strerror(errno));
			exit(-1);
		}
	}
	/* Drop the current user for a new one */
	if(getuid() == 0 || geteuid() == 0) {
		struct passwd *pw = getpwnam(DEFAULT_RUNAS);
		if(pw) {
			syslog(LOG_NOTICE, "Setting user to " DEFAULT_RUNAS);
			setuid(pw->pw_uid);
		}
	}
	/* Register signal handlers */
	signal(SIGALRM,  sighndl);
	signal(SIGTERM,  sighndl);
	signal(SIGHUP,   sighndl);
	signal(SIGCHLD,  sighndl);
	signal(SIGUSR1,  sighndl);
	signal(SIGUSR2,  sighndl);
	signal(SIGPIPE,  sighndl);



	/* Finally we fork off */
	pid = fork();
	if(pid < 0) {
		syslog(LOG_ERR, "Unable to fork daemon, error code: %d (%s)", errno, strerror(errno));
		exit(-1);
	}
	/* We are done here, */
	if(pid > 0) {
		/* We wait for SIGTERM/SIGCHLD to signal successful fork or alarm after 2 seconds */
		alarm(2);
		pause();
		exit(-1);
	}

	parent = getppid();
	signal(SIGCHLD,	SIG_DFL); /* A child process dies */
    signal(SIGTSTP,	SIG_IGN); /* Various TTY signals */
    signal(SIGTTOU,	SIG_IGN);
    signal(SIGTTIN,	SIG_IGN);
    signal(SIGTERM,	SIG_DFL); /* Die on SIGTERM */

	umask(0);

	/* Attempt to create new session */
	sid = setsid();
	if(sid < 0) {
		syslog(LOG_ERR, "Unable to create new session, error code: %d (%s)", errno, strerror(errno));
		exit(-1);
	}
	/* Attempt to change working directories */
	if(chdir(DEFAULT_WORKINGDIR) < 0) {
		syslog(LOG_ERR, "Unable to change working directory to %s, error code: %d (%s)", DEFAULT_WORKINGDIR, errno, strerror(errno));
		exit(-1);
	}

	std::cout << "[@] Celty daemonized to pid " << getpid() << std::endl;

	/* Redirect standard output streams to /dev/null */
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);

    /* Kill the parent process */
    kill(parent, SIGUSR1);
}

static void sighndl(int sid) {
	switch(sid) {
		case SIGPIPE:
		case SIGTERM: {
			exit(0);
		} case SIGALRM:
		  case SIGCHLD: {
			exit(-1);
		} case SIGUSR1: {
			/* --sig status */
		} case SIGUSR2: {
			/* --sig reload */
		} case SIGHUP: {
			/* --sig halt */
		}
		default: {
		}
	}
}

static void dispatch(const char* signame) {
	if(strncmp(signame, "status", 6) == 0) {

	} else if (strncmp(signame, "reload", 6) == 0) {

	} else if(strncmp(signame, "halt", 4) == 0) {

	} else {
		std::cerr << "Unknown signal '" << signame << "'" << std::endl;
	}
}
