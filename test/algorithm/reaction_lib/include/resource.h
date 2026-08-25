#include <memory>

namespace reaction
{
    template<typename Type>
    class Resource{
        public:
            //Expression 未自定义构造，编译器自动生成默认无参构造，会隐式调用基类 Resource 的无参构造；
            //而 Resource 的指针成员 m_ptr 不会自动初始化，因此必须实现 Resource() : m_ptr(nullptr){} 既满足编译依赖，又防止野指针。
            Resource() : m_ptr(nullptr) {}
            //万能引用只能出现在函数模板中
            template<typename T>
            Resource(T &&t) : m_ptr(std::make_unique<T>(std::forward<T>(t))){}

            //存在unique_ptr时 ，禁用拷贝构造、赋值,避免两个对象同时析构时，
            //delete,m_ptr指向的对象两次
            Resource(const Resource &) = delete ;
            Resource& operator= (const Resource &) = delete ; 

            Resource(const Resource &&) = default ;
            Resource& operator= (const Resource &&) = default; 

            Type &getValue const(){
                if(!m_ptr)
                {
                    throw std::runtime_error("resource is not inlizied")
                }
                return *m_ptr;
            }
            
            template <typename T>
            void updateValue(T &&val)
            {
                if (!m_ptr){
                m_ptr = std::make_unique<Type>(std::forward<T>(t));
                }
                else{
                    *m_ptr = std::forward<T>(t);
                }
            }
            
        private:
            std::unique_ptr<Type> m_ptr;
    }
}


