#pragma once
#include <ABody.hpp>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <runtime_error.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <webserv.hpp>
namespace ft
{
	class FileBody : public ABody
	{
	private:
		unsigned int	_size;
		std::string		_filename;
		int 			_opened_fd;
		int 			_offset;
	public:

	private:

		FileBody();
		FileBody(const FileBody &ref);
		std::string		readFile() const;
	public:
		FileBody(std::string const &path, int offset = 0);
		~FileBody();
		FileBody &operator=(const FileBody &ref);

		int					getOffset() const;
		int					getFd();
		std::string			to_string();
		unsigned int		size() const;
		BodyType			getType();
		int					getOpenedFd();
	};
}