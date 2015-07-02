#include "engine.h"
#include "pathtrace.h"

const vec3 Engine::COLOR_NULL = vec3();
const vec3 Engine::COLOR_AMBI = vec3(0.75f, 0.87f, 0.98f);

Engine::Engine(Display *display, Camera *camera, unsigned int threads)
{
    m_threads = threads;
    m_sampler = new sampler[threads];

    for (unsigned int i = 0; i < threads; i++)
    {
        m_sampler[i].samples = new vec3[WIDTH * HEIGHT];
        m_sampler[i].samplesppx = 0;
        std::cout << "Engine: Created a sampler for thread #" << i << std::endl;
    }

    clearSamples();
    m_display = display;
    m_camera = camera;

    std::cout << "Initialized a new Engine succesfully." << std::endl;
}

Engine::~Engine()
{
    for (unsigned int i = 0; i < m_threads; i++)
    {
        delete[] m_sampler[i].samples;
    }
    delete[] m_sampler;

    std::cout << "Engine object has been destroyed succesfully." << std::endl;
}

void Engine::update(float dt)
{

}

void Engine::render(int thread, int swidth, int sheight, int xoffset, int yoffset)
{
    m_sampler[thread].samplesppx++;

    auto w = static_cast<float>(m_display->getWidth());
    auto h = static_cast<float>(m_display->getHeight());
    auto ar = static_cast<float>(m_display->getAspectRatio());
    auto r_primary = Ray();

    for (int y = yoffset; y < yoffset + sheight; y++)
    {
        for (int x = xoffset; x < xoffset + swidth; x++)
        {
            // Project the ray
            if (m_camera->getProjection() == ORTHOGRAPHIC)
            {
                Orthographic::project(&r_primary, m_camera, w, h, ar, static_cast<float>(x), static_cast<float>(y));
            }
            else if (m_camera->getProjection() == PERSPECTIVE)
            {
                Perspective::project(&r_primary, m_camera, w, h, ar, static_cast<float>(x), static_cast<float>(y));
            }

            // Do the pathtracing
            auto radiance = pathtrace(r_primary, 0);

            // Add the sample to the sampler's sample array
            m_sampler[thread].samples[x + y * WIDTH] += radiance;

            // Calculate the current average color of the pixel
            auto c = m_sampler[thread].samples[x + y * WIDTH] / static_cast<float>(m_sampler[thread].samplesppx);

            // Assign the final color to the current pixel...
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

    // Find the nearest intersection against a triangle if there's any
    if (m_scene.getKDTree().isInitialized())
    {
        m_scene.getKDTree().intersect(r, xFinal);
    }

    return xFinal;
}

void Engine::clearSamples()
{
    for (unsigned int i = 0; i < m_threads; i++)
    {
        for (unsigned int j = 0; j < WIDTH * HEIGHT; j++)
        {
            m_sampler[i].samples[j] = COLOR_NULL;
        }
        m_sampler[i].samplesppx = 0;
    }
}

int Engine::getSamplesPPX()
{
    return m_sampler[0].samplesppx;
}
