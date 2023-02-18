#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../includes/Request.hpp"
# include "../includes/Server.hpp"
# include "../includes/Header.hpp"

class Response {

	public:
	Response();
	Response(Request req, std::vector<Server> vctServ, std::map<int, int> clientServer, char **envp);
	Response(Response const &src);
	~Response();

	Response	&operator=(Response const &rhs);

	Server		getServ() const;
	bool		getlocBlocSelect() const;
	Location	getLocBloc() const;
	bool		getCloseConnection() const;

	void		selectServerBlock();
	void		selectLocationBlock();
	void		sendData();

	private:
	Server						_serv;
	Request						_req;
	std::vector<Server>			_vctServ;
	std::map<int, int>			_clientServer;
	std::vector<std::string>	_path;
	std::vector<std::string>	_errPath;
	std::ifstream				_file;
	std::string					_httpRep;
	int							_statusCode;
	bool						_locBlocSelect;
	Location					_locBloc;
	bool						_isDir;
	bool						_autoindex;
	bool						_closeConnection;
	char						**_envp;

	std::string					rightRoot();
	std::vector<std::string>	rightIndex();
	bool						rightPathLocation();
	bool						rightPathServer();
	std::string					rightPathErr(bool &pageFind);
	void						fileAndDir(std::ofstream &file, bool getDir, std::string path);


	std::string	testAllPaths(bool *err);
	std::string	createDefaultErrorPage();
	std::string	createAutoindexPage();
	std::string	argsToStr();
	bool		rightPath();
	void		sendHeader(std::string path);
	void		sendPage(std::string path);
};

#endif