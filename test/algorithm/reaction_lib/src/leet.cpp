namespace reaction
{
    template <typename T , typename... Args>
    class DataSource
    {
        public:
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