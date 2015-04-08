#include "engine.h"
#include "pathtrace.h"

const vec3 Engine::COLOR_NULL = vec3();
const vec3 Engine::COLOR_AMBI = vec3(0.1f, 0.1f, 0.1f);

Engine::Engine(Display *display, Camera *camera)
{
    m_samples = new vec3[WIDTH * HEIGHT];
    clearSamples();
    m_scene = Scene();
    m_display = display;
    m_camera = camera;
}

Engine::~Engine()
{
    delete[] m_samples;

    std::cout << "Engine object has been destroyed succesfully." << std::endl;
}

void Engine::update(float dt)
{

}

void Engine::render(int swidth, int sheight, int xoffset, int yoffset)
{
    int w = m_display->getWidth();
    int h = m_display->getHeight();
    float ar = m_display->getAspectRatio();
    Ray r_primary = Ray(m_camera->getTransform().getPosition());
    quaternion q = m_camera->getTransform().getRotation();
    quaternion q_inv = q.conjugate();

    // Increase samplecounter by one
    m_samplesppx++;

    for (int y = yoffset; y < yoffset + sheight; y++)
    {

        // Generate seed for RNG each row
        unsigned short Xi[3] = {0, 0, y * y * y};

        for (int x = xoffset; x < xoffset + swidth; x++)
        {
            // Pixel's color value
            vec3 radiance = vec3(0, 0, 0);

            // Tent filter for each ray's xy directions
            float r1 = 2.0f * math::pseudorand(Xi), dx = r1 < 1.0f ? std::sqrt(r1) - 1.0f : 1.0f - std::sqrt(2.0f - r1);
            float r2 = 2.0f * math::pseudorand(Xi), dy = r2 < 1.0f ? std::sqrt(r2) - 1.0f : 1.0f - std::sqrt(2.0f - r2);

            // Construct the ray's direction vector and aim it towards the virtual screen's pixel
            float x_norm = (x + dx - w * 0.5f) / w * ar;
            float y_norm = (h * 0.5f - y + dy) / h;

#if SUPERSAMPLINGLEVEL>1
            for (unsigned int i = 0; i < SUPERSAMPLINGLEVEL; i++)
            {
                // Random jitter per ray for supersampling
                float x_rng = 2 * math::pseudorand(Xi) - 1;
                float y_rng = 2 * math::pseudorand(Xi) - 1;

                // Apply the jitter to the ray dir
                vec3 v_norm = vec3(x_norm + SUPERSAMPLINGJITTER * x_rng, y_norm + SUPERSAMPLINGJITTER * y_rng, -1.0f);

                // Rotate the ray direction based on camera look direction
                quaternion w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
                quaternion r = q * w * q_inv;

                // Set the final direction of the rotated ray dir
                r_primary.setDirection(vec3(r.x, r.y, r.z));

                // Pathtrace
                radiance = pathtrace(r_primary, 0, Xi);

                // Add the sample to the samples array
                m_samples[x + y * WIDTH] += radiance / SUPERSAMPLINGLEVEL;
            }
#else
            // Create the ray's direction vector based on normalized screenspace coordinates
            vec3 v_norm = vec3(x_norm, y_norm, -1.0f);

            // Rotate the ray direction based on camera look direction
            quaternion w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
            quaternion r = q * w * q_inv;

            // Set the final direction of the rotated ray dir
            r_primary.setDirection(vec3(r.x, r.y, r.z));

            // Pathtrace
            radiance = pathtrace(r_primary, 0, Xi);

            // Add the sample to the samples array
            m_samples[x + y * WIDTH] += radiance;
#endif

            vec3 c = m_samples[x + y * WIDTH] / static_cast<float>(m_samplesppx);
            // Set the pixel color to the desired value
            m_display->setPixel(x, y, c);
        }
    }
}

Intersection Engine::intersect(const Ray &r, float t)
{
    auto xInit = invalidIntersection;
    auto xFinal = invalidIntersection;

    // Find the nearest intersection against a sphere if there's any
    for (auto &s : m_scene.getSceneSpheres())
    {
        xInit = s.intersect(r);

        if (xInit != invalidIntersection && xInit.getT() < t)
        {
            xFinal = xInit;
            t = xFinal.getT();
        }
    }

    // Find the nearest intersection against a plane if there's any
    for (auto &p : m_scene.getScenePlanes())
    {
        xInit = p.intersect(r);

        if (xInit != invalidIntersection && xInit.getT() < t)
        {
            xFinal = xInit;
            t = xFinal.getT();
        }
    }

    return xFinal;
}

void Engine::clearSamples()
{
    for (unsigned int i = 0; i < WIDTH * HEIGHT; i++)
    {
        m_samples[i] = COLOR_NULL;
    }

    m_samplesppx = 0;
}

int Engine::getSamplesPPX()
{
    return m_samplesppx;
}
