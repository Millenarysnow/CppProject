#pragma once

#include <vector>
#include <algorithm>
#include <cassert>

#define assertm(msg, expr) assert(((void)msg, expr))

namespace ecs {
    class World final
    {
    public:
    private:
        /**
         * 用于 Component 的内存池
         */
        struct Pool final
        {
            std::vector<void*> instances;
            std::vector<void*> cache;

            using CreateFunc = void*(*)(void);
            using DestroyFunc = void(*)(void*);

            CreateFunc create;
            DestroyFunc destroy;

            Pool(CreateFunc create, DestroyFunc destroy) : create(create), destroy(destroy) { }

            void* Create()
            {
                if (!cache.empty())
                {
                    instances.push_back(cache.back());
                    cache.pop_back();
                }
                else
                {
                    instances.push_back(create());
                }
                return instances.back();
            }

            void Destroy(void* elem)
            {
                if (auto it = std::find(instances.begin(), instances.end(), elem);
                    it != instances.end())
                {
                    cache.push_back(elem);
                    std::swap(*it, instances.back());
                    instances.pop_back();
                }
                else
                {
                    assertm("your elem not in pool", false);
                }
            }
        };
    };  
}
