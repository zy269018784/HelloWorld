  #include <stdio.h>
  #include <string.h>
  #include <unistd.h>
  #include <fcntl.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		return -1;

	int fd = open(argv[1], O_RDWR);
	
	char buf[10] = {};

	write(fd, argv[2], strlen(argv[2]));

	int n = read(fd, buf, 5);
	buf[n] = '\0';
	printf("read %d bytes\n", n);	
	printf("%s\n", buf);

	close(fd);

	return 0;
}
