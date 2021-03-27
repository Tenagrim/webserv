#include <RequestReciever.hpp>
namespace ft
{
	void			RequestReciever::start()
	{
		open_main_socket();
		init_sockaddr();
		bind_main_socket();
	}

	void			RequestReciever::open_main_socket(void)
	{
		_main_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_main_socket == -1)
			throw std::runtime_error("Failed to create socket");
	}
	
	void			RequestReciever::init_sockaddr(void)
	{	
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_addr.s_addr = INADDR_ANY;
		_sockaddr.sin_port = htons(_port);
	}

	void			RequestReciever::bind_main_socket(void)
	{
		int ret;

		ret = bind(_main_socket, (struct sockaddr*)&_sockaddr, sizeof(_sockaddr));  // may be another sizeof
		if (ret == -1)
			throw std::runtime_error(std::string("\nUnable to bind socket: \n" + std::string(strerror(errno))));
	}

	void				RequestReciever::listen_main_socket()
	{
		int ret;

		ret = listen(_main_socket, _queue);
		if (ret < 0)
			throw std::runtime_error(std::string("\nUnable to listen socket: \n" + std::string(strerror(errno))));
	}
	
	void				RequestReciever::accept_connection()
	{
		unsigned int	addrlen;

		addrlen = sizeof(_main_socket);
		_client_fd = accept(_main_socket, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen);
		if (_client_fd < 0)
			throw std::runtime_error(std::string("\nUnable accept main socket: \n" + std::string(strerror(errno))));
		_client_connected = true;	
	}

	bool				RequestReciever::is_data_in_socket(int sock)
	{
		struct timeval		timeout;
		int					sret;
		fd_set				fdset;

		timeout.tv_usec = CHECK_SOCKET_DELAY % 1000;
		timeout.tv_sec = CHECK_SOCKET_DELAY / 1000;
		FD_ZERO(&fdset);
		FD_SET(sock, &fdset);

		sret = select(1, &fdset, NULL, NULL, &timeout);

		if (sret)
		{
			#ifdef DEBUG
				std::cout << "READY TO READ " << sret << " BYTES OF DATA IN SOCKET\n";
			#endif
			return true;
		}
		else
		{
			#ifdef DEBUG
				std::cout << "NO DATA IN SOCKET\n";
			#endif
			return false;
		}
	}

	bool				RequestReciever::is_client_data(unsigned int delay)
	{
		struct timeval		timeout;
		int					sret;
		fd_set				fdset;

		if (!_client_connected)
		{
			#ifdef DEBUG
				std::cout << "NO CLIENT CONNECTED\n";
			#endif
			return false;
		}

		#pragma region save some usecs
		if (delay <= 1000)
		{
			timeout.tv_usec = delay;
			timeout.tv_sec = 0;
		}
		else
		{
			timeout.tv_usec = delay % 1000;
			timeout.tv_sec = delay / 1000;
		}
		#pragma endregion
		
		FD_ZERO(&fdset);
		FD_SET(_client_fd, &fdset);

		sret = select(1, &fdset, NULL, NULL, &timeout);
		//            ^num of fds
		//                  ^read fds
		//                        ^write fds
		//                              ^err fds
		//                                    ^delay
		return (sret);	
		/*
		if (sret)
		{
			#ifdef DEBUG
				std::cout << "READY TO READ " << sret << " BYTES OF DATA\n";
			#endif
			return true;
		}
		else
		{
			#ifdef DEBUG
				std::cout << "NO CLIENT DATA HERE\n";
			#endif
			return false;
		}
		*/
	}

	std::string			RequestReciever::read_client_fd()
	{
		std::stringstream	ss;
		char				buff[READ_BUFF_SIZE];
		int					readed;

		while (is_client_data(CHECK_SOCKET_DELAY)) 
		{
			readed = read(_client_fd, buff, READ_BUFF_SIZE - 1);
			if (readed == - 1)
				throw std::runtime_error(std::string("\nUnable read client socket: \n" + std::string(strerror(errno))));
			ss << buff;
		}
		return(ss.str());
	}
	
	void					RequestReciever::close_connection()
	{
		if (_client_connected)
		{
			close(_client_fd);
			_client_connected = false;
		}
	}


	IRequest				*RequestReciever::getRequest()
	{
		if (!_client_connected)
			throw std::runtime_error("No Client Connected");
		if (!is_client_data(CHECK_SOCKET_DELAY))
			throw std::runtime_error("No Client Data to read");
		
		std::string		text = read_client_fd();
		return new Request(text);
	}

	RequestReciever::RequestReciever() : _host(DEFAULT_HOST), _port(DEFAULT_PORT)
	{
		throw std::runtime_error("No implementation");
	}

	RequestReciever::RequestReciever(std::string const &host, int port) : _host(host), _port(port), _queue(DEFAULT_CONN_QUEUE),  _client_connected(false)
	{
	}

	RequestReciever::~RequestReciever()
	{
		close_connection();
		close(_main_socket);
	}

	RequestReciever::RequestReciever(const RequestReciever &ref) : _host(DEFAULT_HOST), _port(DEFAULT_PORT)
	{
		(void)ref;
		throw std::runtime_error("No implementation");
	}

	RequestReciever &RequestReciever::operator=(const RequestReciever &ref)
	{
		(void)ref;
		throw std::runtime_error("No implementation");
		return (*this);
	}

	int						RequestReciever::getId()
	{
		return(_id);
	}

	long					RequestReciever::getListenFd()
	{
		return _main_socket;
	}
}