#include "Request.hpp"
#include "DefaultPage.hpp"
#include "Header.hpp"
#include "utils.hpp"
#include <string.h>
#include <sys/epoll.h>
#include <fstream>

Request::Request() {}

Request::Request(int fd) : 
_fd(fd), _errRequest(false), _queryStringSet(false), _boundarySet(false),
_closeConnection(false), _connectionSet(false), _acceptSet(false),
_refererSet(false), _agentSet(false), _serverName("Webserv/1.0"),
_methodSet(false), _hostSet(false), _tooLarge(false),
_badRequest(false), _awaitingRequest(false), _endAwaitingRequest(false),
_bytesRecieved(0), _bodyFileExists(false), _bodyFilePath("./uploads/bodyfile")
{
	this->functPtr[0] = &Request::setMethodVersionPath;
	this->functPtr[1] = &Request::setMethodVersionPath;
	this->functPtr[2] = &Request::setMethodVersionPath;
	this->functPtr[3] = &Request::setMethodVersionPath;
	this->functPtr[4] = &Request::setHostPort;
	this->functPtr[5] = &Request::setConnection;
	this->functPtr[6] = &Request::setAccept;
	this->functPtr[7] = &Request::setReferer;
	this->functPtr[8] = &Request::setAgent;
	this->functPtr[9] = &Request::setAuthentification;
	this->functPtr[10] = &Request::setContentLength;
	this->functPtr[11] = &Request::setContentType;
}

Request::Request(Request const &src) {
	*this = src;
}

Request::~Request() {}

Request	&Request::operator=(Request const &rhs) {
	if (this != &rhs)
	{
		this->_fd = rhs._fd;
		this->_errRequest = rhs._errRequest;
		this->_queryStringSet = rhs._queryStringSet;
		this->_boundarySet = rhs._boundarySet;
		this->_closeConnection = rhs._closeConnection;
		this->_boundary = rhs._boundary;

		this->_method = rhs._method;
		this->_path = rhs._path;
		this->_httpVersion = rhs._httpVersion;
		this->_host = rhs._host;
		this->_port = rhs._port;

		this->_connection = rhs._connection;
		this->_connectionSet = rhs._connectionSet;
		this->_accept = rhs._accept;
		this->_acceptSet = rhs._acceptSet;
		this->_queryString = rhs._queryString;
		this->_refererSet = rhs._refererSet;
		this->_referer = rhs._referer;
		this->_agentSet = rhs._agentSet;
		this->_agent = rhs._agent;

		this->_serverName = rhs._serverName;
		this->_authentification = rhs._authentification;
		this->_contentLength = rhs._contentLength;
		this->_contentType = rhs._contentType;
		this->_methodSet = rhs._methodSet;
		this->_hostSet = rhs._hostSet;
		this->_tooLarge = rhs._tooLarge;
		this->_badRequest = rhs._badRequest;

		this->_awaitingRequest = rhs._awaitingRequest;
		this->_endAwaitingRequest = rhs._endAwaitingRequest;
		this->_bytesRecieved = rhs._bytesRecieved;
		this->_bodyFileExists = rhs._bodyFileExists;
		this->_bodyFilePath = rhs._bodyFilePath;
	}
	return *this;
}

size_t		Request::getBytesRecievd() const
{
	return (this->_bytesRecieved);
}

void		Request::setErrorRequest(bool v)
{
	this->_errRequest = v;
}

int			Request::getFd() const {
	return this->_fd;
}

bool		Request::getErrRequest() const {
	return this->_errRequest;
}

bool		Request::getcloseConnection() const {
	return this->_closeConnection;
}

std::string	Request::getMethod() const {
	return this->_method;
}

std::string	Request::getPath() const {
	return this->_path;
}

std::string	Request::getHttpVersion() const {
	return this->_httpVersion;
}

std::string	Request::getHost() const {
	return this->_host;
}

std::string	Request::getPort() const {
	return this->_port;
}

std::string	Request::getConnection() const {
	return this->_connection;
}

std::string	Request::getReferer() const {
	return this->_referer;
}

std::string	Request::getAgent() const {
	return this->_agent;
}

std::string	Request::getServerName() const {
	return this->_serverName;
}

std::string	Request::getAuthentification() const {
	return this->_authentification;
}
std::string	Request::getContentLength() const {
	return this->_contentLength;
}

std::string	Request::getContentType() const {
	return this->_contentType;
}

std::string	Request::getAccept() const {
	return this->_accept;
}

std::map<std::string, std::string>	Request::getQueryString() const {
	return this->_queryString;
}

bool		Request::getConnectionSet() const {
	return this->_connectionSet;
}

bool		Request::getAcceptSet() const {
	return this->_acceptSet;
}

bool		Request::getRefererSet() const {
	return this->_refererSet;
}

bool		Request::getAgentSet() const {
	return this->_agentSet;
}

bool		Request::getBadRequest() const {
	return this->_badRequest;
}

bool		Request::getAwaitingRequest() const {
	return this->_awaitingRequest;
}

bool		Request::getEndAwaitingRequest() const {
	return this->_endAwaitingRequest;
}

void	Request::parsArgs(std::string arg) {
	std::vector<std::string>	args;
	std::vector<std::string>	keyValue;

	this->_queryStringSet = true;
	args = ft_split(arg.c_str(), "&");
	for (size_t i = 0; i < args.size(); i++)
	{
		keyValue = ft_split(args[i].c_str(), "=");
		if (keyValue.size() == 1)
			this->_queryString.insert(std::make_pair(keyValue[0], ""));
		else
			this->_queryString.insert(std::make_pair(keyValue[0], keyValue[1]));
	}
}

void	Request::setMethodVersionPath(std::vector<std::string> strSplit) {
	std::vector<std::string>	splitBis;

		if (strSplit.size() != 3)
			this->_badRequest = true;
		else
		{
			this->_method = strSplit[0];
			if (strSplit[2] != "HTTP/1.0" && strSplit[2] != "HTTP/1.1")
				this->_badRequest = true;
			this->_httpVersion = strSplit[2];
			strSplit = ft_split(strSplit[1].c_str(), "?");
			if (strSplit.size() == 2)
				this->parsArgs(strSplit[1]);
			this->_path = strSplit[0];
			this->_methodSet = true;
		}
}

void	Request::setHostPort(std::vector<std::string> strSplit) {
	strSplit = ft_split(strSplit[1].c_str(), ":");
	this->_host = strSplit[0];
	this->_port = strSplit[1];
	this->_hostSet = true;
}

void	Request::setConnection(std::vector<std::string> strSplit) {
	this->_connection = strSplit[1];
	this->_connectionSet = true;
}

void	Request::setAccept(std::vector<std::string> strSplit) {
	this->_accept = strSplit[1];
	this->_acceptSet = true;
}

void	Request::setReferer(std::vector<std::string> strSplit) {
	this->_referer = strSplit[1];
	this->_refererSet = true;
}

void	Request::setAgent(std::vector<std::string> strSplit) {
	// for (size_t i = 1; i < strSplit.size(); i++)
		// std::cout << strSplit[i] << std::endl;
	// std::cout << std::endl;
	for (size_t i = 1; i < strSplit.size(); i++)
	{
		if (i != 1)
			this->_agent += " ";
		this->_agent += strSplit[i];
	}
	// std::cout << this->_agent << std::endl;
	this->_agentSet = true;
}

void	Request::setAuthentification(std::vector<std::string> strSplit) {
	for (size_t i = 1; i < strSplit.size(); i++)
	{
		if (i != 1)
			this->_authentification += " ";
		this->_authentification += strSplit[i];
	}
}

void	Request::setContentLength(std::vector<std::string> strSplit) {
	this->_contentLength = strSplit[1];
}

void	Request::setContentType(std::vector<std::string> strSplit) {
	for (size_t i = 1; i < strSplit.size(); i++)
	{
		if (i != 1)
			this->_contentType += " ";
		this->_contentType += strSplit[i];
	}
	if (strSplit.size() == 3 and strSplit[1] == "multipart/form-data;" and
		strSplit[2].find("boundary=") != std::string::npos)
	{
		strSplit = ft_split(strSplit[2].c_str(), "=\"");
		this->_boundarySet = true;
		this->_boundary = "--";
		this->_boundary += strSplit[1];
	}
}

void	Request::setGetParams(std::vector<std::string> vct, size_t *i) {
	std::vector<std::string>	strSplit;
	std::string					endBoundary = this->_boundary + "--";
	std::string					name;
	std::string					value;

	while (*i < vct.size() and vct[*i] != endBoundary)
	{
		strSplit = ft_split(vct[*i].c_str(), " ");
		if (strSplit[0] == "Content-Disposition:")
		{
			for (size_t j = 0; j < strSplit.size(); j++)
			{
				if (strSplit[j].find("name=") != std::string::npos)
				{
					// Tester si dans le code html, il n'y a pas de name ou que le name est vide
					name = ft_split(strSplit[j].c_str(), "=\"")[1];
					break ;
				}
			}
			*i += 2;
			value = vct[*i];
			this->_queryString.insert(std::make_pair(name, value));
		}
		*i += 1;
	}
}

std::string	Request::extractFileName(const std::string &line)
{
	std::string 				tmp(line);
	std::vector<std::string>	v;
	std::vector<std::string>	keys;

	v = ft_split(tmp, " ");
	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i].find("=") != (size_t)-1)
		{
			keys = ft_split(v[i], "=");
			if (keys[0] == "filename")
			{
				keys = ft_split(keys[1], "\"");
				if (keys.size())
					return (keys[0]); 
			}
		}
	}
	return ("");
}

/* void	Request::parseBoundaryData(const std::string &bound_data)
{
	std::vector<std::string>	data;
	std::vector<std::string>	fields;

	bool						contentType;

	std::string					fileName;
	std::ofstream				outfile;

	fileName = "./uploads/";
	contentType = false;
	data = ft_split_str(bound_data, "\r\n\r\n");

	std::cout << "data.size(): " << data.size() << std::endl;

	if (data.size())
	{
		fields = ft_split(data[0], "\r\n");

		std::cout << "fields.size(): " << fields.size() << std::endl;

		if (fields.size() >= 2)
		{
			if (!memcmp(fields[0].c_str(), "Content-Disposition:", 20))
				fileName += this->extractFileName(fields[0]);
			if (!memcmp(fields[1].c_str(), "Content-Type:", 13))
				contentType = true;
		}

		if (contentType)
		{
			outfile.open(this->_bodyFilePath.c_str(),
				std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

			if (!outfile.is_open())
				perror("error open outfile ");

			for (size_t i = 1; i < data.size(); i++)
			{
					data[i] += "\r\n\r\n";
					outfile.write(data[i].c_str(), data[i].length());
			}
			outfile.close();
			
			 if (rename(_bodyFilePath.c_str(), fileName.c_str()))
				perror("error rename fiel failed"); 
		}
	}
} */


void	Request::parseBoundaryData(const std::string &bound_data)
{
	bool						contentType;
	size_t						index;
	std::string					fileName;
	std::vector<std::string>	fields;
	std::ofstream				outfile;


	fileName = "./uploads/";
	contentType = false;
	index = bound_data.find("\r\n\r\n");

	if (index == (size_t)-1)
		std::cerr << "err index == -1" << std::endl;

	fields = ft_split(bound_data.substr(0, index), "\r\n");

	std::cout << "fields.size(): " << fields.size() << std::endl;

	if (fields.size() >= 2)
	{
		if (!memcmp(fields[0].c_str(), "Content-Disposition:", 20))
			fileName += this->extractFileName(fields[0]);
		if (!memcmp(fields[1].c_str(), "Content-Type:", 13))
			contentType = true;
	}

	if (contentType)
	{
		outfile.open(this->_bodyFilePath.c_str(),
			std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

		if (!outfile.is_open())
			perror("error open outfile ");

		outfile.write(bound_data.c_str() + index + 4, bound_data.length() - index - 4 - 2);

		outfile.close();
		
		/* if (rename(_bodyFilePath.c_str(), fileName.c_str()))
			perror("error rename fiel failed"); */
	}
}



/*
	Extract and save the data in boundary 
*/

void	Request::extractFile(const std::string &inpath)
{
	std::string					request;
	std::vector<std::string>	bounds;

	request = fileToStr(inpath);
	bounds = ft_split_str(request, this->_boundary);
	std::cout << "bounds.size(): " << bounds.size() << std::endl;
	for (size_t i = 0; i < bounds.size() && bounds[i] != "--\r\n"; i++)
		this->parseBoundaryData(bounds[i]);
}

/*
	Extract and set HTTP fields
*/
void		Request::extractFields(const std::string &header)
{
	std::vector<std::string>	vct;
	std::vector<std::string>	strSplit;
	std::string					key[12] = { "GET", "HEAD", "POST", "DELETE", "Host:",
				"Connection:", "Accept:", "Referer:", "User-Agent:", "Authentification:",
				"Content-Length:", "Content-Type:"};

	vct = ft_split(header, "\n\r");
	for (size_t i = 0; i < vct.size(); i++)
		{
			strSplit = ft_split(vct[i].c_str(), " ");
			if (this->_boundarySet and strSplit[0] == this->_boundary)
				this->setGetParams(vct, &i);
			else
			{
				for (size_t j = 0; j < 12; j++)
				{
					if (strSplit[0] == key[j])
					{
						(this->*functPtr[j])(strSplit);
						break ;
					}
				}
			}
		}
}

int	Request::awaitingRequest(int fd)
{
	int							bytes = 0;
	size_t						total_bytes = 0;
	size_t						bufflen = 4096;
	char						buff[bufflen + 1];
	std::ofstream 				out;

	if (this->_bodyFileExists)
		out.open(this->_bodyFilePath.c_str(),
			std::ofstream::out | std::ofstream::ate | std::ofstream::app | std::ofstream::binary);
	else
	{
		out.open(this->_bodyFilePath.c_str(), std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
		this->_bodyFileExists = true;
	}

	if (!out.is_open())
	{
		out.close();
		remove(this->_bodyFilePath.c_str());
		this->_awaitingRequest = false;
		this->_endAwaitingRequest = true;
		this->getErrorPage();
		return (0);
	}

	while ((bytes = recv(fd, buff, bufflen, 0)) > 0)
	{
		total_bytes += bytes;
		buff[bytes] = '\0';
		out.write(buff, bytes);
		if (bytes < (int)bufflen)
			break ;
	}

	/* if (!bytes)
	{
		remove(this->_bodyFilePath.c_str());
		this->_awaitingRequest = false;
		this->_endAwaitingRequest = true;
		this->_closeConnection = true;
		return (0);
	} */

	out.close();
	this->_bytesRecieved += total_bytes;

	if ((int)this->_bytesRecieved == ft_stoi(this->_contentLength, NULL))
	{		
		this->extractFile(this->_bodyFilePath.c_str());
		//remove(this->_bodyFilePath.c_str());
		this->_awaitingRequest = false;
		this->_endAwaitingRequest = true;
		this->_bodyFileExists = false;
		return (0);
	}


	return (0);
}

int		Request::parsRequest(int fd)
{
	size_t						bufflen = 4096;
	char						buff[bufflen + 1];
	int							oct = 0;

	std::string request;

	std::string	header;
	std::string	body;

	std::ofstream out;

	size_t	total = 0;

	if (!this->_awaitingRequest)
	{
		oct = recv(fd, buff, bufflen, 0);

		if (oct < 1)
		{
			if (!oct)
			{
				this->_closeConnection = true;
				return (0);
			}
			else if (oct == -1)
			{
				perror("recv call failed");
				this->getErrorPage();
				return (1);
			}
		}
		
		total = oct;

		buff[oct] = '\0';
		// header simple

		std::cout << "first recv total = " << oct << std::endl;

		request.append(buff);

		size_t	index = request.find("\r\n\r\n");

		header = request.substr(0, index);
		body = request.substr(index + 4, request.length());

		this->extractFields(header);


		std::cout << "\n///////	HEADER		///////////\n" << header << std::endl;
		std::cout << "\n///////	BODY		///////////\n" << body << std::endl;;


		std::cout << "body.length() = " << body.length() << std::endl;

		if (body.length())
		{

			std::ofstream	out(this->_bodyFilePath.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

			out.write(&buff[index + 4],  total - (header.length() + 4));

			std::cout << total - (header.length() + 4) << " bytes write (total - header)" << std::endl;


			while ((oct = recv(fd, buff, bufflen, 0)) > 0)
			{
				total += oct;
				buff[oct] = '\0';
				out.write(buff, oct);
				if (oct < (int)bufflen)
					break ;
			}

			// check err recv ??

			out.close();

			this->_bodyFileExists = true;
			
			std::cout << total - (header.length() + 4) << " bytes write " << std::endl;
	
		}

		if (this->_methodSet && this->_method == "POST"
					&& ft_stoi(this->_contentLength, NULL) != (int)(total - (header.length() + 4)))
		{
			std::cout << "/////////// NEED TO WAIT ANOTHER REQUEST" << std::endl;

			this->_awaitingRequest = true;
			this->_bytesRecieved += (total - (header.length() + 4));		
			std::cout << "_bytesRecieved = " << this->_bytesRecieved << std::endl;	
			
		}
		else
		{
			if (this->_bodyFileExists)
			{
				this->extractFile(this->_bodyFilePath.c_str());
				this->_bodyFileExists = false;
				//remove(this->_bodyFilePath.c_str());
			}
			std::cout << " ///	NOT AN AWAITING REQUEST	/////" << std::endl;
		}

		if (!this->_methodSet || !this->_hostSet || this->_badRequest)
			this->getErrorPage();
	}
	else
	{
		int res = this->awaitingRequest(fd);
		if (res)
			return (res);
	}

	return 0;
}

void	Request::getErrorPage() {
	std::string	path;
	std::string	strHeader;
	std::string	page;
	int			statusCode;

	if (this->_tooLarge)
		statusCode = 413;
	if (!this->_methodSet)
		statusCode = 400;
	else
		statusCode = 500;

	DefaultPage	defaultPage;
	path = defaultPage.createDefaultErrorPage(statusCode);

	Header header(path, &statusCode);
	strHeader = header.getHeaderRequestError();
	send(this->_fd, strHeader.c_str(), strHeader.size(), MSG_NOSIGNAL);

	page = fileToStr(path);
	send(this->_fd, page.c_str(), page.size(), MSG_NOSIGNAL);
	this->_closeConnection = true;
}

std::ostream &operator<<(std::ostream & o, Request const & rhs) {
	o << "method = " << rhs.getMethod() << std::endl;
	o << "path = " << rhs.getPath() << std::endl;
	o << "httpVersion = " << rhs.getHttpVersion() << std::endl;
	o << "host = " << rhs.getHost() << std::endl;
	o << "port = " << rhs.getPort() << std::endl;
	return o;
}