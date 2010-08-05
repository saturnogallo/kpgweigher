// Smart.h
//
// 本文件描述了如下模版类，这些类高效的解决了一些普遍的C++编程问题
// 注意：这些类不保证多线程安全

// 类图
//
//                    |===> CAutoPtr
//            inherit |
// CSmartBase =======>|                                            |===> CFreshTag
//                    |               inherit              combin  |
//                    |===> CSmartBuf =======> CSmartArray =======>|===> CRingQueue
//                                                                 |               combin
//                                                                 |===> CQuickMap ======> CQuickID
//

// guojun, 2004.01 beijing
// modified, 2004.03

#ifndef _Smart_h_
#define _Smart_h_

#include "AutoPtr.h"
#include "SmartBuf.h"
#include "SmartArray.h"
#include "FreshTag.h"
#include "RingQueue.h"
#include "QuickMap.h"
#include "QuickID.h"

#endif	// #ifdef _Smart_h_
