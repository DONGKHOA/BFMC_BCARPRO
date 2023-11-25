# VECTOR
## Giới thiệu
> Không giống như array, chỉ có một số giá trị nhất định có thể lưu trữ dưới một tên biến duy nhất. Vector trong C++ giống dynamic array (có khả năng tự động thay đổi kích thước khi một phần tử được chèn hoặc xóa tùy thuộc vào nhu cầu của tác vụ được thực thi, với việc lưu trữ của chính sẽ được cùng chứa tự động xử lý). Các phần tử vector được đặt trong contiguous storage để chúng có thể được truy cập và duyệt qua bằng cách sử dụng iterator.

## So sánh `Vector` và `Dynamic array`

> Chúng đều được cấp phát bộ nhớ trong vùng nhớ heap (sau khi ảo hóa bộ nhớ).
> Đều có khả năng resize.

Ưu điểm của `vector` so với `dynamic array`:
- Memory allocation: `vector` xử lý cấp phát bộ nhớ và giải phóng bộ nhớ một cách tự động, trong khi đó `dynamic array` yêu cầu người dùng quản lý.
- Element access: `vector` truy cập phần tử bằng cách sử dụng iterator, trong khi đó `dynamic array` yêu cầu sử dụng index.

**Example:**
Index-based iteration:
```C++
for (std::size_t i = 0; i != v.size(); ++i) 
{
    // access element as v[i]

    // any code including continue, break, return
}
```
Iterator-based iteration:
```C++
for (auto it = v.begin(); it != v.end(); ++it) 
{
    // if the current index is needed:
    auto i = std::distance(v.begin(), it); 

    // access element as *it

    // any code including continue, break, return
}
```
- Sorting: `vector` được cung cấp các chức năng sắp xếp tích hợp, trong khi `array` (`dynamic` và `static`) yêu cầu sắp xếp thủ công.
- Insertion and delection: `vector` được cung cấp khả năng chèn và xóa phần tử, trong khi đó `dynamic array` phải yêu cầu dịch chuyển phần tử.
- Cache performance: `vector` cho hiệu xuất sử dụng tốt hơn.

## `Vector` có thứ tự trong C++ không?
> Không có `vector` nào không được sắp xếp trong C++. Các phần tử `vector` được đặt trong bộ nhớ liền kề để chúng có thể được truy cập và di chuyển qua các iterator. Trong `vector`, dữ liệu được chèn vào cuối. Việc chèn một phần tử vào cuối sẽ mất thời gian chênh lệch, vì đôi khi có thể cần mở rộng `vector`.

## Các `vector` được lưu trữ trong C++ như thế nào?

**Syntax**:
```C++
#include <vector>
//...
vector<object_type> variable_name;
```
**Example**:
```C++
#include <vector>
int main()
{
    std::vector<int> my_vector;
}
```
Vậy là đã có một `vector` có mỗi phần tử kiểu `int`. Sau đó có thể gán phần tử như thế này:
```C++
vector<int> my_vector = {1,2,3,4,5}
```
hoặc:
```C++
vector<int> my_vector = {1,2,3,4,5};
vector<int> your_vector = my_vector;
```
## Cơ chế ngắn chặn rò rỉ bộ nhớ của `vector`:

> Khi một `vector` thoát khỏi ra phạm vị (scope) mà chương trình chạy nó sẽ tự động giải phóng vùng nhớ mà nó kiểm soát (nếu cần). Nó giúp ích trong việc leak memory.

**Example**:
```C++
void doSomething(bool earlyExit)
{
	int *array = new int[3]{ 1, 3, 2 };

	if (earlyExit) // thoát khỏi hàm
		return;

	delete[] array; // trường hợp hàm thoát sớm, array sẽ không bị xóa
}
```
Nếu biến `earlyExit` được gán giá trị là true thì mảng `array` sẽ không bao giờ được giải phóng và bộ nhớ sẽ bị rò rỉ.
Nếu như biến `array` là một `vector` thì điều này sẽ không xảy ra.

## `Vector` tự ghi nhớ độ dài của mình

> Không giống như `dynamic array` không biết độ dài của mảng nó đang trỏ tới là bao nhiêu, `std::vectors` tự theo dõi độ dài của chính nó. Chúng ta có thể lấy được độ dài của `vector` thông qua hàm `size()`:

**Example**:
```C++
#include <iostream>
#include <vector>

void printLength(const std::vector<int>* array)
{
    std::cout << "The length is: " << array->size() << '\n';
}

int main()
{
    std::vector array { 9, 7, 5, 3, 1 };
    printLength(&array);

    return 0;
}
```

**Output**:
```C++
The length is: 5
```

## Các hàm của `vector` trong C++:

**1. Modifiers**
**2. Iterators**
**3. Capacity**
**4. Element access**

### Modifiers

- `push_back()`: Hàm đẩy một phần tử vào vị trí sau cùng của `vector`. Nếu kiểu của đối tượng được truyền dưới dạng tham số trong `push_back()` không giống với kiểu của `vector` thì sẽ bị ném ra.
```C++
ten_vector.push_back(ten_cua_phan_tu);
```
- `assign()`: Nó gán một giá trị mới cho các phần tử `vector` bằng cách thay thế các giá trị cũ.
```C++
ten_vector.assign(int size, int value);
```
- `pop_back()`: Hàm `pop_back()` được sử dụng để xóa đi phần tử cuối cùng một `vector`.
```C++
ten_vector.pop_back();
```
- `insert()`: Hàm này chèn các phần tử mới vào trước phần tử trước vị trí được trỏ bởi vòng lặp. Ta cũng có thể chuyển một số đối số thứ ba, đếm số lần phần tử được chèn vào trước vị trí được trỏ.
```C++
ten_vector.insert(position, value);
```
- `erase()`: Hàm được sử dụng để xóa các phần tử tùy theo vị trí vùng chứa.
```C++
ten_vector.erase(position);
```
**hoặc**:
```C++
ten_vector.erase(start_position, end_position);
```
- `emplace()`: Nó mở rộng vùng chứa bằng cách chèn phần tử mới vào.
```C++
ten_vector.emplace(ten_vector.position, element);
```
- `emplace_back()`: Nó được sử dụng để chèn một phần tử mới vào vùng chứa `vector`, phần tử mới sẽ được thêm vào cuối `vector`.
```C++
ten_vector.emplace_back(value);
```
- `swap()`: Hàm được sử dụng để hoán đổi nội dung của một `vector` này với một `vector` khác cùng kiểu. Kích thước có thể khác nhau.
```C++
ten_vector_1.swap(ten_vector_2);
```
- `clear()`: Hàm được sử dụng để loại bỏ tất cả các phần tử của vùng chứa `vector`.
```C++
ten_vector.clear();
```

**Example**:
```C++
// Modifiers in vector 
#include <iostream> 
#include <vector> 
using namespace std; 
   
int main() 
{ 
    // Assign vector 
    vector<int> vec; 
   
    // fill the array with 7 elements: 12
    vec.assign(7, 12); 
   
    cout << "The vector elements are: "; 
    for (int i = 0; i < 7; i++) 
        cout << vec[i] << " "; 
   
    // inserts 24 to the last position 
    vec.push_back(24); 
    int s = vec.size(); 
    cout << "\nThe last element is: " << vec[s - 1]; 
 
    // prints the vector 
    cout << "\nThe vector elements after push back are: "; 
    for (int i = 0; i < vec.size(); i++) 
    cout << vec[i] << " "; 
   
    // removes last element 
    vec.pop_back(); 
   
    // prints the vector 
    cout << "\nThe vector elements after pop_back are: "; 
    for (int i = 0; i < vec.size(); i++) 
        cout << vec[i] << " "; 
   
    // inserts 10 at the beginning 
    vec.insert(vec.begin(), 10); 
   
    cout << "\nThe first element after insert command is: " << vec[0]; 
   
    // removes the first element 
    vec.erase(vec.begin()); 
   
    cout << "\nThe first element after erase command is: " << vec[0]; 
   
    // inserts at the beginning 
    vec.emplace(vec.begin(), 5); 
    cout << "\nThe first element emplace is: " << vec[0]; 
   
    // Inserts 20 at the end 
    vec.emplace_back(20); 
    s = vec.size(); 
    cout << "\nThe last element after emplace_back is: " << vec[s - 1]; 
   
    // erases the vector 
    vec.clear(); 
    cout << "\nVector size after clear(): " << vec.size(); 
   
    // two vector to perform swap 
    vector<int> obj1, obj2; 
    obj1.push_back(2); 
    obj1.push_back(4); 
    obj2.push_back(6); 
    obj2.push_back(8); 
   
    cout << "\n\nVector 1: "; 
    for (int i = 0; i < obj1.size(); i++) 
        cout << obj1[i] << " "; 
   
    cout << "\nVector 2: "; 
    for (int i = 0; i < obj2.size(); i++) 
        cout << obj2[i] << " "; 
   
    // Swaps obj1 and obj2 
    obj1.swap(obj2); 
   
    cout << "\nAfter Swap nVector 1: "; 
    for (int i = 0; i < obj1.size(); i++) 
        cout << obj1[i] << " "; 
   
    cout << "\nVector 2: "; 
    for (int i = 0; i < obj2.size(); i++) 
        cout << obj2[i] << " ";
    
    return 0;
}
```

### Iterators
- `begin()`: đặt iterator đến phần tử đầu tiên trong `vector`.
```C++
ten_vector.begin();
```
- `end()`: đặt iterator đến sau phần tử cuối cùng trong `vector`.
```C++
ten_vector.end();
```
- `rbegin()`: đặt reverse iterator (trình lặp đảo) đến phần tử cuối cùng trong `vector` (reverse begin). Nó di chuyển từ phần tử cuối cùng đến phần tử đầu tiên
```C++
ten_vector.rbegin();
```
- `rend()`: đặt reverse iterator (trình lặp đảo) đến phần tử đầu tiên trong `vector` (reverse end)

```C++
ten_vector.rend();
```
- `cbegin()`: đặt constant iterator (trình vòng lặp) đến phần tử đầu tiên trong `vector`.
```C++
ten_vector.cbegin();
```
- `cend()`: đặt constant iterator (trình vòng lặp) đến phần tử cuối cùng trong `vector`.
```C++
ten_vector.cend();
```
- `crbegin()`: đặt constant reverse iterator (trình lặp đảo liên tục) đến phần tử cuối cùng trong `vector` (reverse begin). Nó di chuyển từ phần tử cuối cùng đến phần tử đầu tiên.
```C++
ten_vector.cbregin();
```
- `crend()`: đặt constant reverse iterator (trình lặp đảo liên tục) đến phần tử đầu tiên trong `vector` (reverse end).
```C++
ten_vector.crend();
```
**Example 1**:
```C++
#include <iostream> 
#include <vector> 
   
using namespace std; 
   
int main() 
{ 
    vector<int> vec1; 
   
    for (int i = 1; i <= 10; i++) 
        vec1.push_back(i); 
   
    cout << "Understanding begin() and end() function: " << endl; 
    for (auto i = vec1.begin(); i != vec1.end(); ++i) 
        cout << *i << " "; 
 
    return 0; 
}
```

**Example 2**:
```C++
// C++ program to illustrate the 
// iterators in vector 
#include <iostream> 
#include <vector> 

using namespace std; 

int main() 
{ 
    vector<int> g1; 

    for (int i = 1; i <= 5; i++) 
        g1.push_back(i); 

    cout << "Output of begin and end: "; 
    for (auto i = g1.begin(); i != g1.end(); ++i) 
        cout << *i << " "; 

    cout << "\nOutput of cbegin and cend: "; 
    for (auto i = g1.cbegin(); i != g1.cend(); ++i) 
        cout << *i << " "; 

    cout << "\nOutput of rbegin and rend: "; 
    for (auto ir = g1.rbegin(); ir != g1.rend(); ++ir) 
        cout << *ir << " "; 

    cout << "\nOutput of crbegin and crend : "; 
    for (auto ir = g1.crbegin(); ir != g1.crend(); ++ir) 
        cout << *ir << " "; 

    return 0; 
}
```

**Example 3**:
```C++
// CPP program to illustrate working of crbegin()  
// crend() 
#include <iostream> 
#include <vector> 
using namespace std; 

int main () 
{ 
  // initializing vector with values 
  vector<int> vect = {10, 20, 30, 40, 50}; 

  // for loop with crbegin and crend 
  for (auto i = vect.crbegin(); i != vect.crend(); i++) 
    cout << ' ' << *i;  //printing results 

  cout << '\n'; 
  return 0; 
}
```
### Capacity

- `size()`: hàm sẽ trả về số lượng phần tử đang được sử dụng trong `vector`.
```C++
ten_vector.size();
```
- `max_size()`: hàm trả về số phần tử tối đa mà `vector` có thể chứa.
```C++
ten_vector.max_size();
```
- `capacity()`: hàm trả về số phần tử được cấp phát cho `vector` nằm trong bộ nhớ.
```C++
ten_vector.capacity();
```
- `resize(n)`: Hàm này thay đổi kích thước vùng chứa để nó chứa đủ n phần tử. Nếu kích thước hiện tại của `vector` lớn hơn n thì các phần tử phía sau sẽ bị xóa khỏi `vector` và ngược lại nếu kích thước hiện tại nhỏ hơn n thì các phần tử bổ sung sẽ được chèn vào phía sau vector.
```C++
ten_vector.resize(int n, int value);
```
- `empty()`: Trả về liệu vùng chứa có trống hay không, nếu trống thì trả về True, nếu có phần tử thì trả về False.
```C++
ten_vector.empty();
```
- `shrink_to_fit()`: Giảm dung lượng của vùng chứa để phù hợp với kích thước của nó và hủy tất cả các phần tử vượt quá dung lượng.
```C++
ten_vector.shrink_to_fit();
```
- `reserve(n)`: hàm cấp cho `vector` số dung lượng vừa đủ để chứa n phần tử.
```C++
ten_vector.reserve(n);
```

**Example**:
```C++
#include <iostream> 
#include <vector> 
   
using namespace std; 
   
int main() 
{ 
    vector<int> vec1; 
   
    for (int i = 1; i <= 10; i++) 
        vec1.push_back(i); 
   
    cout << "Size of our vector: " << vec1.size(); 
    cout << "\nCapacity of our vector: " << vec1.capacity(); 
    cout << "\nMax_Size of our vector: " << vec1.max_size(); 
   
    // resizes the vector size to 4 
    vec1.resize(4); 
   
    // prints the vector size after resize() 
    cout << "\nSize of our vector after resize: " << vec1.size(); 
   
    // checks if the vector is empty or not 
    if (vec1.empty() == false) 
        cout << "\nVector is not empty"; 
    else
        cout << "\nVector is empty"; 
   
    return 0; 
}
```
### Element access

- `at(g)`: Trả về một tham chiếu đến phần tử ở vị trí ‘g’ trong `vector`.
```C++
ten_vector.at(position);
```
- `data()`: Trả về một con trỏ trực tiếp đến (memory array) bộ nhớ mảng được `vector` sử dụng bên trong để lưu trữ các phần tử thuộc sở hữu của nó.
```C++
ten_vector.data();
```
- `front()`: hàm dùng để lấy ra phần tử đầu tiên của `vector`.
```C++
ten_vector.front();
```
- `back()`: hàm dùng để lấy ra phần tử cuối cùng của `vector`.
```C++
ten_vector.back();
```

**Example**:
```C++
// C++ program to illustrate the 
// element accesser in vector 
#include <bits/stdc++.h> 
using namespace std; 
  
int main() 
{ 
    vector<int> g1; 
  
    for (int i = 1; i <= 10; i++) 
        g1.push_back(i * 10); 
  
    cout << "\nReference operator [g] : g1[2] = " << g1[2]; 
  
    cout << "\nat : g1.at(4) = " << g1.at(4); 
  
    cout << "\nfront() : g1.front() = " << g1.front(); 
  
    cout << "\nback() : g1.back() = " << g1.back(); 
  
    // pointer to the first element 
    int* pos = g1.data(); 
  
    cout << "\nThe first element is " << *pos; 
    return 0; 
}
```

## Thực hiện nén nhiều giá trị bools

> `Vector` còn có một thủ thuật hay ho khác là sử dụng một cài đặt đặc biệt dành cho `vector` kiểu bool mà nó có thể nén 8 giá trị booleans vào trong chỉ một byte (amazing!!) Quá trình này đã được cài sẵn, tất cả những gì bạn cần chỉ là sử dụng nó như một tính năng hỗ trợ khi lập trình. Thủ thuật này cũng sẽ không ảnh hưởng gì đến cách mà bạn sử dụng `vector`.

```C++
#include <vector>
#include <iostream>

int main()
{
    std::vector<bool> array { true, false, false, true, true };
    std::cout << "The length is: " << array.size() << '\n';

    for (int i : array)
        std::cout << i << ' ';

    std::cout << '\n';

    return 0;
}
```