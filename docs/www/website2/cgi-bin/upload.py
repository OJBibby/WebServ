#!/usr/bin/python3

import cgi, os
try:
	form = cgi.FieldStorage()
	fileitem = form["data"]
	if fileitem.filename:
		path = os.environ["DOCUMENT_ROOT"] + os.environ["UPLOAD_PATH"]
		if os.path.exists(path) == False:
			os.mkdir(path)
		open(path + "/" + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
		message = "The file " + os.path.basename(fileitem.filename) + " was uploaded to " + path
	else:
		message = 'Uploading Failed'
		message = "<H1> " + message + " </H1>"
		message += "<form METHOD=GET ACTION=\"/\">"
		message += "<button type=\"submit\">Homepage</button>"
		message += "</form>\n</body>\n</html>"
	message = f"HTTP/1.1 200 OK\nContent-Length:{len(message)}\nContent-Type:text/html\r\n\r\n" + message
	print(message)
except KeyError:
	message = 'No file to upload'
	message = "<H1> " + message + " </H1>"
	message += "<form METHOD=GET ACTION=\"/\">"
	message += "<button type=\"submit\">Homepage</button>"
	message += "</form>\n</body>\n</html>"
	message = f"HTTP/1.1 200 OK\nContent-Length:{len(message)}\nContent-Type:text/html\r\n\r\n" + message
	print(message)
