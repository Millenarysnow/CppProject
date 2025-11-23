#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#include "sparse_sets.h"

#define assertm(msg, expr) assert(((void)msg, expr))

namespace ecs {

    using ComponentID = uint32_t;
    using Entity = uint32_t;

    /**
	* 给定任意的类型，返回一个该类型唯一的ID
    */
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

    class Commands;
    
    class World final
    {
    public:
        friend class Commands;
		using ComponentContainter = std::unordered_map<ComponentID, void*>; // 存储组件ID对应的组件 
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

        /**
		* 存储某个 Component 类型的所有实体信息
         */
        struct ComponentInfo
        {
            Pool pool;
            SparseSets<Entity, 32> sparseSet;

            void AddEntity(Entity entity)
            {
				sparseSet.Add(entity);
            }

            void RemoveEntity(Entity entity)
            {
				sparseSet.Remove(entity);
            }
        };
        
        using ComponentPool = std::unordered_map<ComponentID, ComponentInfo>;
        ComponentPool pool_;
		std::unordered_map<Entity, ComponentContainter> entities_; // 存储实体对应的组件
    }; 

    class Commands final
    {
	public:

    };
}
