# Class

## Giới thiệu và các tính chất của Class

- Tham khảo [tại đây](https://www.w3schools.com/cpp/cpp_oop.asp)

## Constructor
- Constructor là một thành phần đặc biệt của class được complier gọi mỗi khi object được khởi tạo. có 3 loại constructor:
    - Default constructor
    - Parameterized constructor
    - Copy constructor
- Copy constructor tạo một object mới, là một bản sao của một object hiện tại.

**Syntax:**
```C++
class_name (class_name &){}
```
**Example**
```C++
#include <iostream>

using namespace std;

class Test{
    public:
    int id;

    //Default constructor

    Test(){
        cout << "Default constructor called\n";
        id = -1;
    }

    //Parameterized Constructor 
    Test(int x) 
    { 
        cout <<"Parameterized Constructor called \n"; 
        id = x; 
    } 

    // Copy constructor
    Test(Test &obj){
        cout << "Copy Constructor called\n";
        id = obj.id;
    }
};


int main(){

    // obj1 will call Default Constructor
    Test obj1;

    cout << "Test id: " << obj1.id << endl;

    // obj2 will call Parameterized Constructor 
    Test obj2(10);
    cout << "Test id: " << obj2.id << endl;

    // obj3 will call Copy Constructor 
    Test obj3(obj2);
    cout << "Test id: " << obj3.id << endl;
    return 0;
}
```

## Destructors
- Destructor là một thành phần đặc biệt của class được complier gọi khi phạm vi object kết thúc.

**Example:**
```C++
#include <iostream>

using namespace std;

class Test{
    public:
    int id;

    //Definition for Destructor 

    ~Test(){
        cout << "Destructor called for id:" << id << endl;
    }
};


int main(){
    
    Test obj1;
    obj1.id = 10;

    for (int i = 0; i < 5; i++)
    {
        Test obj2;
        obj2.id = i;
    }   // Scope for obj2
    
    return 0;
}       // Scope for obj1
```

## Note linh tinh

```C++
#include <iostream> 
using namespace std; 
  
class Demo{ 
   int a, b; 
    public: 
    Demo()   // default constructor 
    { 
        cout << "Default Constructor" << endl; 
    } 
    Demo(int a, int b):a(a),b(b)  //parameterised constructor 
    { 
        cout << "parameterized constructor -values" << a  << " "<< b << endl; 
    } 
      
}; 
  
  
int main() { 
     
    return 0; 
}
```

*Note: `Demo(int a, int b):a(a),b(b)` ở đây có sử dụng member initializer list có những tác dụng sau:*
*- Tăng hiệu suất*
*- Thứ tự khởi tạo*
*- Đây là cách duy nhất để khởi tạo các trường const hoặc reference. Vì không thể gán giá trị cho copy constructor*