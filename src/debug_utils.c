/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: debug <debug@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:50:00 by debug             #+#    #+#             */
/*   Updated: 2025/06/25 13:50:00 by debug            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>  // For getcwd

void debug_list_textures(const char *path) {
    DIR *dir;
    struct dirent *entry;
    
    printf("\n=== Listing directory: %s ===\n", path);
    
    dir = opendir(path);
    if (!dir) {
        printf("ERROR: Could not open directory: %s\n", path);
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".png") || strstr(entry->d_name, ".jpg") || 
            strstr(entry->d_name, ".jpeg") || strstr(entry->d_name, ".bmp")) {
            printf("Found texture: %s\n", entry->d_name);
        }
    }
    
    closedir(dir);
    printf("=== End of directory listing ===\n\n");
}

void debug_print_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}
