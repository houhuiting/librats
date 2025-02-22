/* Copyright (c) 2022 Intel Corporation
 * Copyright (c) 2020-2022 Alibaba Cloud
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _LIBRATS_CLAIM_H_
#define _LIBRATS_CLAIM_H_

#include <stddef.h>
#include <stdint.h>
#include <librats/err.h>
#include <librats/log.h>

/**
 * Claims struct used for claims parameters.
 */
typedef struct claim claim_t;
struct claim {
	char *name;
	uint8_t *value;
	size_t value_size;
} __attribute__((packed));

void free_claims_list(claim_t *claims, size_t claims_length);
int librats_add_claim(claim_t *claim, const void *name, size_t name_size, const void *value,
		      size_t value_size);

/* This macro checks whether the expression argument evaluates to RATS_ERR_NONE */
#define CLAIM_CHECK(EXPRESSION)                           \
	do {                                              \
		rats_err_t _result_ = (EXPRESSION);       \
		if (_result_ != RATS_ERR_NONE) {          \
			RATS_ERR("failed to add claims"); \
			goto done;                        \
		}                                         \
	} while (0)

typedef int (*rats_verify_claims_callback_t)(claim_t *claims, size_t claims_size, void *args);

#endif /* _LIBRATS_CLAIM_H_ */
