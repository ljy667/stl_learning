#include <iostream>
#include <cstring>
#include <cstdint>

class String
{
    public:
    String()=default;
    String(const char* string)
    {
      std::cout<<"Created"<<std::endl;
      m_Size = strlen(string);
      m_Data = new char[m_Size];
      memcpy(m_Data,string,m_Size);
    }

    String(const String& other)
    {
      std::cout<<"Copyed"<<std::endl;
      m_Size = other.m_Size;
      m_Data = new char[m_Size];
      memcpy(m_Data,other.m_Data,m_Size);
    }
   
    void Print()
    {
        for(uint32_t i=0;i<m_Size;i++)
        {
            printf("%c",m_Data[i]);

            printf("\n");
        }
     }
    
    String(String&& other) noexcept
    {
       std::cout<<"Moved"<<std::endl;
      m_Size = other.m_Size;
      m_Data = other.m_Data;
      
      other.m_Size = 0;
      other.m_Data = nullptr;
    }

    ~String()
    {
        printf("Destroyed");
        delete m_Data;
    }
    private:
     char* m_Data;
     uint32_t m_Size;
};

class Entity
{
    public:
    Entity(const String& name):m_name(name)
    {
    }
    
    Entity(String&& name):m_name(std::move(name))
    {
    }

    void PrintName()
    {
        m_name.Print();
    }
    private:
    String m_name;
};
 
int main()
{
   Entity Entity("cherno");
   Entity.PrintName();
}