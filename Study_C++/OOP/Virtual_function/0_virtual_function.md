# `Virtual` function
- Là hàm được khai báo trong lớp cơ sở (class base) và được xác định lại (overridden) trong lớp dẫn xuất (class derived).
- Khi bạn tham chiếu đến một đối tượng của lớp dẫn xuất bằng một con trỏ hoặc một tham chiếu đến lớp cơ sở, bạn có thể gọi một hàm ảo cho đối tượng đó và thực thi phiên bản phương thức của lớp dẫn xuất.
- **Virtual function** đảm bảo rằng hàm được gọi một cách chính xác cho một đối tượng, bất kể loại tham chiếu (hoặc con trỏ) được sử dụng lệnh gọi hàm. Chúng chủ yếu được sử dụng để đạt được tính đa hình trong thời gian chạy.
- Các hàm được khai báo bằng từ khóa `virtual` trong lớp cơ sở.

## Rule for `virtual function`:
- `virtual function` không được có keyword `static`.
- `virtual function` có thể là hàm bản của một class khác.
- `virtual function` phải được truy cập bằng con trỏ hoặc tham chiếu của lớp cơ sở để đạt được tính đa hình.
- Prototype của `virtual function` phải giống nhau ở cả base class và derived class.
- Luôn định nghĩa trong base class và được ghi đè (overriden) trong derived class.derived class không bắt buộc phải ghi đè (hoặc re-define lại `virtual function`), trong trường hợp đó phiên bản của base class sẽ được sử dụng.
- A class có thể có `virtual destructor` nhưng nó không thể có `virtual constructor`.

## Compile time (early binding) VS runtime (late binding) behavior of Virtual Functions
**Example**
```C++
#include <iostream>

using namespace std;

class base{
public:
    virtual void print(){
        cout << "print base class\n";
    }

    void show() { 
        cout << "show base class\n"; 
    }
};
 
class derived : public base {
public:
    void print() { 
        cout << "print derived class\n"; 
    }
 
    void show() { 
        cout << "show derived class\n"; 
    }
};

int main(){

    base* b_ptr;
    derived d;
    b_ptr = &d;
 
    // Virtual function, binded at runtime
    b_ptr->print();
 
    // Non-virtual function, binded at compile time
    b_ptr->show();
 
    return 0; 
}
```
**Output**:
```
print derived class
show base class
```
**Explantion**:
> Tính đa hình(polymorphism) chỉ đạt được thông qua con trỏ hoặc tham chiếu của loại base class. Ngoài ra con trỏ base class có thể trỏ đến các đối tưởng của base class và devired class.
> Trong đoạn code trên, con trỏ `b_ptr` chứa địa chỉ của đối tượng `d` của class derived.
> Liên kết muộn - late binding (runtime) được thực hiện bởi con trỏ (vị trí mà con trỏ trỏ tới) và liên kết sớm - early binding (complie time) được thực hiện theo loại con trỏ.
> hàm `print()` được khai báo là `virtual function` nó sẽ ràng buộc khi chạy (runtime) (output là derived class khi con trỏ trỏ đến đối tượng của derived class) và hàm `show()` là `non-virtual function` nó sẽ ràng buộc trong thời gian biên dịch (output là base class).

*Note: Nếu chúng ta tạo `virtual function` trong base class và nó được overridden trong derived class, chúng ta không cần keyword `virtual` trong derived class, functions được tự động là `virtual functions` trong derived class.*

## Working of Virtual Functions (concept of VTABLE and VPTR)