// frame_receiver.cpp - Receive frames and process with ISP
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

#define SERVER_IP "192.168.64.2"
#define PORT 8080
#define FRAME_SIZE 614400

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char *buffer;
    ssize_t bytes_received, total_received;
    
    // Allocate buffer
    buffer = new char[FRAME_SIZE];
    if (!buffer) {
        std::cerr << "Failed to allocate buffer" << std::endl;
        return 1;
    }
    
    // 1. Create socket
    std::cout << "Creating socket..." << std::endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        delete[] buffer;
        return 1;
    }
    std::cout << "✓ Socket created" << std::endl;
    
    // 2. Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        delete[] buffer;
        return 1;
    }
    
    // 3. Connect to server
    std::cout << "Connecting to " << SERVER_IP << ":" << PORT << "..." << std::endl;
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        delete[] buffer;
        return 1;
    }
    std::cout << "✓ Connected!" << std::endl;
    
    // 4. Main loop: receive frames and process
    std::cout << "\n=== Receiving and processing frames ===" << std::endl;
    int frame_count = 0;
    
    while (true) {
        // Receive one complete frame
        total_received = 0;
        while (total_received < FRAME_SIZE) {
            bytes_received = recv(sockfd, buffer + total_received, 
                                 FRAME_SIZE - total_received, 0);
            
            if (bytes_received < 0) {
                perror("Receive failed");
                goto cleanup;
            }
            
            if (bytes_received == 0) {
                std::cout << "\nServer closed connection" << std::endl;
                goto cleanup;
            }
            
            total_received += bytes_received;
        }
        
        frame_count++;
        std::cout << "\n[" << frame_count << "] Received " << total_received 
                  << " bytes" << std::endl;
        
        // Save RAW file temporarily
        char raw_filename[64];
        snprintf(raw_filename, sizeof(raw_filename), "frame_%03d.raw", frame_count);
        
        std::ofstream raw_file(raw_filename, std::ios::binary);
        if (!raw_file) {
            std::cerr << "Failed to save RAW file" << std::endl;
            continue;
        }
        raw_file.write(buffer, FRAME_SIZE);
        raw_file.close();
        std::cout << "[" << frame_count << "] Saved RAW: " << raw_filename << std::endl;
        
        // Process with ISP Pipeline
        // Must run from ISP_Pipeline directory for relative paths to work
        char command[512];
        char output_filename[64];
        snprintf(output_filename, sizeof(output_filename), "output_%03d.png", frame_count);
        
        // Change to ISP_Pipeline directory and run isp_main
        const char* home = getenv("HOME");
        snprintf(command, sizeof(command), 
                "cd %s/Project/ISP_Pipeline && ./build/isp_main network/%s > /dev/null 2>&1", 
                home, raw_filename);
        
        std::cout << "[" << frame_count << "] Processing with ISP..." << std::endl;
        int ret = system(command);
        
        if (ret == 0) {
            // Move output.png from data/ to network/ with new name
            snprintf(command, sizeof(command), 
                    "mv %s/Project/ISP_Pipeline/data/output.png %s/Project/ISP_Pipeline/network/%s", 
                    home, home, output_filename);
            system(command);
            std::cout << "[" << frame_count << "] ✓ Saved PNG: " << output_filename << std::endl;
            
            // Optional: keep or delete RAW
            // remove(raw_filename);
        } else {
            std::cerr << "[" << frame_count << "] ✗ ISP processing failed" << std::endl;
        }
    }
    
cleanup:
    // 5. Cleanup
    std::cout << "\n=== Completed ===" << std::endl;
    std::cout << "✓ Processed " << frame_count << " frames" << std::endl;
    close(sockfd);
    delete[] buffer;
    
    return 0;
}