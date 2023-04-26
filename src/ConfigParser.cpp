#include "../include/ConfigParser.hpp"

/**
 * @brief Construct a new Config Parser:: Config Parser object
 * 
 */
ConfigParser::ConfigParser()
{
	std::ifstream in_file;
	
	this->_error_code = 0;
	this->_n_servers = 0;
	
	// Check if config file exists
	in_file.open("./webserv.config", std::ios::in);
	if (in_file.fail() == true) {
		this->exit_with_error(1, in_file);
		return;
	}
	
	// Check if server context exists
	if (this->check_server_context(in_file) == false)
		this->exit_with_error(this->get_error_code(), in_file);
	in_file.close();
}

/**
 * @brief Construct a new Config Parser:: Config Parser object
 * 
 * @param config_file 
 */
ConfigParser::ConfigParser(std::string config_file)
{
	std::ifstream in_file;
	
	this->_error_code = 0;
	this->_n_servers = 0;
	
	// Check if config file exists
	in_file.open(config_file.c_str(), std::ios::in);
	if (in_file.is_open() == false){
		this->exit_with_error(28, in_file);
		return;
	};
	
	// Check if server context exists
	if (this->check_server_context(in_file) == false)
		this->exit_with_error(this->get_error_code(), in_file);
	in_file.close();
}

/**
 * @brief Destroy the Config Parser:: Config Parser object
 * 
 */
ConfigParser::~ConfigParser() {}

std::vector<Config>	&ConfigParser::get_configs()
{
	return this->_configs;
}

/**
 * @brief gets config at index i
 * 
 * @param i config to return
 * @return Config& 
 */
Config& ConfigParser::get_config(int i)
{
	return this->_configs.at(i);
}

/**
 * @brief gets parser error code
 * 
 * @return int 
 */
int ConfigParser::get_error_code()
{
	return this->_error_code;
}

/**
 * @brief gets how many configs were generated
 * 
 * @return int 
 */
int ConfigParser::get_n_servers()
{
	return this->_n_servers;
}

/**
 * @brief sets the parser error code
 * 
 * @param error_code 
 */
void ConfigParser::set_error_code(int error_code)
{
	this->_error_code = error_code;
}

/**
 * @brief sets the number of configs that were created
 * 
 * @param i 
 */
void ConfigParser::set_n_servers(int i)
{
	this->_n_servers = i;
}

/**
 * @brief parses config file for different server contexts and locations within
 * 		servers, also performs some error checks
 * 
 * @param config_file file with server configuration(s)
 * @return true 
 * @return false 
 */
bool ConfigParser::check_server_context(std::ifstream& config_file)
{
	std::string line;
	int context = 0;

	while (getline(config_file, line) && !this->_error_code) {
		line = remove_comments(line);
		if (line.find_first_not_of(" \r\t\b\f") == std::string::npos)
			continue;
		if ((line.find("server") != std::string::npos && check_def_format("server", line) && line.find("{") != std::string::npos) && line.find("}") != std::string::npos)
			throw std::runtime_error("Server context is empty.");
		else if ((line.find("server") != std::string::npos && check_def_format("server", line) && line.find("{") != std::string::npos) && context == 0) {
			context += 1;
			this->_configs.push_back(Config());
			this->_configs.at(this->_n_servers).init();
			this->_n_servers++;
			continue;
		}
		else if (context == 0) {
			throw std::runtime_error("Line outside of server context is not comment or empty line.");
		}
		if ((context && line.find(LISTEN) != std::string::npos) && check_def_format(LISTEN, line))
			this->clean_listen(line);
		else if ((context && line.find(ROOT) != std::string::npos) && check_def_format(ROOT, line))
			this->clean_root(line);
		else if ((context && line.find(INDEX) != std::string::npos) && check_def_format(INDEX, line))
			this->clean_index(line);
		else if ((context && line.find(ERROR_PAGE) != std::string::npos) && check_def_format(ERROR_PAGE, line))
			this->clean_error_page(line);
		else if ((context && line.find(SERVER_NAME) != std::string::npos) && check_def_format(SERVER_NAME, line))
			this->clean_server_name(line);
		else if ((context && line.find(CLIENT_MAX_BODY) != std::string::npos) && check_def_format(CLIENT_MAX_BODY, line))
			this->clean_client_max_body_size(line);
		else if ((context && line.find(AUTOINDEX) != std::string::npos) && check_def_format(AUTOINDEX, line))
			this->clean_autoindex(line);
		else if ((line.find(CGI_PATH) != std::string::npos) && check_def_format(CGI_PATH, line))
			this->clean_path(line);
		else if ((line.find(CGI_EXT) != std::string::npos) && check_def_format(CGI_EXT, line))
			this->clean_ext(line);
		else if ((line.find(UPLOAD_STORE) != std::string::npos) && check_def_format(UPLOAD_STORE, line))
			this->clean_upload_store(line);
		/*
			go into function and separate location config
			keep track of context
		*/
		if (line.find("location") != std::string::npos && line.find("{") != std::string::npos)
			this->get_config(this->get_n_servers() - 1).set_location(config_file, line);
		else if (line.find("{") != std::string::npos)
			throw std::runtime_error("Invalid configuration file context.");
        if (line.find("}") != std::string::npos)
        {
            context -= 1;
            if (context == 0)
                addToExtMap();
            else if (context < 0)
				throw std::runtime_error("Invalid configuration file context.");
        }
	}
	if (context != 0 && this->get_error_code() != 0)
		return false;
	return true;
}

/**
 * @brief cleans the port information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_listen(std::string line)
{
	line = find_int(line, 1);
	if (line.empty())
	{
		this->get_config(this->_n_servers - 1).set_port(0);
		return (this->set_error_code(3));
	}
	this->get_config(this->_n_servers - 1).set_port(to_int(line.c_str()));
	if (this->get_config(this->get_n_servers() - 1).getHost().empty())
	{
		this->get_config(this->get_n_servers() - 1).setHost(line);
		this->get_config(this->get_n_servers() - 1).setNamePort(line);
	}
	else
	{
		this->get_config(this->get_n_servers() - 1).setHost(this->get_config(this->get_n_servers() - 1).getHost() + ":" + line);
		this->get_config(this->get_n_servers() - 1).setNamePort(this->get_config(this->get_n_servers() - 1).getNamePort() + ":" + line);
	}
}

/**
 * @brief cleans the server name information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_server_name(std::string line)
{
    std::string ip = get_word(line, 1);
    std::string name = get_word(line, 2);
    bool	is_ip = true;
    if (ip.empty())
        return (this->set_error_code(4));
    for (size_t i = 0; i < ip.length(); i++)
    {
        if (!isdigit(ip[i]) && ip[i] != '.')
        {
            is_ip = false;
			name.swap(ip);
            if (ip.empty())
                ip = findIP(name);
            break;
        }
    }
    if (!is_ip && !ip.empty())
    {
        for (size_t i = 0; i < ip.length(); i++)
        {
            if (!isdigit(ip[i]) && ip[i] != '.')
                return (this->set_error_code(4));
        }
    }
    else if (!is_ip)
		return (this->set_error_code(4));
	if (name.empty())
	{
		name = ip;
		this->get_config(this->get_n_servers() - 1).set_server_name(ip);
	}
	else
		this->get_config(this->get_n_servers() - 1).set_server_name(name);
	this->get_config(this->get_n_servers() - 1).set_addr(inet_addr(ip.c_str()));
	if (this->get_config(this->get_n_servers() - 1).getHost().empty())
	{
		this->get_config(this->get_n_servers() - 1).setHost(ip);
		this->get_config(this->get_n_servers() - 1).setNamePort(name);
	}
	else
	{
		this->get_config(this->get_n_servers() - 1).setHost(ip + ":" + this->get_config(this->get_n_servers() - 1).getHost());
		this->get_config(this->get_n_servers() - 1).setNamePort(name + ":" + this->get_config(this->get_n_servers() - 1).getNamePort());
	}
}

/**
 * @brief cleans the error pages information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_error_page(std::string line)
{
	if (this->get_config(this->get_n_servers() - 1).get_root().empty())
		return this->set_error_code(5);
	std::string error;
	error = find_int(line, 1);
	if (error.empty())
		return this->set_error_code(5);
	std::size_t pos;
	std::size_t pos2;
	pos = line.find(error);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos2 = line.find_first_of(" \r\t\b\f;", pos);
	if (pos == std::string::npos)
		return this->set_error_code(5);
	if (line[pos] == '/')
		pos += 1;
	if (pos2 != std::string::npos)
		line.erase(pos2);
	std::string path(this->get_config(this->get_n_servers() - 1).get_root() + &line[pos]);
	if (file_exists(path))
		this->get_config(this->get_n_servers() - 1).set_default_error(to_int(error), path);
	else
		std::cout << RED << "Provided error page [" << path << "] does not exist, falling back to default." << RESET << std::endl;
}

/**
 * @brief cleans the client max body size information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_client_max_body_size(std::string line)
{
	line = find_int(line, 1);
	if (line.size() == 0)
		this->set_error_code(7);
	this->get_config(this->_n_servers - 1).set_client_max_body_size(to_int(line.c_str()));
}

/**
 * @brief cleans the autoindex information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_autoindex(std::string line)
{
	if (line.find("on") != std::string::npos)
		this->get_config(this->_n_servers - 1).set_autoindex(true);
	else if (line.find("off") != std::string::npos)
		this->get_config(this->_n_servers - 1).set_autoindex(false);
	else	
		this->set_error_code(8);
}

/**
 * @brief cleans the root information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_root(std::string line)
{
	line = get_value(line);
	if (line.size() == 0)
		this->set_error_code(9);
	if (line[line.size() - 1] != '/')
		line = line + "/";
	this->get_config(this->_n_servers - 1).set_root(line);
}

/**
 * @brief cleans the default index information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_index(std::string line)
{
	line = get_value(line);
	if (line.size() == 0)
		this->set_error_code(10);
	this->get_config(this->get_n_servers() - 1).set_index(line);
}

/**
 * @brief cleans paths information from the config file
 * 
 * @param line current line
 */
void ConfigParser::clean_path(std::string line)
{
	size_t pos = 0;
	size_t pos2 = 0;
	line = remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos2 = line.find_first_of(" \r\t\b\f", pos);
		this->_path.push_back(line.substr(pos, pos2 - pos));
		pos = line.find_first_not_of(" \r\t\b\f", pos2);
	}
}

/**
 * @brief cleans the extension information from the config file
 * 
 * @param line current line
 */
void			ConfigParser::clean_ext(std::string line)
{
	size_t pos = 0;
	size_t pos2 = 0;
	line = remove_end(line, ';');
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	pos = line.find_first_of(" \r\t\b\f", pos);
	pos = line.find_first_not_of(" \r\t\b\f", pos);
	while (pos != std::string::npos) {
		pos = line.find_first_not_of(" \r\t\b\f", pos);
		pos2 = line.find_first_of(" \r\t\b\f", pos);
		this->_ext.push_back(line.substr(pos, pos2 - pos));
		pos = pos2;
	}
}

/**
 * @brief cleans the upload store information from the config file
 * 
 * @param line current line
 */
void	ConfigParser::clean_upload_store(std::string line)
{
	line = get_value(line);
	if (line.size() == 0)
		this->set_error_code(9);
	if (line[line.size() - 1] != '/')
		line = line + "/";
	this->get_config(this->_n_servers - 1).set_upload_store(line);
}

/**
 * @brief adds extensions to map
 * 
 */
void	ConfigParser::addToExtMap()
{
	std::vector<std::string>::iterator it1 = this->_ext.begin();
	std::vector<std::string>::iterator it2 = this->_path.begin();
	while (it1 != this->_ext.end() && it2 != this->_path.end())
	{
		this->get_config(this->_n_servers - 1).setIntrPath(*it1, *it2);
		++it1;
		++it2;
	}
	if (it1 != this->_ext.end() || it2 != this->_path.end())
		this->set_error_code(12);
	this->_ext.clear();
	this->_path.clear();
}

/**
 * @brief error handling
 * 
 * @param err_code  error to exit with
 * @param in_file  file to close
 * @return int 
 */
int ConfigParser::exit_with_error(int err_code, std::ifstream& in_file)
{
	if (err_code == 1)
		std::cerr << RED << "ERROR: --- Could not find default configuration file at: ./webserver.config ---" << RESET << std::endl;
	else if (err_code == 2)
		std::cerr << RED << "ERROR: --- Could not find a valid ** server {} ** context in the provided configuration file ---" << RESET << std::endl;
	else if (err_code == 3)
		std::cerr << RED << "ERROR: --- Could not find a valid ** port ** configuration ---" << RESET << std::endl;
	else if (err_code == 4)
		std::cerr << RED << "ERROR: --- Could not find a valid ** host ** configuration ---" << RESET << std::endl;
	else if (err_code == 5)
		std::cerr << RED << "ERROR: --- Could not find a valid ** error_page ** configuration ---" << RESET << std::endl;
	else if (err_code == 6)
		std::cerr << RED << "ERROR: --- An empty ** server_name ** was provided ---" << RESET << std::endl;
	else if (err_code == 7)
		std::cerr << RED << "ERROR: --- Could not find a valid ** client_max_body_size ** configuration ---" << RESET << std::endl;
	else if (err_code == 8)
		std::cerr << RED << "ERROR: --- Could not find a valid ** autoindex ** configuration ---" << RESET << std::endl;
	else if (err_code == 9)
		std::cerr << RED << "ERROR: --- Could not find a valid ** root ** configuration ---" << RESET << std::endl;
	else if (err_code == 10)
		std::cerr << RED << "ERROR: --- Could not find a valid ** index ** configuration ---" << RESET << std::endl;
	else if (err_code == 11)
		std::cerr << RED << "ERROR: --- An invalid ** location ** context was provided ---" << RESET << std::endl;
	else if (err_code == 12)
		std::cerr << RED << "ERROR: --- An invalid ** CGI ** context was provided ---" << RESET << std::endl;
	else if (err_code == 13)
		std::cerr << RED << "ERROR: --- Could not find a valid ** allowed_methods ** configuration ---" << RESET << std::endl;
	else if (err_code == 14)
		std::cerr << RED << "ERROR: --- Could not find a valid ** cgi_path ** configuration ---" << RESET << std::endl;
	else if (err_code == 15)
		std::cerr << RED << "ERROR: --- Could not find a valid ** cgi_ext ** configuration ---" << RESET << std::endl;
	else if (err_code == 28)
		std::cerr << RED << "ERROR: --- Could not open provided configuration file ---" << RESET << std::endl;
	in_file.close();
	this->set_error_code(err_code);
	return(err_code);
}

/**
 * @brief find ip address
 * 
 * @param word host
 * @return std::string 
 */
std::string ConfigParser::findIP(std::string &word)
{
	std::ifstream ifile;
	char	buff[256];

	ifile.open("/etc/hosts");
	if (!ifile.is_open())
		return "";
	while (ifile.getline(buff, 256))
	{
		std::string str(buff);
		if (str.find(word) != std::string::npos)
		{
			size_t pos = str.find_first_of(" \f\n\r\t\v");
			if (pos != std::string::npos)
				return str.substr(0, pos);
		}
	}
	return "";
}
