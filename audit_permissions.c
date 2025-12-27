#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    printf("--- Linux Security Audit Tool ---\n");

    // Check 1: Zugriff auf /etc/shadow (Passwort-Hashes)
    if (access("/etc/shadow", R_OK) == 0) {
        printf("[!] KRITISCH: /etc/shadow ist LESBAR! (Sicherheitsrisiko)\n");
    } else {
        printf("[+] OK: /etc/shadow ist geschützt.\n");
    }


    // Check 2: Läuft das Programm mit Root-Rechten?
    if (getuid() == 0) {
        printf("[!] WARNUNG: Dieses Programm läuft als ROOT (uid 0).\n");
    } else {
        printf("[+] Info: Programm läuft als normaler User (uid %d).\n", getuid());
    }

// Check 3: Gibt es ausstehende Sicherheits-Updates?
    printf("--- System Update Check ---\n");
    system("/usr/lib/update-notifier/apt-check --human-readable");

    return 0;
}
