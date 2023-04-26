#!/bin/bash

array[0]="tux"
array[1]="hellokitty"
array[2]="bunny"
array[3]="bud-frogs"
array[4]="moose"
array[5]="sheep"
array[6]="three-eyes"
array[7]="apt"
array[8]="unipony-smaller"
array[9]="fox"
array[10]="stegosaurus"

index=$(($RANDOM % ${#array[@]}))

if (( RANDOM % 3 ))
then
	cow=`/usr/games/fortune | /usr/games/cowsay`
else
	cow=`/usr/games/fortune | /usr/games/cowsay -f ${array[$index]}`
fi

message="<html>
<head>
<meta charset="UTF-8">
<link href=/utils/cowstyle.css rel=stylesheet type=text/css>
<title>Fortune</title>
</head>
<body>
<pre>
$cow
</pre>
<form METHOD=GET ACTION=\"/contact\">
<button type=\"submit\">Get new fortune.</button>
</form>
<form METHOD=GET ACTION=\"/\">
<button type=\"submit\">Homepage</button>
</form>
<form METHOD=POST ACTION=\"/cgi-bin/delete.sh\" enctype=multipart/form-data>
<button type=\"submit\">Purge the cow from this Earth</button>
<input type=hidden value=/cgi-bin/cowsay.sh name=file-to-delete>
</form>
</body>
</html>
"
echo "HTTP/1.1 200 OK"
echo "Content-Length: ${#message}"
echo -e "Content-type: text/html\r\n\r"
echo -en "$message"

exit 0
