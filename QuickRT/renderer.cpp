#include "renderer.h"
#include "blend.h"

#include <queue>
#include <numeric>

#include "random.h"

int Renderer::render(const Scene& scene, cv::Mat& canvas) {
    if (canvas.empty())return 1;

    screen_cache = scene.camera.get_screen();

    std::queue<RenderTask> render_queue;
    std::vector<std::vector<Vec3> > pixel_samples;
    pixel_samples.resize(canvas.cols * canvas.rows);

    std::vector<double> rand_x, rand_y;
    int sqrt_sample_num = anti_alias;
    rand_x.resize(sqrt_sample_num * sqrt_sample_num);
    rand_y.resize(sqrt_sample_num * sqrt_sample_num);

    for (int y = 0; y < canvas.rows; y++)
        for (int x = 0; x < canvas.cols; x++) {
            if (sqrt_sample_num == 0) {
                render_queue.push({ y * canvas.cols + x, (double)x / canvas.cols, (double)y / canvas.rows });
            }
            else {
                blue_noise_batch(anti_alias, rand_x, rand_y, 1.0 / canvas.cols, 1.0 / canvas.rows);
                for (int i = 0; i < sqrt_sample_num; i++) {
                    render_queue.push({ y * canvas.cols + x, (double)x / canvas.cols + rand_x[i], (double)y / canvas.rows + rand_y[i] });
                }
            }

        }

    while (!render_queue.empty()) {
        auto& render_task = render_queue.front();
        render_sample(scene, render_task);
        pixel_samples[render_task.p].push_back(blend_luminos(render_task.color));
        render_queue.pop();
    }

    for (int i = 0; i < canvas.rows; i++)
        for (int j = 0; j < canvas.cols; j++) {
            const auto& samples = pixel_samples[i * canvas.cols + j];
            cv::Vec3f sum = std::accumulate(samples.begin(), samples.end(), Vec3());
            canvas.at<cv::Vec3f>(i, j) = sum / (int)samples.size();
        }

    gamma_corr(canvas, this->gamma);

    return 0;
}

void Renderer::gamma_corr(cv::Mat& image, double gamma)const {
    for (int row = 0; row < image.rows; row++) {
        float* rowptr = (float*)image.ptr(row);
        for (int col = 0; col < image.cols; col++) {
            *rowptr = powf(*rowptr, gamma);
        }
    }
}

void Renderer::render_sample(const Scene& scene, RenderTask& task) {
    Vec3 pixel = screen_cache.locate(task.cx, task.cy);
    render_ray(scene, Ray(scene.camera.pos, pixel - scene.camera.pos), task.color);
}

int Renderer::render_ray(const Scene& scene, const Ray& ray, Color& color, int reflect_times) {

    Vec3 hitpoint;
    const Object* hit_obj = trace_ray(scene, ray, hitpoint);

    if (!hit_obj) {
        color = scene.bg_color;
        return 1;
    }

    const Material* material = scene.get_material(hit_obj->material);

    // direct mode
    if (is_direct) {
        color = material->diffusion;
        return 0;
    }

    bool has_ambient = true;

    // diffusion
    Vec3 normal = hit_obj->normal(hitpoint);
    if (material->diffusion.intensity() > 0.0) {
        Vec3 obstruct_hitpoint;
        Vec3 direction = scene.light.pos - hitpoint;
        Ray shadow_ray(hitpoint + bias * direction, direction);
        const Object* obstruct = trace_ray(scene, shadow_ray, obstruct_hitpoint);
        if (!obstruct) {
            Color diffuse_color = blend_multiply(scene.light.color, material->diffusion);
            double diffuse_intensity = cv::normalize(shadow_ray.direction()).dot(normal);
            diffuse_color *= sqrt(diffuse_intensity * diffuse_intensity);
            color = blend_linear(color, diffuse_color);
            has_ambient = false;
        }
    }

    // reflect
    if (has_reflect && reflect_times < reflect_limit && material->reflection.intensity() > 0.0) {
        Color reflect_color;
        bool inside = normal.dot(ray.direction()) > 0;
        Vec3 normal_ref = inside ? -normal : normal;

        Vec3 direction = ray.direction() - 2 * normal_ref.dot(ray.direction()) * normal_ref;
        Ray reflect_ray(hitpoint + bias * direction, direction);
        render_ray(scene, reflect_ray, reflect_color, reflect_times + 1);
        reflect_color = blend_multiply(reflect_color, material->reflection);
        reflect_color *= material->reflection.intensity();
        color = blend_linear(color, reflect_color);
        has_ambient = false;

        // refract
        if (has_transparent && material->refraction.intensity() > 0.0) {
            Color refract_color;
            double n = inside ? material->refraction_index : 1.0 / material->refraction_index;
            double cos_i = -normal_ref.dot(ray.direction());
            double cos_k = sqrt(1.0 - n*n*(1 - cos_i * cos_i));
            Vec3 direction = ray.direction() * n + normal_ref * (n * cos_i - cos_k);

            Ray refract_ray(hitpoint + bias * direction, direction);
            render_ray(scene, refract_ray, refract_color, reflect_times + 1);
            
            refract_color = blend_multiply(refract_color, material->refraction);
            refract_color *= material->refraction.intensity();
            color = blend_linear(color, refract_color);
        }
    }

    // ambient
    if (has_ambient && material->ambient.intensity() > 0.0) {
        Color ambient_color = blend_multiply(scene.ambient_color, material->ambient);
        color = blend_linear(color, ambient_color);
    }

    // emission
    if (material->emission.intensity() > 0.0) {
        color = blend_linear(color, material->emission);
    }

    return 0;
}


const Object* Renderer::trace_ray(const Scene& scene, const Ray& ray, Vec3& hitpoint)const {
    double min_hit_len;
    const Object* hit_obj = nullptr;

    // get the nearest object
    for (const auto& obj : scene.objects) {
        double hit_len;
        if (obj->hit_by(ray, hit_len)) {
            if (!hit_obj || hit_len < min_hit_len) {
                hit_obj = obj;
                min_hit_len = hit_len;
            }
        }
    }

    if (!hit_obj) {
        return nullptr;
    }
    else {
        hitpoint = ray.at(min_hit_len);
        return hit_obj;
    }

}