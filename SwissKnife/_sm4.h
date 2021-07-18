#pragma once


#ifndef XYSSL_SM4_H
#define XYSSL_SM4_H

#define SM4_ENCRYPT     1
#define SM4_DECRYPT     0


typedef struct
{
	int mode;                     //���Ƽ��ܻ��ǽ���
	unsigned long sk[32];
}
sm4_context;


#ifdef __cplusplus
extern "C" {
#endif


	void sm4_setkey_enc(sm4_context *ctx, unsigned char key[16]);


	void sm4_setkey_dec(sm4_context *ctx, unsigned char key[16]);


	void sm4_crypt_ecb(sm4_context *ctx,
		int mode,
		int length,
		unsigned char *input,
		unsigned char *output);


	void sm4_crypt_cbc(sm4_context *ctx,
		int mode,
		int length,
		unsigned char iv[16],
		unsigned char *input,
		unsigned char *output);

#ifdef __cplusplus
}
#endif

#endif /* sm4.h */
