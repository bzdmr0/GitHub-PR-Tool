#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>

#include <GLFW/glfw3.h>

// IMPORTANT: Define the implementation for the stb_image_write library
// Do this in one C file before including the header.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Screenshot function
void save_image(int width, int height) {
    // Allocate memory for pixel data (4 bytes per pixel for RGBA)
    unsigned char* pixels = (unsigned char*)malloc(width * height * 4);
    if (!pixels) {
        fprintf(stderr, "Failed to allocate memory for screenshot\n");
        return;
    }

    // Read pixels using RGBA format (most compatible with OpenGL ES)
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Generate filename with timestamp
    char filename[256] = "../images/screenshot.png";

    // Flip image vertically (OpenGL is bottom-left, PNG is top-left)
    unsigned char* flipped = (unsigned char*)malloc(width * height * 4);
    for (int y = 0; y < height; y++) {
        memcpy(flipped + (y * width * 4),
               pixels + ((height - 1 - y) * width * 4),
               width * 4);
    }

    // Save as PNG using stb_image_write
    if (stbi_write_png(filename, width, height, 4, flipped, width * 4)) {
        printf("Screenshot saved as %s\n", filename);
    } else {
        fprintf(stderr, "Failed to save screenshot: %s\n", filename);
    }

    free(pixels);
    free(flipped);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Add a static variable to prevent saving hundreds of files if the key is held down.
    static int s_key_was_pressed = 0;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !s_key_was_pressed) {
        save_image(width, height);
        s_key_was_pressed = 1;
    }
    // Reset the flag when the key is released
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        s_key_was_pressed = 0;
    }
}

#endif