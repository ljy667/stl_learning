namespace reaction
{

    template <typename T , typename... Args>
    class DataSource
    //class DataSource : public Expression<T , Args...>  继承的话不会有开销
    {
        public:
            auto get() const {

            }
        //如果DateSource不继承Expression , 组合的对象至少有一个指针的开销
        private:
            Expression<T , Args...>*  m_expr;
    };

    template <typename T>
    auto Var(T &&t){
       return DataSource<T>(std::forward<T> t);
    }

    template<typename Fun , typename... Args>
    auto calc(Fun &&fun , Args &&... args){
        return DataSource<Fun , Args...>(std::forward<T> (fun) , std::forward<Args>(args)...)
    }
}