
#include "resource.h"
#include <tuple>
//在表达式中对var 和 calc 进行区分

namespace reaction{
    template<typename Type>
    class DataSource ; 

    template <typename T>
    class ExpressionTraits
    {
        //从一个数据源中，把那个resource萃取出来
        using type = T ;
    };

    template <typename T>
    class ExpressionTraits<DataSource<T>>
    {
        using type = T ;
    }

    template<typename Fun , typename... Args>
    class ExpressionTraits<DataSource<Fun , Args...>>
    {   
        /* 
        这是 C++ 模板特化代码，为DataSource<Fun, Args...>类型定制类型萃取工具ExpressionTraits。
        核心功能：通过std::invoke_result_t计算可调用对象Fun传入Args...参数后，调用的返回值类型。
        关键定义：将计算出的返回值类型定义为别名type，外部可通过ExpressionTraits<...>::ty pe获取该类型
        */

        using type = std::invoke_result_t<Fun ,typename ExpressionTraits<Args>::type...>;
    }


    template<typename Fun , typename... Args>
    using ReturnType = typename ExpressionTraits<DataSource<Fun, Args...>>::Type ; 


    /*  
    1.统一封装计算逻辑：把「函数 + 多个输入参数」打包成一个独立的 “计算资源”，
    和普通常量资源对外保持一致的接口；
    2.惰性求值：构造时只保存依赖关系，不立刻执行计算；只有主动调用 evaluate() 时才运算一次，结果永久缓存，避免重复计算；
    3.编译期类型安全：通过前置的 ExpressionTraits / ReturnType 元函数，
    在编译阶段就推导出最终结果类型，校验函数与参数的类型匹配性，运行时零类型开销。 
    */
    template<typename Fun , typename... Args>
    class Expresssion: public Resource<ReturnType<Fun, Args...>>{
        public: 
            template<typename F , typename... A>
            Expression(F &&f, A &&... args) : m_func(std::forward<F>(f)) , 
                m_args(std::forward<Args>(args)...){}
        
        protected : 
            //evaluate()：惰性求值的核心实现
            auto evaluate(){
                auto result = [&]<std::size_t ... I>(std::index_sequence<I...>){
                    return std::invoke(m_func, std::get<I>(m_args).get()...) ;
                }
                //构建临时对象给lambda传参并执行
                (std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(m_args)>>>{});
                this->updateValue(result)
            }
        private:
            Fun m_func;
            std::tuple<Args...> m_args ;
    }

    



    //复杂表达式走这个版本
    template<typename Fun , typename... Args>
    class Expression : Resource<std::invoke_result<Fun , Args...>>
    {   //std::invoke_result 获取 Fun 函数 / 可调用对象 传入 Args... 参数后的返回类型6
        public:
            template<typename F ,typename... A>
            Expression(F &&fun , A &&... args)
            :Resource<std::invoke_result_t>() , m_fun(std::forward<F>(fun)) , m_args(std::forward<A>(args)...)
            {}  //如果不手动指定，会走到Resource的默认构造中,这里已经手动指定为Resource<>()
                
        private:
            void evaluate()
            {
                std::apply(m_fun , m_args);
            }
            Fun m_fun;
            std::tuple<Args...> m_args;
    };

    


    //简单表达式，使用特化版本
    template<typename Type>
    class Expression<Type> : Resource<Type>
    {
        public:
            Expression(Type &&t) : Resource<Type>(std::forward<type>(t)){} 
        //using Resource<Type>::Resource;
                                      
            auto get() const {return Resource<Type>}
    };
}




