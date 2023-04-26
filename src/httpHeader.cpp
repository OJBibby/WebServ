#include "../include/httpHeader.hpp"

/**
 * @brief Construct a new http Header::http Header object
 * 
 */
httpHeader::httpHeader()
{
	this->_content_length = 0;
}

/**
 * @brief check to see if method is valid
 * 
 * @param line 
 * @return true 
 * @return false 
 */
bool httpHeader::isMethodValid(std::string line)
{
	/* check to see if method is valid */
	if (line == "GET" || line == "POST" || line == "DELETE" || line == "PUT" || \
		line == "HEAD" || line == "OPTIONS" || line == "TRACE" || line == "CONNECT")
		return true;
	return false;

}

/**
 * @brief check to see if version is valid
 * 
 * @param line 
 * @return true 
 * @return false 
 */
bool httpHeader::isVersionValid(std::string line)
{
	/* check to see if version is valid */
	if (line == "HTTP/1.0" || line == "HTTP/1.1")
		return true;
	else if (line.substr(0, 5) == "HTTP/")
	{
		this->_error = 3;
		this->_version.clear();
		return true;
	}
	return false;
}

/**
 * @brief check to see if URI is valid
 * 
 * @param line 
 * @return true 
 * @return false 
 */
bool httpHeader::isUriValid(std::string line)
{
	/* check to see if URI is valid */
	if (line[0] == '/')
		return true;
	return false;
}

/**
 * @brief check to see if header is valid
 * 
 * @param line 
 * @return true 
 * @return false 
 */
bool httpHeader::isHeaderValid(std::string line)
{
	/* check to see if header is valid */
	if (line.find(": ") != std::string::npos)
		return true;
	
	return false;
}

/**
 * @brief Construct a new http Header::http Header object
 * 
 * @param header 
 */
httpHeader::httpHeader(std::string header)
{
	_statusCode = 0;
	_sentSize = 0;
	_requestTime = get_current_time();
	this->_error = 0;
	
	size_t start = 0, end = 0;
	
	/* Find the request header */
	this->_header_length = header.find("\r\n\r\n") + 4;
	
	/* Find the method */
	end = header.find(" ");
	if (end == std::string::npos)
		this->_error = 2;
	std::string tmp_method = header.substr(start, end - start);
	if (!isMethodValid(tmp_method)) {
		/* method is not valid */
		this->_error = 2;
	}

	/* Find the URI */
	start = end + 1;
	end = header.find(" ", start);
	this->_uri = header.substr(start, end - start);
	if (end - start > 500)
	{
		/* "URI is too long" */
		this->_error = 1;
		this->_uri.clear();
	}
	else if (!isUriValid(this->_uri)) {
		/* URI is not valid */
		this->_error = 2;
		this->_uri.clear();
	}
	else
	{
		this->_uri = decodeURI(header.substr(start, end - start));
		start = this->_uri.find_first_of("?");
		if (start != std::string::npos)
		{
			this->_query = this->_uri.substr(start);
			this->_uri.erase(start);
		}
	}

	/* Find the version */
	start = end + 1;
	end = header.find("\r\n", start);
	this->_version = header.substr(start, end - start);
	if (!isVersionValid(this->_version)) {
		/* version is not valid */
		this->_error = 2;
		this->_version.clear();
	}
	
	setMethod(tmp_method);
	
	/* Create key : value pairs */
	start = end + 1;
	while ((end = header.find("\r\n", start)) != std::string::npos)
	{
		std::string line = header.substr(start, end - start);
		if (line.empty())
			break;
		if (line[0] == '\n')
			line.erase(0, 1);
		if (!isHeaderValid(line))
		{
			_error = 2;
			start = end + 2;
			break ;
		}
		size_t separator = line.find(": ");
		if (separator != std::string::npos) {
			std::string name;
			if (line[0] == '\n')
				name = line.substr(1, separator - 1);
			else
				name = line.substr(0, separator);
			std::string value = line.substr(separator + 2);
			if (name.empty() || value.empty())
			{
				_error = 2;
				break;
			}
			name = toLowerCase(name);
			// Prints header
			// std::cout << GREEN << name << ": " << value << RESET << std::endl;
			this->setHeader(name, value);
		}
		else
			break;
		start = end + 2;
	}
	this->_content_length = atol(this->get_single_header("content-length").c_str());
}

/**
 * @brief Destroy the http Header::http Header object
 * 
 */
httpHeader::~httpHeader()
{}

/**
 * @brief Construct a new http Header::http Header object
 * 
 * @param copy 
 */
httpHeader::httpHeader(const httpHeader& copy)
{
	*this = copy;
}

/**
 * @brief assignment operator overlaod
 * 
 * @param rhs 
 * @return httpHeader& 
 */
httpHeader &httpHeader::operator=(const httpHeader& rhs)
{
	if (this != &rhs)
	{
		this->_header = rhs._header;
		this->_method = rhs._method;
		this->_uri = rhs._uri;
		this->_query = rhs._query;
		this->_version = rhs._version;
		this->_header_length = rhs._header_length;
		this->_content_length = rhs._content_length;
		this->_cookie = rhs._cookie;
		this->_error = rhs._error;
		this->_requestTime = rhs._requestTime;
		this->_statusCode = rhs._statusCode;
		this->_sentSize = rhs._sentSize;
		this->_userIP = rhs._userIP;
	}
	return *this;
}

/**
 * @brief gets method in header
 * 
 * @return const httpMethods& 
 */
const httpMethods &httpHeader::getMethod() const
{
	return(this->_method);
}

/**
 * @brief gets URI
 * 
 * @return const std::string& 
 */
const std::string &httpHeader::getUri() const
{
	return(this->_uri);
}

/**
 * @brief gets HTTP version
 * 
 * @return const std::string& 
 */
const std::string &httpHeader::getVersion() const
{
	return(this->_version);
}

/**
 * @brief gets error generate in header construction
 * 
 * @return const uint8_t& 
 */
const uint8_t &httpHeader::isError() const
{
	return this->_error;
}

/**
 * @brief gets a single entry in the header map
 * 
 * @param entry 
 * @return const std::string 
 */
const std::string httpHeader::get_single_header(std::string entry)
{
	std::string empty = "";
	std::map<std::string, std::string>::iterator it = this->_header.find(entry);
	if (it != this->_header.end()) {
		return it->second;
	}
	return empty;
}

/**
 * @brief 
 * 
 * @param size 
 */
void httpHeader::setSentSize(int size)
{
	this->_sentSize = size;
}

/**
 * @brief sets header object status code
 * 
 * @param code 
 */
void httpHeader::setStatusCode(int code)
{
	this->_statusCode = code;
}

/**
 * @brief sets user IP
 * 
 * @param address 
 */
void httpHeader::setUserIP(std::string address)
{
	this->_userIP = address;
}

/**
 * @brief 
 * 
 * @return int 
 */
int httpHeader::getSentSize()
{
	return this->_sentSize;
}

/**
 * @brief gets status code associated with header
 * 
 * @return int 
 */
int httpHeader::getStatusCode()
{
	return this->_statusCode;
}

/**
 * @brief inserts single entry into header map
 * 
 * @param name 
 * @param value 
 */
void httpHeader::setHeader(std::string name, std::string value)
{
	this->_header.insert(std::map<std::string, std::string>::value_type(name, value));
}

/**
 * @brief sets method to be used in header
 * 
 * @param tmp_method 
 */
void httpHeader::setMethod(std::string tmp_method)
{
	int i = 0;
	std::string methods[9] = {"GET", "POST", "DELETE", "PUT", \
		"HEAD", "OPTIONS", "TRACE", "CONNECT", "NONE"};
	
	for (i = 0; i < 9; i++)
	{
		if (tmp_method == methods[i])
			break;
	}
	switch (i)
	{
		case 0:
			this->_method = GET;
			break;
		case 1:
			this->_method = POST;
			break;
		case 2:
			this->_method = DELETE;
			break;
		case 3:
			this->_method = PUT;
			break;
		case 4:
			this->_method = HEAD;
			break;
		case 5:
			this->_method = OPTIONS;
			break;
		case 6:
			this->_method = TRACE;
			break;
		case 7:
			this->_method = CONNECT;
			break;
		default:
			this->_method = NONE;
			break;
	}

}

/**
 * @brief prints header
 * 
 */
void httpHeader::printHeader()
{
	std::string method;
	switch (_method) {
		case 0:
			method = "GET";
			break;
		case 1:
			method = "POST";
			break;
		case 2:
			method = "DELETE";
			break;
		case 3:
			method = "PUT";
			break;
		case 4:
			method = "HEAD";
			break;
		case 5:
			method = "OPTIONS";
			break;
		case 6:
			method = "TRACE";
			break;
		case 7:
			method = "CONNECT";
			break;
		case 8:
			method = "NONE";
			break;				
	}
 
	std::cout << PURPLE << "[" << _userIP << "]" << " " << RESET;
	std::cout << "[" << PURPLE << _requestTime << RESET << "] ";
	std::cout << YELLOW << "\"" << method << " " << _uri << " " << _version << "\" ";
	std::cout << PURPLE << _statusCode << " " << _sentSize << " " << RESET;
	
	std::map<std::string, std::string>::iterator itr = _header.begin();
	while (itr != _header.end() && itr->first != "user-agent") 
		itr++;
	if (itr != _header.end())
		std::cout << YELLOW << "\"" << itr->second << "\"" << RESET;
	std::cout << "\n\n";

}

/**
 * @brief checks if header has the right version
 * 
 * @return true 
 * @return false 
 */
bool httpHeader::isHttp11()
{
	if ((this->_version == "HTTP/1.1" || this->_version == "http/1.1" || this->_version == "Http/1.1") \
		&& !get_single_header("host").empty())
		return true;
	return false;
}

/**
 * @brief gets header length
 * 
 * @return size_t 
 */
size_t httpHeader::getHeaderLength()
{
	return this->_header_length;
}

/**
 * @brief gets content length
 * 
 * @return size_t 
 */
size_t httpHeader::getContentLength()
{
	return this->_content_length;
}

/**
 * @brief sets the URI
 * 
 * @param str 
 */
void	httpHeader::setURI(std::string str)
{
	this->_uri = str;
}

/**
 * @brief gets the complete header map
 * 
 * @return const std::map<std::string, std::string>& 
 */
const std::map<std::string, std::string>& httpHeader::getCompleteHeader() const
{
	return this->_header;
}

/**
 * @brief get Port
 * 
 * @return int 
 */
int httpHeader::getPort()
{
	std::map<std::string, std::string>::iterator it = this->_header.find("host");
	if (it != this->_header.end())
	{
		size_t pos = it->second.find_first_of(":");
		if (pos != std::string::npos)
		{
			pos++;
			char *end = NULL;
			return strtoul(it->second.substr(pos).c_str(), &end, 10);
		}
	}
	return -1;
}

/**
 * @brief gets current time
 * 
 * @return std::string 
 */
std::string	httpHeader::get_current_time()
{
  	time_t now = time(NULL);
  	struct tm *tm_now = localtime(&now);

  	char time_str[128];
  	strftime(time_str, sizeof(time_str), "%d/%b/%Y:%H:%M:%S %z", tm_now);

  	return std::string(time_str);
}
