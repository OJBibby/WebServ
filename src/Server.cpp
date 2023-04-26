#include "../include/Server.hpp"

/**
 * @brief Construct a new Server:: Server object
 * 
 * @param config 
 */
Server::Server(Config config): _config(config), _error(0)
{
	bzero(&_serv_addr, sizeof(sockaddr_in));
	this->_port = this->_config.get_port();
	this->_serv_addr.sin_family = AF_INET;
    this->_serv_addr.sin_port = htons(_port);
    this->_serv_addr.sin_addr = this->_config.get_addr();
}

/**
 * @brief Destroy the Server:: Server object
 * 
 */
Server::~Server() {}

void	Server::initServer()
{
	this->init_socket();
	this->bind_socket();
	this->listen_socket();
}

/**
 * @brief initiate server socket
 * 
 * @return int 
 */
int	Server::init_socket()
{
	int optval = 1;
	if ((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::logic_error("Could not create socket.");
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
        throw std::logic_error("Could not set socket options.");
	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::logic_error("Could not set socket flags.");
	return EXIT_SUCCESS;
}

/**
 * @brief bind socket to address
 * 
 * @return int 
 */
int	Server::bind_socket()
{
    if (bind(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) < 0)
		throw std::logic_error("Could not bind socket to address.");
	return EXIT_SUCCESS;
}

/**
 * @brief listen to socket
 * 
 * @return int 
 */
int	Server::listen_socket()
{
	if (listen(_sockfd, MAX_CONN) < 0)
		throw std::logic_error("Listen failed.");
	return EXIT_SUCCESS;
}

/* ---------- GETTERS ---------- */

/**
 * @brief get server error number
 * 
 * @return int 
 */
int	Server::getError() const
{
	return _error;
}

/**
 * @brief get server socket fd
 * 
 * @return int 
 */
int Server::get_sockfd() const
{ 
	return _sockfd;

}

/**
 * @brief get port being listened to 
 * 
 * @return int 
 */
int Server::get_port() const 
{ 
    return _port; 
}

/**
 * @brief get servers configuration
 * 
 * @return Config& 
 */
Config &Server::get_config()
{
	return _config;
}

/**
 * @brief close server socket
 * 
 * @return int 
 */
int		Server::clean_fd()
{
	int    fd;

	fd = fcntl(this->get_sockfd(), F_GETFL);
	if (fd != -1)
		close(this->get_sockfd());
	return EXIT_SUCCESS;
}
