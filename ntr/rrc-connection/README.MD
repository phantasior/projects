make build
1st terminal: ./build/server
2nd terminal: ./build/client

connection_request имеет метод CreateConnectionRequest, который создает структуру RRCConnectionRequest
потом она в том же методе кодируется, и возвращается структура с указателем на закодированную память + ее длину
аналогично все в файлах connection_setup и connection_setup_complete

есть два скрипта: server.c и client.c
клиент по localhost подключается к серверу, отправляет connection_request, 
сервер в свою очередь отправляет в ответ connection_setup, и потом клиент отвечает
на запрос ответом connection_setup_complete, все сопровождается логами
