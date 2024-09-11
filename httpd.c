#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

static void log_exit(char *fmt, ...);
static void* xmalloc(size_t sz);
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void service(FILE *in, FILE *out, char *docroot);

/* -- main -- */
int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <docroot>\n", argv[0]);
		exit(1);
	}
    
	struct stat sb;
	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		exit(1);
	}
	
	if (!(S_ISDIR(sb.st_mode))) {
		fprintf(stderr, "%s is not a directory\n", argv[1]);
		exit(1);
	}

}

/* -------------------- */
static void log_exit(char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vprintf(stderr, fmt, ap);
	fputc('\n', stderr);
	va_end(ap);
	exit(1);
}

static void* xmalloc(size_t sz) {
	void *p;

	p = malloc(sz);
	if (!p) log_exit("failed to allocate memory");
	return p;
}

static void install_signal_handlers(void) {
	trap_signal(SIGPIPE, signal_exit);
}

static void trap_signal(int sig, sighandler_t handler) {
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_falgs = SA_RESTART;
	if (sigaction(sig, &act, NULL) < 0)
		log_exit("sigaction() failed: %s\n", strerror(errno));
}

static void service(FILE *in, FILE *out, char *docroot) {
	struct HTTPRequset *req;
	req = read_request(in);
	respond_to(req, out, docroot);
	free_request(req);
}
