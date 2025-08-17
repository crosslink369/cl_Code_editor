#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m"
#define YELLOW  "\x1B[33m"
#define CYAN    "\x1B[36m"
#define RESET   "\x1B[0m"

void clearScreen() {
    printf("\033[2J\033[H");
}

void animateText(const char *text, int delay_ms) {
    for (size_t i = 0; i < strlen(text); i++) {
        putchar(text[i]);
        fflush(stdout);
        usleep(delay_ms * 1000);
    }
}

int isInstalled(const char *pkg) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "command -v %s >/dev/null 2>&1", pkg);
    return system(cmd) == 0;
}

void showProgress(const char *label, int seconds, const char *pkg) {
    printf("%s: [", label);
    for (int i = 0; i < 20; i++) {
        printf("%s", isInstalled(pkg) ? GREEN "#" RESET : YELLOW ">" RESET);
        fflush(stdout);
        usleep(seconds * 50000);
    }
    printf("] %s\n", isInstalled(pkg) ? GREEN "INSTALLED" RESET : GREEN "DONE" RESET);
}

void installVSCode() {
    clearScreen();
    animateText("Setting up VS Code Server environment...\n", 30);
    
    if (!isInstalled("wget")) {
        showProgress("Installing wget", 2, "wget");
        system("pkg install wget -y && pkg install android-tools > /dev/null");
    }
    
    if (!isInstalled("gcc")) {
        showProgress("Installing GCC", 2, "gcc");
        system("pkg install gcc -y > /dev/null");
    }
    
    if (!isInstalled("proot-distro")) {
        showProgress("Installing Proot-Distro", 3, "proot-distro");
        system("pkg install proot-distro -y > /dev/null");
    }
    
    showProgress("Setting up Fedora", 5, "fedora");
    system("proot-distro install fedora > /dev/null 2>&1");
    
    animateText("\nLaunching VS Code Server...\n", 20);
    system("termux-open-url https://crossberry.vercel.app");
    system("proot-distro login fedora -- bash -c 'curl -fsSL https://code-server.dev/install.sh | sh'");
    system("proot-distro login fedora -- bash -c 'export PASSWORD=\"password\" && code-server'");
}

void installNvChad() {
    clearScreen();
    animateText("Setting up NvChad configuration...\n", 30);
    
    struct stat st = {0};
    char *home = getenv("HOME");
    char nvim_path[256];
    snprintf(nvim_path, sizeof(nvim_path), "%s/.config/nvim", home);
    
    if (stat(nvim_path, &st) == -1) {
        showProgress("Cloning NvChad", 3, "nvim");
        system("git clone https://github.com/NvChad/starter ~/.config/nvim > /dev/null 2>&1");
    }
    
    animateText("\nLaunching Neovim with NvChad...\n", 20);
    system("nvim");
}

int main() {
    clearScreen();
    printf(CYAN "####  " RED "#####   " CYAN "####   ####   ####  \n" RESET);
    printf(CYAN "#    # " RED "#    # " CYAN "#    # #      #      \n" RESET);
    printf(CYAN "#      " RED "#    # " CYAN "#    #  ####   ####  \n" RESET);
    printf(CYAN "#      " RED "#####  " CYAN "#    #      #      # \n" RESET);
    printf(CYAN "#    # " RED "#   #  " CYAN "#    # #    # #    # \n" RESET);
    printf(CYAN " ####  " RED "#    #  " CYAN "####   ####   ####  \n\n" RESET);
    
    printf("Created by: Surya Prakash\n");
    printf("Download CrossLink app: " BLUE "https://drive.google.com/file/d/1eM8uPfoeH7e0dJ4egvqoJR8A2gd8FqzK/view?usp=drivesdk\n\n" RESET);
    
    printf("Choose installation:\n");
    printf("[1] " BLUE "VS Code Server (Full development environment)\n" RESET);
    printf("[2] " GREEN "NvChad (Neovim configuration)\n\n" RESET);
    
    int choice;
    printf("Select option (1/2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        installVSCode();
    } else if (choice == 2) {
        installNvChad();
    } else {
        printf(RED "Invalid selection!\n" RESET);
        return 1;
    }
    
    printf(GREEN "\nInstallation complete!\n" RESET);
    return 0;
}
