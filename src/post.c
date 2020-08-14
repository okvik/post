#include <u.h>
#include <libc.h>

void
usage(void)
{
	fprint(2, "usage: %s [-q] [-m mode] file [name]\n", argv0);
	exits("usage");
}

int
openrw(char *n)
{
	int fd;
	
	if((fd = open(n, ORDWR)) >= 0)
		return fd;
	if((fd = open(n, OREAD)) >= 0)
		return fd;
	if((fd = open(n, OWRITE)) >= 0)
		return fd;
	return -1;
}

void
main(int argc, char *argv[])
{
	int mode = 0660, quiet = 0;

	int fd, srvfd;
	char name[256];

	ARGBEGIN{
	case 'm':
		mode = strtoul(EARGF(usage()), nil, 8); break;
	case 'q':
		quiet = 1; break;
	default: usage();
	}ARGEND;
	if(argc == 0 || argc > 2) usage();
	if(argc == 2)
		snprint(name, sizeof name, "/srv/%s", argv[1]);
	else
		snprint(name, sizeof name, "/srv/post.%d", getpid());

	if((fd = openrw(argv[0])) == -1)
		sysfatal("open: %r");
	if((srvfd = create(name, OWRITE, mode)) == -1)
		sysfatal("create: %r");
	fprint(srvfd, "%d", fd);

	if(!quiet) print("%s\n", name);

	exits(nil);
}
