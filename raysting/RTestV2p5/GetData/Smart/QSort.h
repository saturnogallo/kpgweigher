// QSort.h
//
// 包含排序算法等：QSortPtr()、QSortObj()、BSearchObj()、BSearchPtr()、SpecialSort()

#ifndef _QSort_h_
#define _QSort_h_

#ifndef NULL
	#define NULL 0
#endif

//功  能：使用标准C函数qsort()进行【对象指针数组】排序
//参  数：dwCmpPar，比较参数，传递给比较函数，意义自定义
//注  意：TYPE必须有公共成员函数
//			int TYPE::Compare(const TYPE& cmper) const;
//		  比较函数返回值满足
//		  >0，*this >  cmper
//		  =0，*this == cmper
//		  <0，*this <  cmper
template<typename TYPE> struct tagCmpFunc {
	static int qcmp(const void* p1, const void* p2)
	{	return (*(TYPE**)p1)->Compare( (const TYPE&) (**(TYPE**)p2) ); }
};
template<typename TYPE>
inline void QSortPtr(TYPE* const* ppObj, int nTail, int nHead = 0)
	{	if(nHead < nTail)
			qsort((void*)(ppObj+nHead), nTail-nHead+1, sizeof(TYPE*), tagCmpFunc<TYPE>::qcmp);
	}

//功  能：使用标准C函数qsort()进行【对象数组】排序
//		  『其它同上』
template<typename TYPE> struct tagCmpFunc2 {
	static int qcmp(const void* p1, const void* p2)
	{	return ((TYPE*)p1)->Compare( (const TYPE&) (*(TYPE*)p2) ); }
};
template<typename TYPE>
inline void QSortObj(TYPE* pObj, int nTail, int nHead = 0)
	{	if(nHead < nTail)
			qsort(pObj+nHead, nTail-nHead+1, sizeof(TYPE), tagCmpFunc2<TYPE>::qcmp);
	}


template<typename TYPE>
inline TYPE const* GetPtrType(TYPE const* v)
	{	return v;	}

template<typename TYPE>
inline TYPE const* GetPtrType(TYPE const* const* v)
	{	return *v;	}

//功  能：折半查找基本模板『同时用于 BSearchObj() 和 BSearchPtr()』
//参  数：insert返回应该插入的位置『如果查找成功，insert==返回值』
//返回值：成功范围找到的元素的下标号，失败返回-1；
//注  意： typename TYPE的比较函数原型为 int TYPE::Compare(const KEY_TYPE& cmper) const;
//		  模板参数见后面 BSearchObj() 和 BSearchPtr() 调用
template<typename SEARCH_TYPE, typename OBJ_TYPE, typename KEY_TYPE>
int BSearchTempl(SEARCH_TYPE base, const KEY_TYPE& key, int tail, int head /* = 0 */, int* insert /* = NULL*/)
{
	int nMid = 0;
	int iComp = 0;

	while(head <= tail) {
		nMid  = (head+tail) / 2;

		// 对于 GetPtrType<OBJ_TYPE>(base+nMid);
		// 在BSearchObj(...) 中会自动调用 TYPE const* GetPtrType(TYPE const* v)
		// 在BSearchPtr(...) 中会自动调用 TYPE const* GetPtrType(TYPE const* const* v)
		iComp = GetPtrType<OBJ_TYPE>(base+nMid)->Compare(key);

		if(iComp == 0) {
			if(insert) *insert = nMid;
			return nMid;
		}
		else {
			if(iComp > 0) {
				tail = nMid-1;
				iComp = -1;
			}
			else {
				head = nMid+1;
				iComp = 1;
			}
		}
	}

	if(insert) {
		if(iComp > 0)
			*insert = nMid+1;
		else
			*insert = nMid;
	}

	return -1;
}

//功    能：对象数组折半查找
//使用注意：当base实际为派生类数组，把其转化为基类数组使用时需要保证派生类中没有数据成员
template<typename TYPE, typename KEY_TYPE>
inline int BSearchObj(TYPE const* base, const KEY_TYPE& key, int tail, int head = 0, int* insert = NULL)
	{	return BSearchTempl<TYPE const*, TYPE, KEY_TYPE>(base, key, tail, head, insert);	}

//功    能：对象指针数组折半查找『对象条件同QSortPtr』
template<typename TYPE, typename KEY_TYPE>
inline int BSearchPtr(TYPE const* const* base, const KEY_TYPE& key, int tail, int head = 0, int* insert = NULL)
	{	return BSearchTempl<TYPE const* const*, TYPE, KEY_TYPE>(base, key, tail, head, insert);	}

//功  能：指定顺序排序
//参  数：base，需要排序的数组；count，个数；
//		  order，指定的顺序，里面为 0 到 count-1 的数字，每个数字有且只有一个
//返回值：
template<typename TYPE>
void SpecialSort(TYPE* base, int count, const int* order)
{
	ASSERT(count>=0);

	BYTE* p = NULL;
	TYPE* _base;

	DWORD tmpbuf[1024];
	int total_size = sizeof(TYPE)*count;
	if(total_size > sizeof(tmpbuf)) {
		p = new BYTE[sizeof(TYPE)*count];
		_base = (TYPE*)p;
	}
	else
		_base = (TYPE*)tmpbuf;

	for(int i=0; i<count; i++) {
#ifdef _DEBUG
		ASSERT(0<=order[i] && order[i]<count);
		int j;
		for(j=0; j<count; j++) {
			if(i == order[j])
				break;
		}
		ASSERT(j<count);
#endif
		memcpy(_base+order[i], base+i, sizeof(TYPE));
	}

	memcpy(base, _base, sizeof(TYPE)*count);

	if(NULL != p)
		delete[] p;
}

#endif	//#ifndef _QSort_h_
