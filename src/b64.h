/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */

#pragma once

#include "buf.h"

#include <jansson.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Returns the length of data after decoding.
 */
size_t __attribute__((warn_unused_result))
jose_b64_dlen(size_t elen);

/**
 * Returns the length of data after encoding.
 */
size_t __attribute__((warn_unused_result))
jose_b64_elen(size_t dlen);

/**
 * Decodes the encoded C string to a byte array.
 *
 * NOTE: The buffer MUST be at least as long as
 *       jose_b64_dlen(strlen(enc)).
 */
bool __attribute__((warn_unused_result))
jose_b64_decode(const char *enc, uint8_t dec[]);

/**
 * Decodes the encoded C string to a buffer.
 */
jose_buf_t * __attribute__((warn_unused_result))
jose_b64_decode_buf(const char *enc, bool lock);

/**
 * Decodes the encoded JSON string to a byte array.
 *
 * NOTE: The buffer MUST be at least as long as
 *       jose_b64_dlen(json_string_length(enc)).
 */
bool __attribute__((warn_unused_result))
jose_b64_decode_json(const json_t *enc, uint8_t dec[]);

/**
 * Decodes the encoded JSON string to a buffer.
 */
jose_buf_t * __attribute__((warn_unused_result))
jose_b64_decode_json_buf(const json_t *enc, bool lock);

/**
 * Decodes the encoded JSON string containing a JSON serialization.
 *
 * Upon successful decoding, the serialization is deserialized.
 */
json_t * __attribute__((warn_unused_result))
jose_b64_decode_json_load(const json_t *enc, int flags);

/**
 * Encodes the input byte array to a C string.
 *
 * NOTE: The enc parameter MUST be at least as long as
 *       jose_b64_elen(len) + 1.
 */
void
jose_b64_encode(const uint8_t dec[], size_t len, char enc[]);

/**
 * Encodes the input buffer to a C string.
 *
 * NOTE: The enc parameter MUST be at least as long as
 *       jose_b64_elen(len) + 1.
 */
void
jose_b64_encode_buf(const jose_buf_t *dec, char enc[]);

/**
 * Encodes the input byte array to a JSON string.
 */
json_t * __attribute__((warn_unused_result))
jose_b64_encode_json(const uint8_t dec[], size_t len);

/**
 * Encodes the input buffer to a JSON string.
 */
json_t * __attribute__((warn_unused_result))
jose_b64_encode_json_buf(const jose_buf_t *dec);

/**
 * Encodes the input JSON after serializing it.
 */
json_t * __attribute__((warn_unused_result))
jose_b64_encode_json_dump(const json_t *dec, int flags);
