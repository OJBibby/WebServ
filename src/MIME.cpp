#include "../include/MIME.hpp"


/**
 * @brief These are the allowed media types, which indicates the format of a document, file etc.
 *	Browsers use the MIME type, not the file extension, to determine how to process a URL,
 *	so it's important that web servers send the correct MIME type in the response's Content-Type header.
 * 
 */
MIME::MIME()
{
	this->_mime_types[".html"] = "Content-Type: text/html\r\n\r\n";
    this->_mime_types[".htm"] = "Content-Type: text/html\r\n\r\n";
    this->_mime_types[".css"] = "Content-Type: text/css\r\n\r\n";
    this->_mime_types[".ico"] = "Content-Type: image/x-icon\r\n\r\n";
    this->_mime_types[".avi"] = "Content-Type: video/x-msvideo\r\n\r\n";
    this->_mime_types[".bmp"] = "Content-Type: image/bmp\r\n\r\n";
    this->_mime_types[".doc"] = "Content-Type: application/msword\r\n\r\n";
    this->_mime_types[".gif"] = "Content-Type: image/gif\r\n\r\n";
    this->_mime_types[".gz"] = "Content-Type: application/x-gzip\r\n\r\n";
    this->_mime_types[".ico"] = "Content-Type: image/x-icon\r\n\r\n";
    this->_mime_types[".jpg"] = "Content-Type: image/jpeg\r\n\r\n";
    this->_mime_types[".jpeg"] = "Content-Type: image/jpeg\r\n\r\n";
    this->_mime_types[".png"] = "Content-Type: image/png\r\n\r\n";
    this->_mime_types[".txt"] = "Content-Type: text/plain\r\n\r\n";
    this->_mime_types[".mp3"] = "Content-Type: audio/mp3\r\n\r\n";
    this->_mime_types[".pdf"] = "Content-Type: application/pdf\r\n\r\n";
    this->_mime_types["default"] = "Content-Type: text/plain\r\n\r\n";
}

/**
 * @brief get type to check if it is allowed, reverts to default
 *
 * @param type 
 * @return std::string 
 */
std::string MIME::get_content_type(std::string type)
{
	if (this->_mime_types.count(type))
		return this->_mime_types[type];
	return this->_mime_types["default"];
}