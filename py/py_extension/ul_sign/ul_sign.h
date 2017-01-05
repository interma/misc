/**
 * @file
 * @brief 生成数字签名
 */
//***********************************************************************
//			Signature Utilities (SU) 1.0
//
//   programmed by Ming Lei, Jul, 2000
//***********************************************************************


#ifndef __UL_SIGN_H__
#define __UL_SIGN_H__


/**
 *  为一段data创建唯一的64位数据签名
 *  
 *  @param[in]  psrc  src data
 *  @param[in]  slen  data的长度
 *  @param[out] sign1 返回的数字签名的高32位
 *  @param[out] sign2 返回的数字签名的低32位
 *  @return 操作结果
 * - 1   成功
 * - -1  失败
 *  @note 异常情况：
 * - if slen<0，有可能出现程序异常；
 * - if slen==0，sign = 0:0；
 */
int creat_sign_fs64 (char* psrc, int slen, unsigned int* sign1 , unsigned int* sign2);


#endif
