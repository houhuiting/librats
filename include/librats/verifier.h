/* Copyright (c) 2022 Intel Corporation
 * Copyright (c) 2020-2021 Alibaba Cloud
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _RATS_VERIFIER_H
#define _RATS_VERIFIER_H

#include <stdint.h>
#include <librats/api.h>
#include <librats/claim.h>
#include <librats/endorsement.h>

#define RATS_VERIFIER_TYPE_NAME_SIZE 32
#define RATS_VERIFIER_TYPE_MAX	     32

#define RATS_VERIFIER_API_VERSION_1	  1
#define RATS_VERIFIER_API_VERSION_MAX	  RATS_VERIFIER_API_VERSION_1
#define RATS_VERIFIER_API_VERSION_DEFAULT RATS_VERIFIER_API_VERSION_1

#define RATS_VERIFIER_OPTS_FLAGS_DEFAULT      0
#define RATS_VERIFIER_OPTS_FLAGS_SGX1_ENCLAVE (1 << 0)
#define RATS_VERIFIER_OPTS_FLAGS_SGX2_ENCLAVE (1 << 1)
#define RATS_VERIFIER_OPTS_FLAGS_TDX	      (1 << 2)
#define RATS_VERIFIER_OPTS_FLAGS_SNP	      (1 << 3)
#define RATS_VERIFIER_OPTS_FLAGS_SEV	      (1 << 4)
#define RATS_VERIFIER_OPTS_FLAGS_CSV	      (1 << 5)

typedef struct rats_core_context rats_core_context_t;
typedef struct rats_verifier_ctx rats_verifier_ctx_t;

extern rats_verifier_err_t rats_verifier_init(rats_conf_t *conf, rats_core_context_t *ctx);

typedef struct rats_verifier_opts {
	uint8_t api_version;
	unsigned long flags;
	const char name[RATS_VERIFIER_TYPE_NAME_SIZE];
	/* Different attester instances may generate the same format of verifier,
	 * e.g, sgx_ecdsa and sgx_ecdsa_qve both generate the format "sgx_ecdsa".
	 * By default, the value of type equals to name.
	 */
	char type[RATS_VERIFIER_TYPE_NAME_SIZE];
	uint8_t priority;

	/* Optional */
	rats_verifier_err_t (*pre_init)(void);
	rats_verifier_err_t (*init)(rats_verifier_ctx_t *ctx);
	rats_verifier_err_t (*verify_evidence)(
		rats_verifier_ctx_t *ctx, attestation_evidence_t *evidence, const uint8_t *hash,
		uint32_t hash_len, attestation_endorsement_t *endorsements /* optional */,
		claim_t **claims, size_t *claims_length);
	rats_verifier_err_t (*cleanup)(rats_verifier_ctx_t *ctx);
} rats_verifier_opts_t;

struct rats_verifier_ctx {
	rats_verifier_opts_t *opts;
	void *verifier_private;
	void *handle;

	union {
		struct {
			const char name[RATS_VERIFIER_TYPE_NAME_SIZE];
			uint8_t cert_type;
		} sgx_ecdsa;

		struct {
			const char name[RATS_VERIFIER_TYPE_NAME_SIZE];
			uint8_t cert_type;
		} tdx;
	} config;
};

static inline int rats_verifier_cmp(const void *a, const void *b)
{
	return (*(rats_verifier_ctx_t **)b)->opts->priority -
	       (*(rats_verifier_ctx_t **)a)->opts->priority;
}

#endif
