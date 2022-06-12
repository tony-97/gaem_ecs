#pragma once

struct SpawnComponent_t
{
    void (*fn_spawn)(int x, int y, float rot, void* gfact);
    float spawn_interval {  };
    float elapsed {  };
    unsigned spawned {  };
};
