#define CONSTANT 0.01


__kernel void starSimulation(const int numberOfGalacticCenters,
        __global float4 *stars, __global const float4 *galacticCenters,
        __global float4 *starSpeed, const float tijd)
{
    const int id = get_global_id(0);
#if 1

    // increase speed
    for (int i = 0; i < numberOfGalacticCenters; ++i)
    {
        if (stars[id].x > galacticCenters[i].x)
            starSpeed[id].x -= 1/(stars[id].x - galacticCenters[i].x) * CONSTANT;
        else
            starSpeed[id].x += 1/(galacticCenters[i].x - stars[id].x) * CONSTANT;

        if (stars[id].y > galacticCenters[i].y)
            starSpeed[id].y -= 1/(stars[id].y - galacticCenters[i].y) * CONSTANT;
        else
            starSpeed[id].y += 1/(galacticCenters[i].y - stars[id].y) * CONSTANT;

        if (stars[id].z > galacticCenters[i].z)
            starSpeed[id].z -= 1/(stars[id].z - galacticCenters[i].z) * CONSTANT;
        else
            starSpeed[id].z += 1/(galacticCenters[i].z - stars[id].z) * CONSTANT;
    }
    // position from speed
    stars[id].xyz += starSpeed[id].xyz * 0.001;
#else
    stars[id].z =  sin(tijd + (stars[id].x)) + cos(tijd + (stars[id].y));
#endif
}
