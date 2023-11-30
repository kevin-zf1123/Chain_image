#ifndef RGBA_H
#define RGBA_H
#include <cmath>
struct rgba
{
    float r;
    float g;
    float b;
    float a;
};


class pixel_tree_node
{
public:
    rgba pixel;
    pixel_tree_node *left;
    pixel_tree_node *right;
    int width;
    int height;
    //constructor
    pixel_tree_node(rgba init,int w,int h) {
        // Initialize the fields of the pixel_tree_node object
        pixel.r = init.r;
        pixel.g = init.g;
        pixel.b = init.b;
        pixel.a = init.a;
        left = nullptr;
        right = nullptr;
        width = w;
        height = h;
    }
    //destructor
    ~pixel_tree_node() {
        delete left;
        delete right;
    }
    void append (rgba p_left,rgba p_right,int r_w, int r_h,int l_w, int l_h) {
        this->left = new pixel_tree_node(p_left,l_w,l_h);
        this->right = new pixel_tree_node(p_right,r_w,r_h);
    }
};
class pixel_tree
{
private:
    pixel_tree_node *root;
public:
    //destructor
    ~pixel_tree() {
        delete root;
    }
    //constructor
    pixel_tree() {
        root = nullptr;
    }
    pixel_tree_node *getRoot() {
        return root;
    }
};
rgba *getAverage(rgba**matrix,int x,int y,int width,int height);
int getHighestBit(int n);
bool splitstate(int width_helper,int height_helper,int width, int height, pixel_tree_node *node, rgba **matrix, bool direction);
rgba rgba_minus(rgba a, rgba b);
rgba *getAverage(rgba**matrix,int x,int y,int width,int height)
{
    rgba *avg = new rgba;
    avg->r = 0;
    avg->g = 0;
    avg->b = 0;
    avg->a = 0;
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i < 0 || i >= width || j < 0 || j >= height) {
                continue;
            }
            avg->r += matrix[i][j].r;
            avg->g += matrix[i][j].g;
            avg->b += matrix[i][j].b;
            avg->a += matrix[i][j].a;
            count++;
        }
    }
    avg->r /= count;
    avg->g /= count;
    avg->b /= count;
    avg->a /= count;
    return avg;
};

int getHighestBit(int n) {
    int highestBit = 0;
    while (n > 0) {
        n >>= 1;
        highestBit++;
    }
    return highestBit;
}

bool splitstate(int width_helper,int height_helper,int width, int height, pixel_tree_node *node, rgba **matrix, bool direction){
    rgba left_average;
    rgba right_average;
    rgba left_vector;
    rgba right_vector;
    while (width_helper <= width && height_helper <= height){
        int width_node = pow(2, getHighestBit(width-1));
        int height_node = pow(2,getHighestBit(height-1));
        switch (direction){
            case 0:
            left_average = *getAverage(matrix,width_helper,height_helper,width_node,height_node);
            right_average = *getAverage(matrix,width-width_helper,height_helper,width-width_node,height_node);
            left_vector = rgba_minus(left_average,*getAverage(matrix,width,height,width_node,height_node));
            right_vector = rgba_minus(right_average, *getAverage(matrix,width,height,width_node,height_node));
            node->append(left_average,right_average,width_node,height_node,width-width_node,height_node);
            if (width_node != 1){
                width_node >>=1;
            }else{
                ++width_helper;
            }
            break;
            case 1:
            left_average = *getAverage(matrix,width_helper,height_helper,width_node,height_node);
            right_average = *getAverage(matrix,width_helper,height-height_helper,width_node,height-height_node);
            left_vector = rgba_minus(left_average,*getAverage(matrix,width,height,width_node,height_node));
            right_vector = rgba_minus(right_average, *getAverage(matrix,width,height,width_node,height_node));
            node->append(left_average,right_average,width_node,height_node,width_node,height-height_node);
            if (height_node != 1){
                height_node >>=1;
            }else{
                ++height_helper;
            }
            break;
        }
        direction = !direction;
        //recursions
        direction = splitstate(width_helper,height_helper,width_node,height_node,node->left,matrix,direction);
        
        switch (direction){
            case 0:
            width_node <<=1;
            direction = splitstate(width_helper,height_helper,width-width_node,height_node,node->right,matrix,direction);
            break;
            case 1:
            height_node <<=1;
            direction = splitstate(width_helper,height_helper,width_node,height-height_node,node->right,matrix,direction);
            break;
        }
    }
}

rgba rgba_minus(rgba a, rgba b) {
    rgba c;
    c.r = a.r - b.r;
    c.g = a.g - b.g;
    c.b = a.b - b.b;
    c.a = a.a - b.a;
    return c;
}
#endif