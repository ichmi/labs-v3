#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>

int	main(int ac, char **av)
{
	char	*addr = av[1];

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "google.com");
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}