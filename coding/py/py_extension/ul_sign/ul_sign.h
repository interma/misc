/**
 * @file
 * @brief ��������ǩ��
 */
//***********************************************************************
//			Signature Utilities (SU) 1.0
//
//   programmed by Ming Lei, Jul, 2000
//***********************************************************************


#ifndef __UL_SIGN_H__
#define __UL_SIGN_H__


/**
 *  Ϊһ��data����Ψһ��64λ����ǩ��
 *  
 *  @param[in]  psrc  src data
 *  @param[in]  slen  data�ĳ���
 *  @param[out] sign1 ���ص�����ǩ���ĸ�32λ
 *  @param[out] sign2 ���ص�����ǩ���ĵ�32λ
 *  @return �������
 * - 1   �ɹ�
 * - -1  ʧ��
 *  @note �쳣�����
 * - if slen<0���п��ܳ��ֳ����쳣��
 * - if slen==0��sign = 0:0��
 */
int creat_sign_fs64 (char* psrc, int slen, unsigned int* sign1 , unsigned int* sign2);


#endif
