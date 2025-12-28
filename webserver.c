#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // 1. Socket erstellen & konfigurieren
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // 2. An Port binden und lauschen
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("🚀 Hoster aktiv! Warte auf Anfragen auf http://192.168.64.2:8080\n");

    while(1) {
        // 3. Verbindung annehmen
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        // 4. index.html von der Festplatte öffnen
        FILE *html_file = fopen("index.html", "r");
        if (html_file == NULL) {
            char *error = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nDatei nicht gefunden!";
            write(new_socket, error, strlen(error));
        } else {
            // 5. Den richtigen Header senden (Wichtig für die Anzeige!)
            char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            write(new_socket, header, strlen(header));
            
            // 6. Dateiinhalt einlesen und senden
            char buffer[4096];
            int bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), html_file)) > 0) {
                write(new_socket, buffer, bytes_read);
            }
            fclose(html_file);
        }
        
        printf("✅ Anfrage bearbeitet und Webseite ausgeliefert.\n");
        close(new_socket);
    }
    return 0;
}
