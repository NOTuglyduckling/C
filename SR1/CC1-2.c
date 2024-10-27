#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>

// Function to calculate disk usage
void calculate_disk_usage(const char *dir_name, uid_t user_id, unsigned long *total_size) {
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char path[1024];

    // Iterate through files in directory
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." directories
        }

        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        // Get file information
        if (lstat(path, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        // Check if the file belongs to the user
        if (file_stat.st_uid == user_id) {
            // Add file size to total
            *total_size += file_stat.st_size;
        }

        // If it's a directory, recurse into it
        if (S_ISDIR(file_stat.st_mode)) {
            calculate_disk_usage(path, user_id, total_size);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <username> <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *username = argv[1];
    const char *dir_name = argv[2];
    unsigned long total_size = 0;

    // Retrieve the user ID (uid) for the given username
    struct passwd *pw = getpwnam(username);
    if (pw == NULL) {
        fprintf(stderr, "User '%s' not found\n", username);
        return EXIT_FAILURE;
    }
    uid_t user_id = pw->pw_uid;

    // Calculate the total disk usage
    calculate_disk_usage(dir_name, user_id, &total_size);

    // Output the result
    printf("Total disk usage for user %s: %lu bytes\n", username, total_size);

    return 0;
}
