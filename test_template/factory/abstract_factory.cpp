#include <vector>

//数据库访问有关的基类

class IDBConnection
{

};


//工厂
class IDBConnectionFactory{
    public:
       virtual IDBConnection* CreateDBConnection()=0;
       virtual ~IDBConnectionFactory()=default;
};

class IDBCommand
{

};

//工厂
class IDBCommandFactory{
    public:
       virtual IDBCommand* CreateDBCommand()=0;
       virtual ~IDBCommandFactory()=default;
};


class IDBDataReader
{

};

//工厂
class IDBDataReaderFactory{
    public:
       virtual IDBDataReader* CreateDBDataReader()=0;
       virtual ~IDBDataReaderFactory()=default;
};


//=====================================================
//sql
class SqlConnection:public IDBConnection
{

};

class SqlCommand:public IDBCommand
{

};

class SqlDataReader:public IDBDataReader
{

};


class SqlDBFactory:public IDBConnectionFactory
{
    public:
       IDBConnection* CreateDBConnection() override
       {
           return new SqlConnection();
       }
};

class SqlCommandFactory:public IDBCommandFactory
{
    public:
       IDBCommand* CreateDBCommand() override
       {
           return new SqlCommand();
       }
};

class SqlDataReaderFactory:public IDBDataReaderFactory
{
    public:
       IDBDataReader* CreateDBDataReader() override
       {
           return new SqlDataReader();
       }
};


//=====================================================
//Oracle


//=====================================================



class EmployeeDAO
{
    //设计两个多态
    //用工厂创建对象，抽象工厂到多态工厂，用多态工厂创建对象
    IDBConnectionFactory* dbConnectionFactory;
    IDBCommandFactory* dbCommandFactory;
    IDBDataReaderFactory* dbDataReaderFactory;


    public:
    std::vector<EmployeeDAO> GetEmployees()
    {

        //目前具有关联性，connection和connection目前必须对应
        //IDBConnection* connection = new SqlConnection();
        IDBConnection* connection = dbConnectionFactory->CreateDBConnection();
        connection->ConnectionString("connection str");

        IDBCommand* command = dbCommandFactory->CreateDBCommand();
        command->CommandText("select * from Employee");
        command->SetConnection(connection);

        IDBDataReader* dataReader =command->ExecuteReader();

    }

};

