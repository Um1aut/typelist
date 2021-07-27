#include<iostream>
#include <type_traits>
//main struct for multiple typenames, btw it won't work with classes
template<typename... Args>
struct TypeList {
};

//get first typelist element
template<typename List>
struct FirstE;

template<typename Last, typename... First>
struct FirstE<TypeList<Last, First...>> {
	typedef Last type;	
};

template<typename List>
using FirstT = typename FirstE<List>::type;

//pop First Element, then push
template<typename List>
struct popFirst;

template<typename Last, typename... First>
struct popFirst<TypeList<Last, First...>> {
	using type = TypeList<First...>;
};

template<typename List>
using popFirstT = typename popFirst<List>::type;

//push

template<typename List, typename Element>
struct push;

template<typename... Elems, typename Element>
struct push<TypeList<Elems...>, Element> {
	using type = TypeList<Element, Elems...>;
};

template<typename List, typename Element>
using pushT = typename push<List, Element>::type;


//indexing
template <typename List, unsigned Index>           
struct Element
{
    using type = typename Element<typename popFirst<List>::type, Index - 1>::type;
};

template <typename List>
struct Element<List,0>                             
{
    typedef typename FirstE<List>::type type;
};

template <typename List, unsigned Index>
struct ElementI : ElementI<popFirstT<List>,Index - 1>
{};

template <typename List>
struct ElementI<List,0> : FirstE<List>
{};

template <typename List, unsigned Index>
using ElementT = typename Element<List,Index>::type;

template <typename List>
struct Back;

template <typename... Args>
struct Back<TypeList<Args...>>
{
    using type = typename Element<TypeList<Args...>, sizeof...(Args) - 1>::type;
};

int main() {
	TypeList<int, double, bool> tl;

    static_assert(std::is_same<typename popFirst<decltype(tl)>::type,TypeList<double,bool>>::value, "not same");
    static_assert(std::is_same<typename push<decltype(tl),float>::type,TypeList<float,int,double,bool>>::value, "not same");
    static_assert(std::is_same<typename Back<decltype(tl)>::type,bool>::value, "not same");
    static_assert(std::is_same<typename ElementI<decltype(tl),0>::type,int>::value, "not same");
    static_assert(std::is_same<typename ElementI<decltype(tl),1>::type,double>::value, "not same");
    static_assert(std::is_same<ElementT<decltype(tl),2>,bool>::value, "not same");	
		
	return 0;
}
