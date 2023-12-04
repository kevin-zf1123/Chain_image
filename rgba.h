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
struct vct2
{
    int x;
    int y;
};

class pixel_tree_node
{
public:
    rgba pixel;
    pixel_tree_node *leftup;
    pixel_tree_node *rightup;
    pixel_tree_node *leftdown;
    pixel_tree_node *rightdown;
    vct2 sz;
    //constructor
    pixel_tree_node(rgba init,vct2 vct2) {
        // Initialize the fields of the pixel_tree_node object
        pixel.r = init.r;
        pixel.g = init.g;
        pixel.b = init.b;
        pixel.a = init.a;
        leftup = nullptr;
        rightup = nullptr;
        leftdown = nullptr;
        rightdown = nullptr;
        sz = vct2;
    }
    //destructor
    ~pixel_tree_node() {
        delete leftup;
        delete rightup;
        delete leftdown;
        delete rightdown;
    }
    void append (rgba p_left_up,rgba p_right_up,rgba p_left_down,rgba p_right_down, vct2 lu,vct2 ru, vct2 ld, vct2 rd) {
        this->leftup = new pixel_tree_node(p_left_up,lu);
        this->rightup = new pixel_tree_node(p_right_up,ru);
        this->leftdown = new pixel_tree_node(p_left_down,ld);
        this->rightdown = new pixel_tree_node(p_right_down,rd);
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
void splitstate(int &width_helper,int &height_helper,int width, int height, pixel_tree_node *node, rgba **matrix);
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

void splitstate(int& width_helper,int& height_helper, int width, int height, pixel_tree_node* node, rgba** matrix) {
    while (width_helper < width && height_helper < height) {
        int width_node = pow(2, getHighestBit(width - 1));
        int height_node = pow(2, getHighestBit(height - 1));
        vct2 lu = {width_helper, height_helper};
        vct2 ru = {width_helper + width_node, height_helper};
        vct2 ld = {width_helper, height_helper + height_node};
        vct2 rd = {width_helper + width_node, height_helper + height_node};
        vct2 lusz = {width_node,height_node};
        vct2 rusz = {width - width_node, height_node};
        vct2 ldsz = {width_node, height - height_node};
        vct2 rdsz = {width - width_node, height - height_node};
        //get average
        rgba left_up_average = *getAverage(matrix, lu.x, lu.y, lusz.x, lusz.y);
        rgba left_down_average = *getAverage(matrix, ld.x, ld.y, ldsz.x, ldsz.y);
        rgba right_up_average = *getAverage(matrix, ru.x, ru.y, rusz.x, rusz.y);
        rgba right_down_average = *getAverage(matrix, rd.x,rd.y, rdsz.x, rdsz.y);
        //subtract
        rgba left_up_vector = rgba_minus(left_up_average, *getAverage(matrix,width_helper,height_helper,width,height));
        rgba left_down_vector = rgba_minus(left_down_average, *getAverage(matrix,width_helper,height_helper,width,height));
        rgba right_up_vector = rgba_minus(right_up_average, *getAverage(matrix,width_helper,height_helper,width,height));
        rgba right_down_vector = rgba_minus(right_down_average, *getAverage(matrix,width_helper,height_helper,width,height));
        //append
        node->append(left_up_vector, right_up_vector, left_down_vector, right_down_vector,lusz,rusz,ldsz,rdsz);

        if (width_node==1 ){
            width_node >>=1;
        }else {
            width_helper += width_node;
        }
        if (height_node == 1) {
            height_node >>= 1;
        }else {
            height_helper += height_node;
        }

        splitstate(width_helper, height_helper, width_node, height_node, node->leftup, matrix);
        splitstate(width_helper, height_helper, width_node, height-height_node, node->leftdown, matrix);
        splitstate(width_helper, height_helper, width-width_node, height-height_node, node->rightdown, matrix);
        splitstate(width_helper, height_helper, width-width_node, height_node, node->rightup,matrix);
        width_node <<= 1;
        height_node <<= 1;

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