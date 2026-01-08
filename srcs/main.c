#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	ssize_t	bytes;
	char	buf;

	bytes = read(STDIN_FILENO, buf, BUFFER_SIZE)
}