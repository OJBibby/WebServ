#include "../include/Config.hpp"


Config::Config()
{

}

/**
 * @brief sets default values for the config
 * 
 */
void Config::init()
{
	this->_error_code = 0;
	this->set_port(80);
	this->set_addr(inet_addr("127.0.0.1"));
	this->set_server_name("default");
	this->set_client_max_body_size(1048576);
	this->set_autoindex(false);
	this->create_default_errors();
}

/**
 * @brief Construct a new Config:: Config object
 * 
 * @param obj 
 */
Config::Config(const Config &obj)
{
	*this = obj;
}

/**
 * @brief assignment operator overload
 * 
 * @param obj 
 * @return Config& 
 */
Config &Config::operator=(const Config &obj)
{
	if (this != &obj) {
		this->_port = obj._port;
		this->_addr = obj._addr;
		this->_server_name = obj._server_name;
		this->_name_port = obj._name_port;
		this->_host = obj._host;
		this->_default_error = obj._default_error;
		this->_client_max_body_size = obj._client_max_body_size;
		this->_autoindex = obj._autoindex;
		this->_root = obj._root;
		this->_index = obj._index;
		this->_error_code = obj._error_code;
		this->_location = obj._location;
		this->_redirection = obj._redirection;
		this->_intr_paths = obj._intr_paths;
		this->_upload_store = obj._upload_store;
	}
	return *this;
}

/**
 * @brief Destroy the Config:: Config object
 * 
 */
Config::~Config()
{

}

// Getters

/**
 * @brief gets config port
 * 
 * @return u_int16_t& 
 */
u_int16_t					&Config::get_port()
{
	return this->_port;
}

/**
 * @brief gets address to be used
 * 
 * @return in_addr& 
 */
in_addr					&Config::get_addr()
{
	return this->_addr;
}

/**
 * @brief gets host to be used
 * 
 * @return std::string& 
 */
std::string		&Config::getHost()
{
	return this->_host;
}

/**
 * @brief gets the server name
 * 
 * @return std::string& 
 */
std::string					&Config::get_server_name()
{
	return this->_server_name;
}

/**
 * @brief gets map with default errors
 * 
 * @return std::map<int, std::string>& 
 */
std::map<int, std::string>	&Config::get_default_error()
{
	return this->_default_error;
}

/**
 * @brief gets a single errors path
 * 
 * @param error  error to be returned
 * @return std::string& 
 */
std::string							&Config::get_error_path(int error)
{
	return this->get_default_error().at(error);
}

/**
 * @brief gets client max body size
 * 
 * @return size_t& 
 */
size_t 						&Config::get_client_max_body_size()
{
	return this->_client_max_body_size;
}

/**
 * @brief gets autoindex value, is it on or off
 * 
 * @return true 
 * @return false 
 */
bool						&Config::get_autoindex()
{
	return this->_autoindex;
}

/**
 * @brief gets the main context root
 * 
 * @return std::string& 
 */
std::string					&Config::get_root()
{
	return this->_root;
}

/**
 * @brief gets path to upload store
 * 
 * @return std::string& 
 */
std::string					&Config::get_upload_store()
{
	return this->_upload_store;
}

/**
 * @brief gets default index
 * 
 * @return std::string 
 */
std::string					Config::get_index()
{
	return this->_index;
}

/**
 * @brief gets all locations in config
 * 
 * @return std::map<std::string, Location>& 
 */
std::map<std::string, Location>		&Config::get_location()
{
	return this->_location;
}

/**
 * @brief gets error code in case config is faulty
 * 
 * @return int - error num
 */
int 								Config::get_error_code()
{
	return this->_error_code;
}

/**
 * @brief gets cgi extension - path map
 * 
 * @return std::map<std::string, std::string>& 
 */
std::map<std::string, std::string>	&Config::getIntrPath()
{
	return this->_intr_paths;
}

/**
 * @brief adds cgi extension and path to map
 * 
 * @param ext allowed file extension
 * @param path path to program that executes the extension
 */
void								Config::setIntrPath(std::string &ext, std::string &path)
{
	this->_intr_paths.insert(std::map<std::string, std::string>::value_type(ext, path));
}

// Setters

/**
 * @brief sets config error code
 * 
 * @param error_code 
 */
void 					Config::set_error_code(int error_code)
{
	this->_error_code = error_code;
}

/**
 * @brief sets config port
 * 
 * @param port 
 */
void					Config::set_port(uint16_t port)
{
	this->_port = port;
}

/**
 * @brief sets config address
 * 
 * @param addr 
 */
void					Config::set_addr(in_addr_t addr)
{
	this->_addr.s_addr = addr;
}

/**
 * @brief sets config host
 * 
 * @param host 
 */
void	Config::setHost(std::string host)
{
	this->_host = host;
}

/**
 * @brief sets config server name
 * 
 * @param server_name 
 */
void					Config::set_server_name(std::string server_name)
{
	this->_server_name = server_name;
}

/**
 * @brief adds default error to map
 * 
 * @param i error number
 * @param default_error default error path
 */
void					Config::set_default_error(int i, std::string default_error)
{
	this->_default_error[i] = default_error;
}

/**
 * @brief set config max body size
 * 
 * @param clien_max_body_size 
 */
void 					Config::set_client_max_body_size(int clien_max_body_size)
{
	this->_client_max_body_size = clien_max_body_size;
}

/**
 * @brief set config autoindex 
 * 
 * @param autoindex 
 */
void					Config::set_autoindex(bool autoindex)
{
	this->_autoindex = autoindex;
}

/**
 * @brief set config root
 * 
 * @param root 
 */
void					Config::set_root(std::string root)
{
	this->_root = root;
}

/**
 * @brief set upload path
 * 
 * @param upload_store 
 */
void					Config::set_upload_store(std::string upload_store)
{
	this->_upload_store = upload_store;
}

/**
 * @brief set default index
 * 
 * @param index 
 */
void					Config::set_index(std::string index)
{
	this->_index = index;
}

/**
 * @brief add location to location map
 * 
 * @param config_file whole config file
 * @param line line where config first appears
 */
void					Config::set_location(std::ifstream& config_file, std::string line)
{
	std::string key = get_value(line);
	Location location(config_file, line, this->get_root(),this->get_index(), key);
	if (location.get_error_code() == 11)
		throw std::runtime_error("Invalid configuration file context.");
	this->set_error_code(location.get_error_code());
	if (!location.get_redirection().empty())
		this->_redirection.insert(std::make_pair(key, location.get_redirection()));
	else
		this->_location.insert(std::make_pair(key, location));
}

/**
 * @brief check if config is valid
 * 
 */
void						Config::check_config()
{
	// root value exists and given directory exists
	if (this->get_root().size() == 0 || dir_exists(this->get_root()) == false) {
		this->set_error_code(20);
		throw std::logic_error(INVALID_ROOT);
	}
	
	// index value exists and given file exists
	if (this->get_index().size() == 0 || !file_exists(this->get_root() + this->get_index())) {
		this->set_error_code(21);
		throw std::logic_error(INVALID_INDEX);
	}
	
	if (this->get_upload_store().size() > 0 && !dir_exists(this->get_root() + this->get_upload_store())) {
		this->set_error_code(30);
		throw std::logic_error(INVALID_UPLOAD_STORE);
	}
	// Location check
	std::map<std::string, Location> location = this->get_location();
	for (std::map<std::string, Location>::iterator it = location.begin(); it != location.end(); it++) {
		int error_code = it->second.check_location();
		if (error_code != EXIT_SUCCESS) {
			this->set_error_code(error_code);
			switch (this->get_error_code()) {
				case 23:
					throw std::logic_error(INVALID_LOCATION_ROOT);
				case 24:
					throw std::logic_error(INVALID_LOCATION_INDEX);
				case 25:
					throw std::logic_error(INVALID_LOCATION_REDIRECTION);
				case 26:
					throw std::logic_error(INVALID_METHODS);
			}
		}
	}
	// CGI check
	std::map<std::string, std::string>::const_iterator p_it = _intr_paths.begin();
	while (p_it != _intr_paths.end()) {
		if (!file_exists(p_it->second)) {
			throw std::logic_error(INVALID_PROGRAM_PATH);
		}
		p_it++;
	}
}

/**
 * @brief assigns paths to default supported errors
 * 
 */
void	Config::create_default_errors()
{
	if (!file_exists("error/400_BadRequest.html"))
		throw std::runtime_error("Webserv default error file [error/400_BadRequest.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/403_Forbidden.html"))
		throw std::runtime_error("Webserv default error file [error/403_Forbidden.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/404_NotFound.html"))
		throw std::runtime_error("Webserv default error file [error/404_NotFound.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/405_MethodNotAllowed.html"))
		throw std::runtime_error("Webserv default error file [error/405_MethodNotAllowed.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/406_NotAcceptable.html"))
		throw std::runtime_error("Webserv default error file [error/406_NotAcceptable.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/408_RequestTimeout.html"))
		throw std::runtime_error("Webserv default error file [error/408_RequestTimeout.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/411_LengthRequired.html"))
		throw std::runtime_error("Webserv default error file [error/411_LengthRequired.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/413_PayloadTooLarge.html"))
		throw std::runtime_error("Webserv default error file [error/413_PayloadTooLarge.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/414_URITooLarge.html"))
		throw std::runtime_error("Webserv default error file [error/414_URITooLarge.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/415_UnsupportedMediaType.html"))
		throw std::runtime_error("Webserv default error file [error/415_UnsupportedMediaType.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/418_Imateapot.html"))
		throw std::runtime_error("Webserv default error file [error/418_Imateapot.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/429_TooManyRequests.html"))
		throw std::runtime_error("Webserv default error file [error/429_TooManyRequests.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/500_InternalServer.html"))
		throw std::runtime_error("Webserv default error file [error/500_InternalServer.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/501_NotImplemented.html"))
		throw std::runtime_error("Webserv default error file [error/501_NotImplemented.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/502_BadGateway.html"))
		throw std::runtime_error("Webserv default error file [error/502_BadGateway.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/503_ServiceUnavailable.html"))
		throw std::runtime_error("Webserv default error file [error/503_ServiceUnavailable.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/504_GatewayTimeout.html"))
		throw std::runtime_error("Webserv default error file [error/504_GatewayTimeout.html] has been moved or deleted. Please restore it and try again.");
	if (!file_exists("error/505_HTTPVersionNotSupported.html"))
		throw std::runtime_error("Webserv default error file [error/505_HTTPVersionNotSupported.html] has been moved or deleted. Please restore it and try again.");
	this->set_default_error(400, "error/400_BadRequest.html");
	this->set_default_error(403, "error/403_Forbidden.html");
	this->set_default_error(404, "error/404_NotFound.html");
	this->set_default_error(405, "error/405_MethodNotAllowed.html");
	this->set_default_error(406, "error/406_NotAcceptable.html");
	this->set_default_error(408, "error/408_RequestTimeout.html");
	this->set_default_error(411, "error/411_LengthRequired.html");
	this->set_default_error(413, "error/413_PayloadTooLarge.html");
	this->set_default_error(414, "error/414_URITooLarge.html");
	this->set_default_error(415, "error/415_UnsupportedMediaType.html");
	this->set_default_error(418, "error/418_Imateapot.html");
	this->set_default_error(429, "error/429_TooManyRequests.html");
	this->set_default_error(500, "error/500_InternalServer.html");
	this->set_default_error(501, "error/501_NotImplemented.html");
	this->set_default_error(502, "error/502_BadGateway.html");
	this->set_default_error(503, "error/503_ServiceUnavailable.html");
	this->set_default_error(504, "error/504_GatewayTimeout.html");
	this->set_default_error(505, "error/505_HTTPVersionNotSupported.html");
}

/**
 * @brief gets rediretions map
 * 
 * @return std::map<std::string, std::string>& 
 */
std::map<std::string, std::string> &Config::getRedirection()
{
	return this->_redirection;
}

/**
 * @brief gets config _name_port
 * 
 * @return std::string& 
 */
std::string &Config::getNamePort()
{
	return this->_name_port;
}

/**
 * @brief sets config _name_port
 * 
 * @param name_port 
 */
void	Config::setNamePort(std::string name_port)
{
	this->_name_port = name_port;
}
