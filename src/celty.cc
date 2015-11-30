/*! \file celty.cc
	Main entry point
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
#include <vector>

#include <Configuration.hh>
#include <ModuleLoader.hh>
#include <Statistician.hh>
#include <Endpoint.hh>

#include <Bencode.hh>


static void evsighndl(ev::sig& sig, int i);
static void sighndl(int sid);
static void daemonize(const char* lockfile);
static void dispatch(const char* signame);

static int lockfp = 0;
static int _daemonize = 1;

using namespace Celty;

int main(int argc, char* argv[]) {
	int c;
	std::string cfgfile = CONFIG_FILE;
	std::string modfolder = DEFAULT_MODULEDIR;
	while(true) {
		static struct option long_opts[] = {
			{"keep-head", no_argument, &_daemonize, 0},
			{"sig", required_argument, 0,		  's'},
			{"cfg", required_argument, 0,         'c'},
			{"module-dir", required_argument, 0,  'm'},
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
			} case 'm': {
				if(optarg)
					modfolder = optarg;
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
		std::cout << "Celty " << VERSION_STRING << std::endl;
		std::cout << "[@] NOTICE: Not daemonized" << std::endl;
	}
	Configuration* cfg = Configuration::GetInstance();
	ModuleLoader* modl = ModuleLoader::GetInstance();
	Statistician* stats = Statistician::GetInstance();

	bool loadedcfg = cfg->LoadConfig(cfgfile);

	if(!loadedcfg) {
		syslog(LOG_ERR, "Unable to load configuration file!");
		std::cerr << "[@] Unable to find configuration file!" << std::endl;
		syslog(LOG_INFO, "Releasing lock file %s", DEFAULT_LOCKDIR DEFAULT_LOCKFILE);
		close(lockfp);
		syslog(LOG_NOTICE, "Terminated");
		closelog();
		exit(-1);
	}
	if(_daemonize)
		syslog(LOG_INFO, "Loading Modules");
	else
		std::cout << "[@] Loading Modules" << std::endl;
	modl->LoadAll(modfolder);
	modl->Foreach([=](Celty::Module* mod){
		mod->AnnounceSettings(cfg->ActiveConfig);
		mod->Run();
	});
	if(_daemonize)
		syslog(LOG_INFO, "Loaded %d module(s)", modl->GetLoadedModuleCount());
	else
		std::cout << "[@] Loaded " << modl->GetLoadedModuleCount() << " module(s)" << std::endl;
	int workers;
	if(cfg->ActiveConfig.find("Workers") == cfg->ActiveConfig.end()) {
		workers = sysconf(_SC_NPROCESSORS_ONLN);
	} else {
		workers = ((workers = std::stoi(cfg->ActiveConfig["Workers"])) == 0) ? sysconf(_SC_NPROCESSORS_ONLN) : workers;
	}

	if(_daemonize)
		syslog(LOG_INFO, "Starting %d worker(s)", workers);
	else
		std::cout << "[@] Starting " << workers << " worker(s)" << std::endl;

	std::vector<Endpoint*> endpoints;
	endpoints.reserve(workers+1);

	ev::default_loop loop;

	ev::sig sigint;
	ev::sig sigkill;
	ev::sig sigterm;

	sigint.set<&evsighndl>();
	sigint.start(SIGINT);
	sigkill.set<&evsighndl>();
	sigkill.start(SIGKILL);
	sigterm.set<&evsighndl>();
	sigterm.start(SIGTERM);

	for(; workers > 0; workers--) {
		// Endpoint* e = new Endpoint(/* TODO */);
		// e->Start();
		// endpoints.push_back(e);
	}

	if(cfg->ActiveConfig.find("APIEndpoint") != cfg->ActiveConfig.end()) {
		if(cfg->ActiveConfig["APIEndpoint"] == "yes") {
			if(_daemonize)
				syslog(LOG_INFO, "Starting API Endpoint");
			else
				std::cout << "[@] Starting API Endpoint" << std::endl;
			Endpoint* eapi = new Endpoint(Endpoint::API, cfg->ActiveConfig["APIListen"], cfg->ActiveConfig["APIPort"]);
			eapi->Start();
			endpoints.push_back(eapi);
		}
	}

	loop.run(0);

	if(_daemonize)
		syslog(LOG_INFO, "Halting workers");
	else
		std::cout << std::endl << "[@] Halting workers " << std::endl;

	sigint.stop();
	sigkill.stop();
	sigterm.stop();

	for(auto e : endpoints) {
		e->Halt();
		delete e;
	}
	if(_daemonize)
		syslog(LOG_INFO, "Unloading Modules");
	else
		std::cout << "[@] Unloading Modules" << std::endl;

	modl->Foreach([](Celty::Module* mod){
		mod->Halt();
	});
	modl->UnloadAll();

	delete modl;
	delete cfg;
	delete stats;

	if(lockfp < 0) {
		syslog(LOG_INFO, "Releasing lock file %s", DEFAULT_LOCKDIR DEFAULT_LOCKFILE);
		close(lockfp);
	}

	if(_daemonize) {
		if(remove(PID_FILE) < 0 ) {
			syslog(LOG_ERR, "Unable to remove PID file %s, error code: %d (%s)", PID_FILE, errno, strerror(errno));
		}
		syslog(LOG_NOTICE, "Terminated");
		closelog();
	}
	if(!_daemonize) {
		std::cout << "[@] Bye Bye" << std::endl;
	}
	return 0;
}


static void daemonize(const char* lockfile) {
	pid_t pid, sid, parent, cpid;
	int pidfd;
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

	cpid = getpid();
	if((pidfd = open(PID_FILE, O_CREAT | O_RDWR, 0640)) < 0) {
		syslog(LOG_ERR, "Unable to create PID file %s, error code: %d (%s)", PID_FILE, errno, strerror(errno));
		exit(-1);
	}
	char buff[10];
	snprintf(buff, sizeof(buff), "%d", cpid);
	write(pidfd, buff, strlen(buff));
	close(pidfd);

	std::cout << "[@] Celty daemonized to pid " << cpid << std::endl;

	/* Redirect standard output streams to /dev/null */
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);

	/* Kill the parent process */
	kill(parent, SIGTERM);
}

static void evsighndl(ev::sig& sig, int) {
	sig.loop.break_loop(ev::ALL);
	sig.stop();
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
		} case SIGHUP: {
			/* --sig reload */
			Configuration::GetInstance()->ReloadConfiguration();
		}
		default: {
		}
	}
}

static void dispatch(const char* signame) {
	int pidfd;
	if((pidfd = open(PID_FILE, O_CREAT | O_RDWR, 0640)) < 0) {
		std::cerr << "Unable to open PID file " << PID_FILE << ", error code: "<< errno << " ("<< strerror(errno) <<")" << std::endl;
		exit(-1);
	}
	char buff[10];
	read(pidfd, buff, sizeof(buff));
	int pid = atoi(buff);
	close(pidfd);

	int sig = SIGTSTP;

	if(strncmp(signame, "status", 6) == 0) {
		sig = SIGUSR1;
	} else if (strncmp(signame, "reload", 6) == 0) {
		std::cout << "[@] Reloading Configuration" << std::endl;
		sig = SIGHUP;
	} else if(strncmp(signame, "halt", 4) == 0) {
		std::cout << "[@] Stopping Celty..." << std::endl;
		sig = SIGTERM;
	} else {
		std::cerr << "Unknown signal '" << signame << "'" << std::endl;
	}

	if(kill(pid, sig) < 0) {
		std::cerr << "Unable to send signal, error code: "<< errno << " ("<< strerror(errno) <<")" << std::endl;
		exit(-1);
	}
}
