//把变化归结到一个具体的地方，在这里是mainform里

//抽象基类
class ISplitter
{
    public:
        virtual ~ISplitter() = default;
        virtual void split() = 0;
};

//抽象工厂基类
class SplitterFactory{
    public:
        // ISplitter* CreateSplitter()
        // {
        //     return new BinarySplitter();
        // }

        virtual ISplitter* CreateSplitter() = 0;

        //virtual ~SplitterFactory() {};
        virtual ~SplitterFactory() = default;
};

//=====================================================

class BinarySplitter : public ISplitter
{
    public:
        void split() override
        {
            // Implementation of binary splitting
        }
};


class TxTSplitter : public ISplitter
{
    public:
        void split() override
        {
            // Implementation of TXT splitting
        }
};

class BinarySplitterFactory:public SplitterFactory
{
    public:
        ISplitter* CreateSplitter() override
        {
            return new BinarySplitter();
        }
};

//工厂
class TxTSplitterFactory:public SplitterFactory
{
    public:
        ISplitter* CreateSplitter() override
        {
            return new TxTSplitter();
        }
};




//mainform本身是稳定的
//不稳定的情况是：
//ISplitter* splitter = new BinarySplitter();

//=====================================================
//现在在稳定的状态下，只需加SplitterFactory基类的子类
//并且把这个子类的实例传递给MainForm    
//=====================================================

class MainForm
{
    
    //不允许使用抽象类类型 "SplitterFactory" 的对象
    //SplitterFactory factory;
    
    SplitterFactory* factory;  // 使用指针来避免直接实例化抽象类
                               // SplitterFactory还没有具体类型
                               
    public:
    MainForm(SplitterFactory* factory) : factory(factory) {}
 
    void Button1_Click()
{
    ISplitter* splitter=factory->CreateSplitter(); //多态new
    splitter->split();

    //思路，new带来细节依赖，依赖具体类
    ISplitter* splitter = new BinarySplitter();
}

};
//每一个类都有对应的工厂


