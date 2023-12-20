#include <stdio.h>
#include <math.h>
#include <iostream>
#include <unistd.h> // For command line arguments
#include <jpeglib.h>
#include <png.h>
#include "rgba.h"

int main(int argc, char* argv[]) {
    char *pictureName = argv[1];
    FILE* file = fopen(pictureName, "rb");
    if (file == NULL) {
        std::cout << "Failed to open the image file." << std::endl;
        return 0;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int numChannels = cinfo.num_components;

    unsigned char* imageData = new unsigned char[width * height * numChannels];
    unsigned char** rowPointers = new unsigned char*[height];

    for (int i = 0; i < height; i++) {
        rowPointers[i] = &imageData[i * width * numChannels];
    }

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &rowPointers[cinfo.output_scanline], 1);
    }



    // Convert the image data into a matrix
    rgba** matrix = new rgba*[height];
    for (int i = 0; i < height; i++) {
        matrix[i] = new rgba[width];
    }
    int index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j].r = imageData[index++] / 255.0f;
            matrix[i][j].g = imageData[index++] / 255.0f;
            matrix[i][j].b = imageData[index++] / 255.0f;
            matrix[i][j].a = 1.0f; // JPEG does not have alpha channel, set it to 1.0f
        }
    }
    // Create a pixel tree
    pixel_tree tree;
    pixel_tree_node *tree_root = tree.getRoot();
    tree_root = new pixel_tree_node(*getAverage(matrix, 0, 0, width, height), {width, height});
    tree.setRoot(tree_root);
    // Build the tree
    int width_helper = 0;
    int height_helper = 0;
    splitstate(width_helper,height_helper,width,height,tree.getRoot(),matrix);
    //print
    tree.printInorder();
    // Serialize the tree
    // tree.serializeTree(tree, "tree.txt");
    // Clean up memory
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    delete[] imageData;
    delete[] rowPointers;
    for (int i = 0; i < height; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    return 0;
}
