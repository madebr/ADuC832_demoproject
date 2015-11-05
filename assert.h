#ifndef _ASSERT_H_
#define _ASSERT_H_

#define ct_assert(e) {enum { ct_assert_value = 1/(!!(e)) };}

#endif //_ASSERT_H_
