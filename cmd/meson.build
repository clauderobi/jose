rpath = ''
if buildtype == 'debug' or buildtype == 'debugoptimized'
  rpath = libjose_build_dir
endif

if (build_executable == true)
  executable(meson.project_name(),
    'jose.c', 'jose.h',
    'b64/b64.h',
    'b64/dec.c',
    'b64/enc.c',
    'jwk/jwk.h',
    'jwk/eql.c',
    'jwk/exc.c',
    'jwk/gen.c',
    'jwk/pub.c',
    'jwk/thp.c',
    'jwk/use.c',
    'jws/jws.h',
    'jws/fmt.c',
    'jws/sig.c',
    'jws/ver.c',
    'jwe/jwe.h',
    'jwe/pwd.h',
    'jwe/fmt.c',
    'jwe/dec.c',
    'jwe/enc.c',
    'alg.c',
    'fmt.c',

    dependencies: libjose_dep,
    install: true,
    install_rpath: rpath
  )
endif
