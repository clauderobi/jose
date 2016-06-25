/* vim: set tabstop=8 shiftwidth=4 softtabstop=4 expandtab smarttab colorcolumn=80: */

#include "ecdsa.h"
#include "conv.h"

#include <openssl/ecdsa.h>

#include <string.h>

static size_t
setup(const char *alg, EVP_PKEY *key, const char *prot, const char *payl,
      uint8_t hsh[])
{
    const EVP_MD *md = NULL;
    EVP_MD_CTX *ctx = NULL;
    const char *tmp = NULL;
    unsigned int ign = 0;
    size_t ret = 0;

    if (EVP_PKEY_base_id(key) != EVP_PKEY_EC)
        return 0;

    switch (EC_GROUP_get_curve_name(EC_KEY_get0_group(key->pkey.ec))) {
    case NID_X9_62_prime256v1: tmp = "ES256"; md = EVP_sha256(); break;
    case NID_secp384r1:        tmp = "ES384"; md = EVP_sha384(); break;
    case NID_secp521r1:        tmp = "ES512"; md = EVP_sha512(); break;
    default: return 0;
    }

    if (strcmp(alg, tmp) != 0)
        return 0;

    ctx = EVP_MD_CTX_create();
    if (!ctx)
        goto egress;

    if (EVP_DigestInit(ctx, md) <= 0)
        goto egress;

    tmp = prot ? prot : "";
    if (EVP_DigestUpdate(ctx, (const uint8_t *) tmp, strlen(tmp)) <= 0)
        goto egress;

    if (EVP_DigestUpdate(ctx, (const uint8_t *) ".", 1) <= 0)
        goto egress;

    tmp = payl ? payl : "";
    if (EVP_DigestUpdate(ctx, (const uint8_t *) tmp, strlen(tmp)) <= 0)
        goto egress;

    if (EVP_DigestFinal(ctx, hsh, &ign) > 0)
        ret = EVP_MD_size(md);

egress:
    EVP_MD_CTX_destroy(ctx);
    return ret;
}

const char *
ecdsa_suggest(EVP_PKEY *key)
{
    if (EVP_PKEY_base_id(key) != EVP_PKEY_EC)
        return NULL;

    switch (EC_GROUP_get_curve_name(EC_KEY_get0_group(key->pkey.ec))) {
    case NID_X9_62_prime256v1: return "ES256";
    case NID_secp384r1:        return "ES384";
    case NID_secp521r1:        return "ES512";
    default: return NULL;
    }
}

uint8_t *
ecdsa_sign(const char *alg, EVP_PKEY *key, const char *prot, const char *payl,
           size_t *len)
{
    uint8_t hsh[EVP_MAX_MD_SIZE];
    ECDSA_SIG *ecdsa = NULL;
    uint8_t *sig = NULL;
    size_t hl = 0;

    hl = setup(alg, key, prot, payl, hsh);
    if (hl == 0)
        return NULL;

    ecdsa = ECDSA_do_sign(hsh, hl, key->pkey.ec);
    if (!ecdsa)
        goto error;

    *len = (EC_GROUP_get_degree(EC_KEY_get0_group(key->pkey.ec)) + 7) / 8 * 2;
    sig = malloc(*len);
    if (!sig)
        goto error;

    if (!bn_encode(ecdsa->r, sig, *len / 2))
        goto error;

    if (!bn_encode(ecdsa->s, &sig[*len / 2], *len / 2))
        goto error;

    ECDSA_SIG_free(ecdsa);
    return sig;

error:
    ECDSA_SIG_free(ecdsa);
    free(sig);
    return NULL;
}

bool
ecdsa_verify(const char *alg, EVP_PKEY *key, const char *prot,
             const char *payl, const uint8_t sig[], size_t len)
{
    uint8_t hsh[EVP_MAX_MD_SIZE];
    ECDSA_SIG ecdsa = {};
    bool ret = false;
    size_t hl = 0;

    hl = setup(alg, key, prot, payl, hsh);
    if (hl == 0)
        return NULL;

    ecdsa.r = bn_decode(sig, len / 2);
    ecdsa.s = bn_decode(&sig[len / 2], len / 2);
    if (ecdsa.r && ecdsa.s)
        ret = ECDSA_do_verify(hsh, sizeof(hsh), &ecdsa, key->pkey.ec) == 1;

    BN_free(ecdsa.r);
    BN_free(ecdsa.s);
    return ret;
}