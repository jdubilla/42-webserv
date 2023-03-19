#include "utils.hpp"
#include "DefaultPage.hpp"
#include <stdlib.h>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>

int	ft_stoi(const std::string str, bool *err)
{
	int	sign = 1;
	int	res = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' or str[i] > '9')
		{
			*err = true;
			return 0;
		}
		res = res * 10 + str[i] - 48;
	}
	return res * sign;
}

bool	only_space_or_empty(std::string line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] != ' ' and line[i] != '\t' and line[i] != '\n')
			return false;
	}
	return true;
}

size_t	ft_belong_s(const std::string &s, const std::string &charset)
{
	size_t	i = 0;
	size_t	j = 0;

	while (i < s.length())
	{
		j = 0;
		while (j < charset.length())
		{
			if (charset[j] == s[i])
				return (charset[j]);
			j++;
		}
		i++;
	}
	return (0);
}

size_t	ft_belong_c(const char &c, const std::string &charset)
{
	size_t	i = 0;

	while (i < charset.length())
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

std::vector<std::string>	ft_split(const std::string &s, const std::string &charset)
{
	std::vector<std::string> v;
	std::string str(s);
	std::string	tmp;
	int			index = 0;
	char		c;

	while ((c = ft_belong_s(str, charset)))
	{
		index = str.find(c);
		tmp = str.substr(0, index);
		if (index)
			v.push_back(str.substr(0, index));
		while (ft_belong_c(str[index], charset))
			index++;
		str.assign(str, index, str.length());
	}
	if (str.length())
		v.push_back(str);

	return (v);
}

std::vector<std::string>	ft_split_str(const std::string &src, const std::string &charset)
{
	std::vector<std::string>	v;
	std::string					s(src);
	size_t						index;
	std::string					tmp;

	while ((index = s.find(charset)) != (size_t)-1)
	{
		tmp = s.substr(0, index);
		if (tmp.length())
			v.push_back(s.substr(0, index));
		s = s.substr(index + charset.length(), s.length()); 
	}
	if (s.length())
		v.push_back(s);
	return (v);
}

std::string	ft_itos(int nbr)
{
	std::string s;
	std::stringstream out;

	out << nbr;
	s = out.str();
	return s;
}

std::string	getHttpStatusCodeMessage(int statusCode) {
	int			httpCode[11] = {200, 204, 400, 403, 404, 405, 406, 413, 500, 502, 504};
	std::string	message[11] = {"OK", "No Content", "Bad Request", "Forbidden", "Not Found",
							"Method Not Allowed", "Not Acceptable","Request Entity Too Large" ,
							"Internal Server Error", "Bad Gateway", "Gateway Timeout"};

	for (size_t i = 0; i < 11; i++)
	{
		if (httpCode[i] == statusCode)
			return message[i];
	}
	return "";
}

std::string	fileToStr(std::string path) {
	std::ifstream	file(path.c_str(), std::ios::in | std::ios::binary);
	std::string		page;

	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		page = ss.str();
		file.close();
	}
	return page;
}

std::string 	removeChar(std::string s, char c)
{
	std::string str;
	size_t		i;

	i = 0;
	while (i < s.length())
	{
		if (s[i] != c)
			str += s[i];
		i++;
	}
	return (str);
}

size_t    tab_len(char **env)
{
    size_t  i = 0;
    
    while (env && env[i])
        i++;
    return (i);
}

bool	infileExists(const std::string &file)
{
	std::ifstream in(file.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!in.is_open())
		return (false);
		
	in.close();
	return (true);
}

void		lowerCaseStr(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
}

void			trimSpaceFront(std::string &line)
{
	while (line.length() && line[0] == ' ')
		line.erase(0, 1);
}

void			trimSpaceBack(std::string &line)
{
	while (line.length() && line[line.length() - 1] == ' ')
		line.erase(line.length() - 1, 1);
}

void			trimSpace(std::string &line)
{
	trimSpaceFront(line);
	trimSpaceBack(line);
}

std::string		getIPFromHostName(const std::string& hostName) {
	struct hostent* host = gethostbyname(hostName.c_str());
	if (!host)
		return "";

	std::stringstream ss;
	ss << inet_ntoa(*(struct in_addr*)host->h_addr);
	return ss.str();
}

std::string		getHostNameFromIP(const std::string& ipAddress) {
	std::ifstream hostsFile("/etc/hosts");
	std::string line;

	if (!hostsFile.is_open())
		return "";
	while (std::getline(hostsFile, line))
	{
		if (!line.empty() && line[0] != '#')
		{
			std::istringstream iss(line);
			std::string firstToken, hostName;
			iss >> firstToken >> hostName;
			if (firstToken == ipAddress)
			{
				hostsFile.close();
				return hostName;
			}
		}
	}
	hostsFile.close();
	return "";
}

std::string		getRightHost(const std::string& host) {
	std::vector<std::string>	resSplit;
	std::string					res;

	resSplit = ft_split(host.c_str(), ".");
	if (resSplit.size() == 4)
		return host;
	else if ((res = getIPFromHostName(host)) != "")
		return res;
	return "";
}

std::string	rightPathErr(bool &pageFind, int statusCode, Server const &serv, bool locBlockSelect, Location const &loc) {
	std::map<int, std::string>					mapErr;
	std::map<int, std::string>::const_iterator	it;
	std::string									rightPath;

	if (locBlockSelect and loc.getErrorPageSet())
	{
		mapErr = loc.getErrorPage();
		it = mapErr.find(statusCode);
		if (it != mapErr.end())
		{
			pageFind = true;
			rightPath = it->second;
		}
	}
	if (!pageFind and serv.getErrorPageSet())
	{
		mapErr = serv.getErrorPage();
		it = mapErr.find(statusCode);
		if (it != mapErr.end())
		{
			pageFind = true;
			rightPath = it->second;
		}
	}
	return rightPath;
}

std::string	findRightPageError(int statusCode, Server const &serv, bool locBlockSelect, Location const &loc) {
	bool		pageFind = false;
	std::string	path;
	DefaultPage	defaultPage;

	path = rightPathErr(pageFind, statusCode, serv, locBlockSelect, loc);

	std::ifstream tmp(path.c_str(), std::ios::in | std::ios::binary);

	if (!tmp or !pageFind)
		path = defaultPage.createDefaultPage(statusCode);
	if (tmp)
		tmp.close();
	return path;
}

bool	resolveHost(const std::string& host, std::string& ipAddress) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	if (getaddrinfo(host.c_str(), NULL, &hints, &res) != 0)
		return false;
	struct sockaddr_in* addr = (struct sockaddr_in*)res->ai_addr;
	ipAddress = inet_ntoa(addr->sin_addr);
	freeaddrinfo(res);

	return true;
}

void                free_tab(char **tab)
{
    size_t i = 0;

    while (tab && tab[i])
        free(tab[i++]);
    if (tab)
        free(tab);
}