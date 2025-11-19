#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#define assertm(msg, expr) assert(((void)msg, expr))

namespace ecs {

    using ComponentID = uint32_t;
    using Entity = uint32_t;

    class IndexGetter final
    {
    public:
        template<typename T>
        ComponentID Get()
        {
            static ComponentID id = curIdx_++;
            return id;
        }
    private:
        /**
         * C++17新特性，inline修饰的static变量可以在头文件中定义，不需要在cpp文件中初始化
         */
        inline static ComponentID curIdx_ = 0;    
    };
    
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

            // 拿出元素
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

            // 删除元素
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

        
        
        using ComponentPool = std::unordered_map<ComponentID, Pool>;
        ComponentPool pool_;
    };  
}
