#include "sphere.h"
#include "plane.h"
#include "polygon.h"
#include "quickrt.h"

#include <stdlib.h>

int main(int argc, char** argv) {
    
    Scene scene;

    scene.camera = Camera({ 0, 0, 5.0 }, { 0, 0, -1.0 });
    scene.camera.set_projection(3.5, 0.7, 0.5, 100.0);

    scene.light = Light(Vec3(0, 5, 5), Colors::White);

    scene.bg_color = Colors::Dark;
    scene.ambient_color = Colors::Dark;

    Material mirror_plus = Materials::Mirror;
    Material glass_plus = Materials::Glass;
    mirror_plus.diffusion = { 1.0, 1.0, 1.0, 0.1 };
    glass_plus.diffusion = { 1.0, 1.0, 1.0, 0.1 };
    scene.materials = { mirror_plus, Materials::Paper };
/*    for (int i = 0; i < 5; i++) {
        scene.materials.push_back(Materials::Glass);
    }
/*    scene.materials[1].refraction = { 1.0, 0.0, 0.0, 1.0 };
    scene.materials[2].refraction = { 1.0, 0.5, 0.5, 1.0 };
    scene.materials[3].refraction = { 0.5, 0.5, 0.5, 1.0 };
    scene.materials[4].refraction = { 0.5, 0.5, 1.0, 1.0 };
    scene.materials[5].refraction = { 1.0, 0.5, 1.0, 1.0 };

    scene.materials[1].reflection = { 1.0, 0.0, 0.0, 1.0 };
    scene.materials[2].reflection = { 1.0, 0.5, 0.5, 1.0 };
    scene.materials[3].reflection = { 0.5, 0.5, 0.5, 1.0 };
    scene.materials[4].reflection = { 0.5, 0.5, 1.0, 1.0 };
    scene.materials[5].reflection = { 1.0, 0.5, 1.0, 1.0 };*/

    Sphere obj(2.0, Vec3(-2.0, 0, 0.0), 0);
    Sphere obj2(2.0, Vec3(2.0, 0, 0.0), 1);
    Plane plane({0.0, -2.0, 0.0}, {0.0, 1.0, 0.0}, 0);
    //Triangle plane({ -5.0, -2.0, -2.0 }, { 0.0, -2.0, 2.0 }, { 5.0, -2.0, -2.0 }, 0);

    scene.objects = { &plane, &obj, &obj2 };
    /*
    for (int i = 0; i < 2; i++) {
        scene.objects.push_back(new Sphere(0.3, { rand() % 10 - 5.0, rand() % 3 - 1.0, -(rand()%5) + 3.0 }, 1 + (rand() % 5)));
    }*/


    Renderer renderer;
    renderer.has_reflect = true;
    renderer.is_direct = false;
    renderer.has_transparent = true;
    renderer.reflect_limit = 5;
    renderer.anti_alias = 0;

    renderer.gamma = 0.4;

    cv::Mat image(1080, 1920, CV_32FC3);
    renderer.render(scene, image);

    cv::namedWindow("QuickRT", cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::imshow("QuickRT", image);

    cv::Mat image_out(image.rows, image.cols, CV_8UC3);
    image.convertTo(image_out, CV_8UC3, 255.0);
    cv::imwrite("scene.png", image_out);

    cv::waitKey();
    cv::destroyAllWindows();
    
    return 0;
}