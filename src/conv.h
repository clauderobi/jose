/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */

#pragma once

#include "buf.h"

#include <openssl/bn.h>
#include <jansson.h>

BIGNUM *
bn_decode(const uint8_t buf[], size_t len);

BIGNUM *
bn_decode_buf(const jose_buf_t *buf);

BIGNUM *
bn_decode_json(const json_t *json);

bool
bn_encode(const BIGNUM *bn, uint8_t buf[], size_t len);

jose_buf_t *
bn_encode_buf(const BIGNUM *bn, size_t len);

json_t *
bn_encode_json(const BIGNUM *bn, size_t len);

json_t *
compact_to_obj(const char *compact, ...);

size_t
str_to_enum(const char *str, ...);

bool
has_flags(const char *flags, bool all, const char *query);

const unsigned char *
EVP_PKEY_get0_hmac(EVP_PKEY *pkey, size_t *len);
