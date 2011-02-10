#define CONSTANT 1.0

__kernel void starSimulation(const int numberOfGalacticCenters,
        __global float4 *stars, __global const float4 *galacticCenters)
{
    const int id = get_global_id(0);
    for (int i = 0; i < numberOfGalacticCenters; ++i)
    {
        if (stars[id].x > galacticCenters[i].x)
            stars[id].x -= 1/(stars[id].x - galacticCenters[i].x) * CONSTANT;
        else
            stars[id].x += 1/(galacticCenters[i].x - stars[id].x) * CONSTANT;

        if (stars[id].y > galacticCenters[i].y)
            stars[id].y -= 1/(stars[id].y - galacticCenters[i].y) * CONSTANT;
        else
            stars[id].y += 1/(galacticCenters[i].y - stars[id].y) * CONSTANT;

        if (stars[id].z > galacticCenters[i].z)
            stars[id].z -= 1/(stars[id].z - galacticCenters[i].z) * CONSTANT;
        else
            stars[id].z += 1/(galacticCenters[i].z - stars[id].z) * CONSTANT;
    }
}
