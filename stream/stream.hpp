//==================================================================================================
// 文件：stream.hpp
// 作者：顾宇浩
// 简介：流抽象模板
// 上次修改：2020-5-10 (v2.1.1)
//==================================================================================================

#pragma once



//==================================================================================================
// 类名：IStream（纯虚）（模板）
// 参数：T - 流元素类型
// 说明：同步输入流接口
//==================================================================================================
template <typename T>
class IStream
{
public:
	//==============================================================================================
	// 函数：get
	// 参数：p - 存放取出元素的指针
	// 返回：成功返回true；失败返回false
	// 说明：从流中取出一个元素
	//==============================================================================================
	virtual bool get(T* p) = 0;
};



//==================================================================================================
// 类名：OStream（纯虚）（模板）
// 参数：T - 流元素类型
// 说明：同步输出流接口
//==================================================================================================
template <typename T>
class OStream
{
public:
	//==============================================================================================
	// 函数：put
	// 参数：t - 放入元素
	// 返回：成功返回true；失败返回false
	// 说明：往流中放入一个元素
	//==============================================================================================
	virtual bool put(T t) = 0;
};



//==================================================================================================
// 类名：AsyncIStream（纯虚）（模板）
// 参数：T - 流元素类型
// 说明：异步输入流接口
//==================================================================================================
template <typename T>
class AsyncIStream :virtual public IStream<T>
{
public:
	//==============================================================================================
	// 函数：get
	// 参数：p - 存放取出元素的指针
	// 返回：成功返回true；失败返回false
	// 说明：AsyncIStream默认的阻塞型从流中取出一个元素的方法，注意在流彻底断开的情况下会导致死循环。
	//==============================================================================================
	virtual bool get(T* p);

	//==============================================================================================
	// 函数：try_get
	// 参数：p - 存放取出元素的指针
	// 返回：成功返回true；失败返回false
	// 说明：尝试从流中取出一个元素，失败时p指向内容不被修改。
	//==============================================================================================
	virtual bool try_get(T* p) = 0;
};



//==================================================================================================
// 类名：AsyncOStream（纯虚）（模板）
// 参数：T - 流元素类型
// 说明：异步输出流接口
//==================================================================================================
template <typename T>
class AsyncOStream :virtual public OStream<T>
{
public:
	//==============================================================================================
	// 函数：put
	// 参数：t - 放入元素
	// 返回：成功返回true；失败返回false
	// 说明：AsyncOStream默认的阻塞型往流中放入一个元素的方法，注意在流彻底断开的情况下会导致死循环。
	//==============================================================================================
	virtual bool put(T t);

	//==============================================================================================
	// 函数：try_put
	// 参数：t - 放入元素
	// 返回：成功返回true；失败返回false
	// 说明：尝试往流中放入一个元素
	//==============================================================================================
	virtual bool try_put(T t) = 0;
};



//==================================================================================================
// code: AsyncIStream<T>
//==================================================================================================

template<typename T>
inline bool AsyncIStream<T>::get(T* p)
{
	while (!try_get(p));
	return true;
}



//==================================================================================================
// code: AsyncOStream<T>
//==================================================================================================

template<typename T>
inline bool AsyncOStream<T>::put(T t)
{
	while (!try_put(t));
	return true;
}
